#include <bits/stdc++.h>
using namespace std;

class Pass1 {
public:
    map<string, pair<string, string>> opcode;
    map<string, pair<int, string>> symtab;
    vector<pair<string, int>> littab;
    vector<string> pooltab;
    int litindex = 0, lc = -1;

    void OpcodeMap() {
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

   void pass1() 
{
    OpcodeMap();
    ifstream fin("inputPass1.txt");
    ofstream fout("output.txt");
    string line, word;
    symtab.clear();
    littab.clear();
    pooltab.clear();
    litindex = 0;
    lc = 0; 

    while (getline(fin, line)) 
    {
        stringstream st(line);
        st >> word;
        string label = "";

        // If the word is not an opcode, treat it as a label
        if (opcode.count(word) == 0) 
        {
            if (symtab.count(word) == 0) 
            {
                symtab[word] = {lc, to_string(symtab.size() + 1)};
            } else 
            {
                symtab[word].first = lc;
            }
            label = word; 
            st >> word;
        }

        string operation = word;

        if (operation == "START") 
        {
            fout << "    (" << opcode[word].first << ", " << opcode[word].second << ") ";
            st >> word;
            fout << "(C, " << word << ") ";
            lc = stoi(word);
        } 
        else if (operation == "END") 
        {
            fout << "    (" << opcode[word].first << ", " << opcode[word].second << ") ";
            fout << endl;
            if (!littab.empty()) 
            {
                pooltab.push_back("#" + to_string(litindex + 1));
                for (; litindex < littab.size(); litindex++) 
                {
                    fout << lc << " ";
                    fout << "(" << opcode["DC"].first << ", " << opcode["DC"].second << ") ";
                    littab[litindex].second = lc;
                    string literal = littab[litindex].first;
                    string sublit = literal.substr(2, literal.size() - 3);
                    fout << "(C, " << sublit << ") ";
                    fout << endl;
                    lc++;
                }
            }
            // Print literals
            cout << "Literals after END directive:" << endl;
            for (const auto& lit : littab) {
                cout << "Literal: " << lit.first << ", Address: " << lit.second << endl;
            }
        } 
        else if (operation == "LTORG") 
        {
            fout << "    (" << opcode[word].first << ", " << opcode[word].second << ") ";
            fout << endl;
            if (!littab.empty()) 
            {
                pooltab.push_back("#" + to_string(litindex + 1));
                for (; litindex < littab.size(); litindex++) 
                {
                    fout << lc << " ";
                    fout << "(" << opcode["DC"].first << ", " << opcode["DC"].second << ") ";
                    littab[litindex].second = lc;
                    string literal = littab[litindex].first;
                    string sublit = literal.substr(2, literal.size() - 3);
                    fout << "(C, " << sublit << ") ";
                    fout << endl;
                    lc++;
                }
            }
            // Print literals
            cout << "Literals after LTORG directive:" << endl;
            for (const auto& lit : littab) {
                cout << "Literal: " << lit.first << ", Address: " << lit.second << endl;
            }
        } 
        else if (operation == "EQU") 
        {
            fout << "    ";
            st >> word;
            int plusminusindex = word.find_first_of("+-");
            char plusminus = '0';
            string aftersign = "0";
            string beforesign;
            if (plusminusindex != string::npos) 
            {
                plusminus = word[plusminusindex];
                aftersign = word.substr(plusminusindex + 1);
                beforesign = word.substr(0, plusminusindex);
            } else 
            {
                beforesign = word.substr(0, word.length());
            }
          
            if (symtab.count(beforesign)) 
            {
                symtab[label].first = symtab[beforesign].first;
                if (plusminus == '+') 
                {
                    if (aftersign != "0")
                        symtab[label].first += stoi(aftersign);
                } else if (plusminus == '-') 
                {
                    if (aftersign != "0")
                        symtab[label].first -= stoi(aftersign);
                }
            }
        } 
        else if (operation == "ORIGIN") 
        {
            fout << "    ";
            fout << "(" << opcode[word].first << ", " << opcode[word].second << ") ";
            st >> word;
            int plusminusindex = word.find_first_of("+-");
            char plusminus = '0';
            string beforesign, aftersign;
            if (plusminusindex != string::npos) 
            {
                plusminus = word[plusminusindex];
                aftersign = word.substr(plusminusindex + 1);
                beforesign = word.substr(0, plusminusindex);
            } 
            else 
            {
                beforesign = word.substr(0, word.length());
            }
         
            if (symtab.count(beforesign)) 
            {
                lc = symtab[beforesign].first;
                fout << "(S , " << symtab[beforesign].second << ")";
                if (plusminus == '+') 
                {
                    lc += stoi(aftersign);
                    fout << "+" << aftersign << "\n";
                } 
                else if (plusminus == '-') 
                {
                    lc -= stoi(aftersign);
                    fout << "-" << aftersign << "\n";
                }
            }
        } 
        else 
        {   
            // cout<<"entered main else"<<endl;
            fout << lc << " ";
            fout << "(" << opcode[word].first << ", " << opcode[word].second << ") ";
            while (st >> word) 
            {
                // cout<<"word : "<<word<<endl;
                // cout<<"operr : "<<operation<<endl;                // Remove trailing commas
                if (!word.empty() && word.back() == ',') {
                    word = word.substr(0, word.size() - 1);
                }

                if (operation == "DC") 
                {
                    auto tmp = word.substr(1, word.length() - 2);
                    fout << "(C, " << tmp << ") ";
                } 
                else if (operation == "DS") 
                {
                    fout << "(C, " << word << ") ";
                    lc += stoi(word) - 1;
                } 
                // cout<<"new word "<<word<<endl;
                if (word[1] == '=') 
                {   
                    // cout<<"entered littt"<<endl;
                    // Check if the literal already exists to avoid duplicates
                    auto it = find_if(littab.begin(), littab.end(),
                                    [&](const pair<string, int>& p) { return p.first == word; });
                    if (it == littab.end()) 
                    {
                        littab.push_back({word, -1}); // Address will be assigned later
                        fout << "(L, " << littab.size() << ") ";
                        // Print literals
                        cout << "Added Literal: " << word << endl;
                    } 
                    else 
                    {
                        cout<<"literal not added";
                        fout << "(L, " << (distance(littab.begin(), it) + 1) << ") ";
                    }
                } 
                else if (opcode.count(word) > 0 && opcode[word].first != "IS") 
                {
                    fout << "(" << opcode[word].first << ", " << opcode[word].second << ") ";
                } 
                else if (opcode.count(word) > 0 && opcode[word].first == "IS")
                {
                    fout << "(" << opcode[word].first << ", " << opcode[word].second << ") ";
                } 
                else 
                {
                    if (symtab.count(word) == 0) 
                    {
                        symtab[word] = {lc, to_string(symtab.size() + 1)};
                    }
                    fout << "(S, " << symtab[word].second << ") ";
                }
            }
        }
        fout << endl;
        if (operation != "ORIGIN" && operation != "EQU") 
        {
            lc++;
        }
    }

    // Handle any remaining literals at the end of the assembly program
    if (!littab.empty()) 
    {
        pooltab.push_back("#" + to_string(litindex + 1));
        for (; litindex < littab.size(); litindex++) 
        {
            fout << lc << " ";
            fout << "(" << opcode["DC"].first << ", " << opcode["DC"].second << ") ";
            littab[litindex].second = lc;
            string literal = littab[litindex].first;
            string sublit = literal.substr(2, literal.size() - 3);
            fout << "(C, " << sublit << ") ";
            fout << endl;
            lc++;
        }
    }
    fout.close();
    fin.close();

    // Write symbol table
    ofstream symfile("symtab.txt");
    for (auto& entry : symtab) 
    {
        symfile << entry.first << " " << entry.second.first << " " << entry.second.second << "\n";
    }
    symfile.close();

    // Write literal table
    ofstream litfile("littab.txt");
    for (auto& entry : littab) 
    {
        litfile << entry.first << " " << entry.second << "\n";
    }
    litfile.close();

    // Write pool table
    ofstream poolfile("pooltab.txt");
    for (auto& entry : pooltab) 
    {
        poolfile << entry << "\n";
    }
    poolfile.close();

    cout << "\nInput file is Processed Successfully!" << endl;
}

};

int main() 
{
    Pass1 p1;
    p1.pass1();
    return 0;
}