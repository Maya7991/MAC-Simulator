#include <iostream>
#include <cmath>
#include <vector>

std::vector<std::vector<int>> generateBinaryCombinations(int numVariables) {

    // Calculate the total number of combinations
    int totalCombinations = pow(2, numVariables);

    // Declare arrays for variables
    std::vector<std::vector<int>> variables(numVariables, std::vector<int>(totalCombinations, 0));

    // Generate and store all combinations
    for (int i = 0; i < totalCombinations; i++) {
        for (int j = 0; j < numVariables; j++) {
            variables[j][i] = (i >> j) & 1; // Store the binary value in the respective array
        }
    }

    // Print the stored binary values for each variable
    for (int i = 0; i < numVariables; i++) {
        std::cout << "Variable " << char('a' + i) << ": ";
        for (int j = 0; j < totalCombinations; j++) {
            std::cout << variables[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return variables;
}

void prettyPrintVector(std::vector<std::vector<std::string>>& expressions){
    for (std::vector<std::string> vect1D : expressions)
    {
        for (std::string x : vect1D)
        {
            std::cout << x << " ";
        }    
        std::cout << std::endl;
    }
}


std::vector<std::vector<bool>> generateAllCombinations(int numVariables) {
    int totalCombinations = 1 << numVariables;  // 2^numVariables
    std::vector<std::vector<bool>> combinations;

    for (int i = 0; i < totalCombinations; ++i) {
        std::vector<bool> combination;
        for (int j = numVariables - 1; j >= 0; --j) {
            combination.push_back((i >> j) & 1);
        }
        combinations.push_back(combination);
    }

    return combinations;
}

// Function to generate a variable name based on the variable index
std::string generateVariableName(int variableIndex) {
    return "x" + std::to_string(variableIndex + 1);
}

std::map<std::string, std::vector<bool>> generateTestData(int numVariables)
{

    std::vector<std::vector<bool>> allCombinations = generateAllCombinations(numVariables);

    // Create a map to store input combinations for each variable
    std::map<std::string, std::vector<bool>> variableMap;

    // Populate the map with input combinations
    for (int i = 0; i < numVariables; ++i) {
        std::string variableName = generateVariableName(i);
        for (const auto& combination : allCombinations) {
            variableMap[variableName].push_back(combination[i]);
        }
    }

    // Print the values in the variableMap
    for (const auto& entry : variableMap) {
        std::cout << entry.first << ": ";
        for (bool value : entry.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return variableMap;
  
}