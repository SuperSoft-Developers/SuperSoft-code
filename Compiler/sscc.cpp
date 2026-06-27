#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << path << endl;
        exit(1);
    }

    string content, line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    return content;
}

// Remove espaços simples
string trim(string s) {
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
    while (!s.empty() && isspace(s.back())) s.pop_back();
    return s;
}

// Extrai string entre aspas
string extractString(const string& line) {
    size_t first = line.find('"');
    size_t last = line.rfind('"');

    if (first == string::npos || last == string::npos || first == last)
        return "";

    return line.substr(first + 1, last - first - 1);
}

vector<string> compileSSCtoCPP(const string& code) {
    vector<string> cpp;

    cpp.push_back("#include <iostream>");
    cpp.push_back("using namespace std;");
    cpp.push_back("");
    cpp.push_back("int main() {");

    size_t pos = 0;
    string line;

    while (pos < code.size()) {
        size_t next = code.find('\n', pos);
        if (next == string::npos) next = code.size();

        line = trim(code.substr(pos, next - pos));

        if (line.rfind("print", 0) == 0) {
            string text = extractString(line);
            cpp.push_back("    cout << \"" + text + "\" << endl;");
        }

        pos = next + 1;
    }

    cpp.push_back("    return 0;");
    cpp.push_back("}");

    return cpp;
}

void writeFile(const string& path, const vector<string>& lines) {
    ofstream file(path);
    for (auto& l : lines) {
        file << l << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Uso: sscc input.ssc output.cpp" << endl;
        return 0;
    }

    string inputFile = argv[1];
    string outputFile = argv[2];

    string code = readFile(inputFile);
    vector<string> cpp = compileSSCtoCPP(code);

    writeFile(outputFile, cpp);

    cout << "Compilado com sucesso: " << outputFile << endl;

    return 0;
}
