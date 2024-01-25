#include <iostream>
#include <fstream>
#include <string>

int get_stimuli(std::string filename, int *data) {
    std::ifstream file {filename};

    std::string as_string;
    int i = 0;
    while(std::getline(file, as_string, ',')) {
        data[i++] = std::stoi(as_string);
    }

    return i;
}
