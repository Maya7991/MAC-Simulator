#include <iostream>
#include <fstream>
#include <unordered_map>
#include <regex>
#include <math.h>
#include "utils.cpp"


unsigned num_of_var, num_of_clauses;
std::vector<std::vector<std::string>> expressions;
std::vector<std::string> sim_var;

void parseMacFile(std::istream& input_file){
    
    std::string line;
    int rows;
    std::unordered_map<int, std::string> row_var_map;
    std::regex row_init_pattern(R"(\b(\d+)\s+(x\d+|-x\d+)\b)");
    std::regex memristor_init_pattern(R"(\b(\d+)\s(True|False)\b)");
    std::regex sim_pattern (R"(c(\s+)sim(\s+)((\d+)(\s*),?(\s*))+)");
    std::vector<std::vector<bool>> memory;
    
    // initialise all memristors to False

    while(std::getline(input_file, line)){

        if(line.empty()) continue;

        if(line[0u] == 'c'){            
            
            if(std::regex_match(line,sim_pattern)){
                std::cout << line;
                std::smatch match;
                // if (std::regex_search(line, match, std::regex("\\d+"))) {
                //     for (auto num : match) {
                //         sim_var.push_back(num);
                //     }
                // }

                std::regex digit_pattern("\\d+"); // Pattern to match one or more digits

                // Search for all occurrences of digits in the string
                std::sregex_iterator iter(line.begin(), line.end(), digit_pattern);
                std::sregex_iterator end;

                for (; iter != end; ++iter) {
                    sim_var.push_back('x'+(*iter)[0].str()); // Add matched digits to the list
                }
                
                //store it in a list
                std::cout << "Extracted Numbers:" << std::endl;
                for (const auto& num : sim_var) {
                    std::cout << num << std::endl;
                }
            }
            
            continue;
        }

        if(line[0u] == 'h'){
            // If needed add a header line
            std::istringstream headerStream(line);
            std::string p;
            headerStream >> p >> num_of_var >> num_of_clauses;
            rows = num_of_var * 2;
            std::cout << "num of clauses " << num_of_clauses << std::endl;
            memory.resize(rows, std::vector<bool>(num_of_clauses, false));
            continue;
        }
        // std::vector<std::vector<bool>> memory(rows, std::vector<bool>(num_of_clauses, false));

        // parse 1st line and create a row variable map
        std::sregex_iterator iter(line.begin(), line.end(), row_init_pattern);
        std::sregex_iterator end;
        while(iter != end){
            std::smatch match = *iter;
            // std::cout << "Row init match: "<< match.str() <<std::endl;
            row_var_map[std::stoi(match[1])] = match[2];
            ++iter;
        }

        //Meristor initialisation
        std::sregex_iterator iter1(line.begin(), line.end(), memristor_init_pattern);
        int row_num;
        bool bypass = true;
        while(iter1 != end){
            std::smatch match = *iter1;
            ++iter1;
            if(bypass){
                row_num = std::stoi(match[1]);
                bypass = false;
                continue;
            }   
            
            // std::cout << "memristor init: "<< row_num <<"x" << match[1] <<std::endl;
            memory[row_num][std::stoi(match[1])] = true;
        }

        // for (const auto& row : memory) {
        //     for (int value : row) {
        //         std::cout << value << " ";
        //     }
        //     std::cout << std::endl;
        // }

    }

    // Clause extraction
    
    for(int col=0; col< num_of_clauses; ++col){
    
        std::vector<std::string> expression;
        for(size_t row=0;row< 2*num_of_var; ++row){
            if(memory[row][col]){
                expression.push_back(row_var_map[row]);
            }
        }
        expressions.push_back(expression); // x1 x3 -x4 
    }

    prettyPrintVector(expressions);

}

bool checkVar(std::string input){
    std::regex pattern(R"((-?)x(\d+))");
    std::smatch match;

    if (std::regex_match(input, match, pattern)) {
        bool isNegated = (match[1].str() == "-");
        return !isNegated;  // Return true if not negated, false if negated
    }
    return false;
}

void simulate(){
    // generate input combinations for 2^n and assign it to variables 
    // Then evaluate the expression using or operator

    std::map<std::string, std::vector<bool>> testData;
    testData =  generateTestData(num_of_var);

    
    int num = pow(2, num_of_var);
    std::cout << "num pow: " << num << std::endl ;
    for (int i =0; i < num; i++){
        std::cout<<"-------*Simulation "<< i << " -------"<< std::endl;
        bool result = true;
        
        // bool skip_iteration = true;
        // for (size_t j = 1; j < sim_var.size(); ++j) {
        //     if (testData[sim_var[j]][i] != testData[sim_var[j + 1]][i]) {
        //         skip_iteration = false;
        //         break;
        //     }
        // }

        // if(skip_iteration){
        //     std::cout << "Skipping iteration: "<< i << std::endl;
        //     continue;
        // }
 
        for (std::vector<std::string> vect1D :  expressions)
        { 
            std::cout << "Clause ";
            bool col_result = false;
            for (std::string var : vect1D)
            {
                std::cout<<"var crazy "<<var;
                std::cout<<"var crazyyy "<<testData[var][i];
                bool value = checkVar(var) ? testData[var][i] : !testData[var.substr(1)][i] ;
                // std::cout << var << " " << value << " ";
                col_result = col_result || value;                
            }
            
            result = result && col_result; // check this
            
            // std::cout<< "    clause end**";
            // std::cout <<std::endl;
        }
        
        std::cout<< "result: " << result << " "<<std::endl;
        if (result) {
            std::cout << "Test Data for Simulation " << i << ":" << std::endl;
            for (const auto& entry : testData) {
                std::cout << entry.first << ": " << entry.second[i] << std::endl;
            }
        }
    }
}