#include <iostream>
#include <fstream>
#include "simulator.cpp"

int main(int argc, char const *argv[]){

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1; // Indicate failure
    }

    std::ifstream input_file(argv[1]);
    if(!input_file.is_open()){
        std::cerr << "Unable to open the file: " << argv[1] << std::endl;
    }

    parseMacFile(input_file);
    simulate();

    input_file.close();
    return 0;
}