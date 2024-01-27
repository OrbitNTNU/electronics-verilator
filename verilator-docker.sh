#!/bin/bash
# DESCRIPTION: Wrap a verilator call to run a docker container
#
# Copyright 2020 by Stefan Wallentowitz. This program is free software; you
# can redistribute it and/or modify it under the terms of either the GNU
# Lesser General Public License Version 3 or the Perl Artistic License
# Version 2.0.
# SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0

# Pull newest image 
docker pull verilator/verilator:4.038 > /dev/null

# Get the project root
project_root=$(realpath $3)

# Run the docker image with the user we "made" above
docker run -e CCACHE_DIR=/work/.ccache -v ${project_root}:/work -w /work/$2 --user $(id -u):$(id -g) verilator/verilator:$1 "${@:4}"
