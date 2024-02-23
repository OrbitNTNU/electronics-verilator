import pycparser
import os
import argparse
import math

argparser = argparse.ArgumentParser()
argparser.add_argument('projectname', help='The name of the project')
argparser.add_argument('-f', '--project-folder', help='Project folder location', default='.')
args = argparser.parse_args()

INPUTFILE = args.project_folder + '/obj_dir/V' + args.projectname + '.h'
OUTPUTFOLDER = args.project_folder + '/enums'
try:
    with open(INPUTFILE, 'r') as f:
        lines = f.readlines()
except FileNotFoundError:
    print('Not found:', INPUTFILE)
    print('Verilator not run?')
    exit(1)



in_enum = False
enumfile = ""

for line in lines:
    if not in_enum and "enum" in line:
        in_enum = True
    if in_enum:
        enumfile += line
    if in_enum and "};" in line:
        in_enum = False

parser = pycparser.c_parser.CParser()
ast = parser.parse(enumfile, filename='<none>')

def get_enums():
    return ast.ext

def get_enum_statename_only(name):
    # Verilator names the states like so:
    # Top__DOT__State
    # We only want the 'State' part
    return name.split('__DOT__')[-1]

def get_enum_highest_value(enum):
    return int(
        enum.type.values.enumerators[-1].value.value[:-1]
    )

def get_enum_value_binary_encoded(value, bits):
    value = int(value[:-1])
    return "{0:b}".format(value).zfill(bits)

def enum2txt(enum, bits):
    txt = ""
    name = get_enum_statename_only(enum.type.name)
    for e in enum.type.values.enumerators:
        txt += get_enum_value_binary_encoded(e.value.value, bits) + ' ' + e.name + '\n'
    return txt, name

enums = get_enums()

if not os.path.exists(OUTPUTFOLDER):
    os.mkdir(OUTPUTFOLDER)

for enum in enums:
    highest = get_enum_highest_value(enum)
    bits = int(math.log2(highest)) + 1
    (enumcfgfile, name) = enum2txt(enum, bits)
    with open (OUTPUTFOLDER + '/' + name + '.gtkw', 'w') as f:
        f.write(enumcfgfile)
