#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class MACRO {
public:
    static vector<vector<string>> mnt;  // Macro Name Table
    static vector<string> mdt;          // Macro Definition Table
    static vector<string> pass1Output;  // Non-macro lines for Pass 2
    static unordered_map<string, int> alaIndexMap;  // ALA Mapping (parameter -> index)

    static int mntc, mdtc;

    static void pass1(const string& filename);           // Perform Pass 1
    static void savePass1Output(const string& filename); // Save Pass 1 output
    static void pass2(const string& outputFilename);     // Perform Pass 2

    static void displayMNT();
    static void displayMDT();
};

vector<vector<string>> MACRO::mnt;
vector<string> MACRO::mdt;
vector<string> MACRO::pass1Output;
unordered_map<string, int> MACRO::alaIndexMap;

int MACRO::mntc = 0;
int MACRO::mdtc = 0;

void MACRO::pass1(const string& filename) {
    cout << "---- STARTING PASS 1 ----\n";
    ifstream inp(filename);
    if (!inp.is_open()) {
        cerr << "Error: Unable to open input file.\n";
        return;
    }

    string line;
    while (getline(inp, line)) {
        cout << "Reading line: " << line << "\n";

        if (line == "MACRO" || line == "macro") {
            string macroName;
            getline(inp, line);
            istringstream iss(line);
            iss >> macroName;  // Extract macro name
            cout << "Found MACRO: " << macroName << "\n";

            // Store macro name and MDT index in MNT
            mnt.push_back({to_string(mntc + 1), macroName, to_string(mdtc)});
            mntc++;

            // Tokenize parameters and store them in ALA
            string param;
            int paramIndex = 0;
            while (iss >> param) {
                if (param.back() == ',') param.pop_back();  // Remove trailing comma
                size_t equalPos = param.find('=');
                if (equalPos != string::npos) {
                    param = param.substr(0, equalPos);  // Get parameter name
                }
                alaIndexMap[param] = paramIndex++;  // Map parameter to index
                cout << "Added parameter: " << param << " with index: " << alaIndexMap[param] << "\n";
            }

            // Store macro body in MDT
            while (getline(inp, line)) {
                if (line == "MEND" || line == "mend") {
                    mdt.push_back(line);
                    mdtc++;
                    cout << "Macro ends with MEND.\n";
                    break;
                }
                mdt.push_back(line);
                mdtc++;
                cout << "Adding line to MDT: " << line << "\n";
            }
        } else {
            pass1Output.push_back(line);
            cout << "Non-macro line added to Pass 1 output: " << line << "\n";
        }
    }
    inp.close();
}

void MACRO::savePass1Output(const string& filename) {
    cout << "---- SAVING PASS 1 OUTPUT ----\n";
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Unable to open Pass 1 output file.\n";
        return;
    }

    for (const auto& line : pass1Output) {
        out << line << "\n";
        cout << "Written to Pass 1 output file: " << line << "\n";
    }

    out.close();
    cout << "Pass 1 output saved to " << filename << "\n";
}

void MACRO::pass2(const string& outputFilename) {
    cout << "---- STARTING PASS 2 ----\n";
    ofstream out(outputFilename);
    if (!out.is_open()) {
        cerr << "Error: Unable to open output file.\n";
        return;
    }

    for (const auto& line : pass1Output) {
        istringstream iss(line);
        string word;
        iss >> word;

        bool isMacro = false;
        for (const auto& entry : mnt) {
            if (entry[1] == word) {
                cout << "Macro call detected: " << word << "\n";
                isMacro = true;
                int mdtIndex = stoi(entry[2]);
                vector<string> actualArgs;

                // Extract actual arguments from the macro call
                string arg;
                while (getline(iss, arg, ',')) {
                    size_t equalPos = arg.find('=');
                    if (equalPos != string::npos) {
                        arg = arg.substr(equalPos + 1);  // Extract argument after '='
                    }
                    actualArgs.push_back(arg);
                    cout << "Captured actual argument: " << arg << "\n";
                }

                // Expand the macro using MDT
                for (int i = mdtIndex; i < mdt.size() && mdt[i] != "MEND"; ++i) {
                    istringstream mdtLine(mdt[i]);
                    string token;
                    while (mdtLine >> token) {
                        cout << "Processing token: " << token << "\n";
                        if (token.back() == ',') token.pop_back();

                        // Check if token is a parameter and replace with actual argument
                        if (alaIndexMap.find(token) != alaIndexMap.end()) {
                            int argIndex = alaIndexMap[token];
                            cout << "Found parameter: " << token << " with index: " << argIndex << "\n";
                            if (argIndex < actualArgs.size()) {
                                out << actualArgs[argIndex] << " ";
                                cout << "Replaced with: " << actualArgs[argIndex] << "\n";
                            }
                        } else {
                            out << token << " ";
                            cout << "Written as-is: " << token << "\n";
                        }
                    }
                    out << "\n";
                    cout << "\n";
                }
                break;
            }
        }

        if (!isMacro) {
            out << line << "\n";
            cout << "Non-macro line written: " << line << "\n";
        }
    }
    out.close();
    cout << "Pass 2 completed. Output written to " << outputFilename << "\n";
}

void MACRO::displayMNT() {
    cout << "\nMACRO NAME TABLE (MNT)\n";
    for (const auto& entry : mnt) {
        for (const auto& field : entry) {
            cout << field << " ";
        }
        cout << "\n";
    }
}

void MACRO::displayMDT() {
    cout << "\nMACRO DEFINITION TABLE (MDT)\n";
    for (const auto& line : mdt) {
        cout << line << "\n";
    }
}

int main() {
    string filename = "macro_pass1_input.txt";
    string pass1OutputFile = "pass1_output.txt";
    string pass2OutputFile = "macro_pass2_output.txt";

    MACRO::pass1(filename);
    MACRO::savePass1Output(pass1OutputFile);
    MACRO::displayMNT();
    MACRO::displayMDT();
    MACRO::pass2(pass2OutputFile);

    return 0;
}
