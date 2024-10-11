#include <bits/stdc++.h>
using namespace std;

class Pass2 {
public:
    map<int, pair<string, int>> symtab;  // Symbol table
    vector<pair<string, int>> littab;    // Literal table
    vector<string> pooltab;              // Pool table
    map<string, pair<string, string>> opcode;  // Opcode table
    vector<string> machineCode;          // Final machine code

    void loadTables() {
        // Load SYMTAB
        ifstream symfile("symtab.txt");
        string label;
        int address, index;
        while (symfile >> label >> address >> index) {
            symtab[index] = {label, address};
        }
        symfile.close();

        // Load LITTAB
        ifstream litfile("littab.txt");
        string literal;
        while (litfile >> literal >> address) {
            littab.push_back({literal, address});
        }
        litfile.close();

        // Load POOLTAB
        ifstream poolfile("pooltab.txt");
        string pool;
        while (poolfile >> pool) {
            pooltab.push_back(pool);
        }
        poolfile.close();

        // Load OPCODE Table (from Pass1)
        opcode = {
            {"STOP", {"IS", "00"}},
            {"ADD", {"IS", "01"}},
            {"SUB", {"IS", "02"}},
            {"MULT", {"IS", "03"}},
            {"MOVER", {"IS", "04"}},
            {"MOVEM", {"IS", "05"}},
            {"COMP", {"IS", "06"}},
            {"BC", {"IS", "07"}},
            {"DIV", {"IS", "08"}},
            {"READ", {"IS", "09"}},
            {"PRINT", {"IS", "10"}},
            {"START", {"AD", "01"}},
            {"END", {"AD", "02"}},
            {"ORIGIN", {"AD", "03"}},
            {"EQU", {"AD", "04"}},
            {"LTORG", {"AD", "05"}},
            {"DC", {"DL", "01"}},
            {"DS", {"DL", "02"}},
            {"AREG", {"RG", "01"}},
            {"BREG", {"RG", "02"}},
            {"CREG", {"RG", "03"}},
            {"DREG", {"RG", "04"}},
            {"LT", {"CC", "01"}},
            {"LE", {"CC", "02"}},
            {"EQ", {"CC", "03"}},
            {"GT", {"CC", "04"}},
            {"GE", {"CC", "05"}},
            {"ANY", {"CC", "06"}}
        };
    }

    void pass2() {
        loadTables(); // Load all required tables

        ifstream fin("output.txt");
        ofstream fout("finalMachineCode.txt");
        string line;
        
        while (getline(fin, line)) {
            stringstream ss(line);
            string lc, instr_type, code, operand;
            
            ss >> lc; // Load the location counter
            if (lc == " ") continue; // Skip empty lines
            
            ss >> instr_type >> code;

            // Handle Imperative Statements (IS)
            if (instr_type == "(IS") {
                fout << lc << " " << code << " ";  // Write Location Counter and Opcode

                while (ss >> operand) {
                    if (operand.substr(0, 2) == "(R") {
                        // It's a Register
                        fout << operand.substr(4, 1) << " ";  // Extract and print Register Code
                    } else if (operand.substr(0, 2) == "(S") {
                        // It's a Symbol
                        int symIndex = stoi(operand.substr(4, operand.length() - 5));
                        fout << symtab[symIndex].second << " ";  // Print Symbol Address
                    } else if (operand.substr(0, 2) == "(L") {
                        // It's a Literal
                        int litIndex = stoi(operand.substr(4, operand.length() - 5));
                        fout << littab[litIndex - 1].second << " ";  // Print Literal Address
                    } else {
                        fout << operand << " ";  // Direct value or other operands
                    }
                }
                fout << endl;

            // Handle Declarative Statements (DL)
            } else if (instr_type == "(DL") {
                fout << lc << " " << code << " ";
                while (ss >> operand) {
                    if (operand.substr(0, 2) == "(C") {
                        fout << operand.substr(4, operand.length() - 5) << " ";  // Constant value
                    }
                }
                fout << endl;

            // Handle Assembler Directives (AD)
            } else if (instr_type == "(AD") {
                // Do nothing, as assembler directives are handled during Pass1
                continue;
            }
        }

        fin.close();
        fout.close();

        cout << "Pass2 completed. Machine code generated in finalMachineCode.txt" << endl;
    }
};

int main() {
    Pass2 pass2;
    pass2.pass2();
    return 0;
}
