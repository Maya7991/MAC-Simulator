#include <vector>
#include <fstream>

struct DimacsReader{

    std::istream &f;
    unsigned line_number;
 // cnf DIMACS format --> p format variables clauses
    std::string format, file_name;
    unsigned number_of_variables, number_of_clauses;
    std::vector<int> clause;
    std::vector<std::vector<int>> clauses;  // std::vector<clause> clauses
    bool found_spec = false;
    std::vector<std::string> comments;

    DimacsReader(std::istream &f, std::string file_name): f(f), file_name(file_name) {};
    void parseDimacs();
    void writeMac();

};
