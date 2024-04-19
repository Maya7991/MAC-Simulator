#include <fstream>
#include <iostream>
#include "DimacsParse.hpp"
#include "limits.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // char cwd[PATH_MAX];
    // if (getcwd(cwd, sizeof(cwd)) != NULL) {
    //     std::cout << "Current working directory: " << cwd << std::endl;
    // } else {
    //     perror("getcwd() error");
    // }
    std::ifstream inputFile1(argv[1]);
    if(!inputFile1.is_open()){
        std::cerr <<"Error opening file 1." << std::endl;
    }
    DimacsReader dimacsReader(inputFile1, argv[1]);
    dimacsReader.parseDimacs();

    if(argc > 2){
        std::ifstream inputFile2(argv[2]);
        if(!inputFile2.is_open()){
            std::cerr <<"Error opening file 2." << std::endl;
        }       
        DimacsReader dimacsReader2(inputFile2, argv[2]);
        dimacsReader2.parseDimacs();
        if(dimacsReader2.number_of_variables > dimacsReader.number_of_variables)
        {
            dimacsReader.number_of_variables = dimacsReader2.number_of_variables;
        }
        dimacsReader.number_of_clauses = dimacsReader.number_of_clauses + dimacsReader2.number_of_clauses;
        dimacsReader.clauses.insert(
            dimacsReader.clauses.end(),
            dimacsReader2.clauses.begin(),
            dimacsReader2.clauses.end()
        );
        inputFile2.close();
    }

    std::cout << "dimacsReader.number_of_variables : " << dimacsReader.number_of_variables << std::endl;
    std::cout << "dimacsReader.number_of_clauses : " << dimacsReader.number_of_clauses << std::endl;

    dimacsReader.writeMac();
    inputFile1.close();  
    return 0;
}
