#include <iostream>
#include <fstream>
#include <regex>
#include "DimacsParse.hpp"
#include <map>
#include <set>
#include <math.h>

void DimacsReader::parseDimacs(){

    std::string line;
    line_number = 1;
    std::smatch m;
    static std::regex problem_spec( R"(^p\s+([cd]nf)\s+([0-9]+)\s+([0-9]+)$)" );

    while(std::getline(f, line)){
        line_number++;
        if(line.empty()){
            line_number++;
            continue;
        }

        if(line[0u] == 'c'){
            line_number++;
            // store the comments and use it in writeMac() function to write it in output file
            comments.push_back(line);
            continue;
        }

        if (line[0] == 'p') {
            // Parse the header
            std::istringstream headerStream(line);
            std::string p;
            headerStream >> p >> format >> number_of_variables >> number_of_clauses;
            found_spec = true;
        } else {
            if(!found_spec){
                std::cout<<"Error: No header specification found";
            }
            // Parse clauses
            std::vector<int> clause;
            std::istringstream clauseStream(line);
            int literal;
            while (clauseStream >> literal && literal != 0) {
                clause.push_back(literal);
            }
            if (!clause.empty()) {
                clauses.push_back(clause);
            }
        }
    }
}

std::string constructVarName(int var_index){
    
    std::string varName = (var_index > 0 ? "x" : "-x");
    // std::cout<<"const ructVarName "<<var_index<< " "<< (varName + std::to_string(std::abs(var_index))) <<std::endl;
    return varName + std::to_string(std::abs(var_index));
}

void DimacsReader::writeMac(){

    size_t dotPosition = file_name.find_last_of(".");
    // std::string outputFilename = file_name.substr(0, dotPosition) + "_mac" + file_name.substr(dotPosition);
    std::string outputFilename = file_name.substr(0, dotPosition) + "_mac" + ".mac";

    std::ofstream outputFile(outputFilename); 

    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        return;
    }

    if (clauses.empty()) {
        std::cout << "No clauses were parsed." << std::endl;
    } else {
        std::cout << "Parsed Clauses:" << std::endl;
        for (const std::vector<int>& clause : clauses) {
            for (int literal : clause) {
                std::cout << literal << ' ';
            }
            std::cout<< std::endl; 
        }
    }

    if (!comments.empty()) {
        for (const auto& comment : comments) {
            outputFile << comment << std::endl;
        }
        outputFile << std::endl;
    }

    outputFile << "h " << number_of_variables << " "<< number_of_clauses << std::endl;

    // Step 1: Create row and variable map
    std::map<int,int> rowVariableMap;
    int row_num = -1;
    for (unsigned i = 1; i <= number_of_variables; i++) {
        rowVariableMap[i] = ++row_num;
        outputFile << row_num << " " << "x" << std::to_string(i) << " "; // Step 2: Write the first instruction to the output buffer
        rowVariableMap[-i] = ++row_num;        
        outputFile << row_num << " " << "-x" << std::to_string(i) << " ";// Step 2: Write the first instruction to the output buffer
    }

    outputFile << std::endl;


    // Step 3: Create column and clause map
    std::map<int, int> columnClauseMap;
    for (unsigned i = 0; i < clauses.size(); i++) {
        columnClauseMap[i + 1] = i;
    }

    // Step 4: Create variable and clause map
    std::map<int,std::vector<int>> variableClauseMap;
    for(int i=0; i<number_of_clauses; i++){
        for(auto literal: clauses[i]){
            variableClauseMap[literal].push_back(i+1);
        }
    }

    outputFile << std::endl;
    //writing the memristor initialisation
    for(unsigned i=1; i<= variableClauseMap.size(); i++){
        
        for(int j=2; j>0; j--){
            auto it = variableClauseMap.find(pow(-1, j)*i);
            if(it != variableClauseMap.end() ){
                outputFile << rowVariableMap[pow(-1, j)*i] << " True "; // change rowVariableMap to <int,int> , no need to construct varName
                for(auto clause : it->second){
                    outputFile << columnClauseMap[clause] << " False "; // change columnClauseMap to <int,int> , no need to construct varName
                }
                outputFile << std::endl;
            }
        }
    }

    outputFile << std::endl;

    outputFile << std::endl << "c Metrics: " << std::endl;
    outputFile << "c Crossbar size          : " << number_of_variables*2 << " x " << number_of_clauses <<std::endl;
    outputFile << "c Write cycles(2n)       : " << number_of_variables*2 << std::endl;
    outputFile << "c MAC cycles(2^n)        : " << pow(2, number_of_variables) << std::endl; 

    outputFile.close();
}