#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

using namespace std;

/*
 * Regex for finding the function definition in cpp source code.
 * Using regex has few limitation as it can not cover all possible cases.
 * Ideally we can use some parser library like clang instead of relying on
 * regex to find functions in the source code.
 */
const regex REGEX_FUNCTION_DEFINITION("[a-zA-Z_][a-zA-Z0-9]*.*[\\s]+([a-zA-Z0-9]+)\\((.*)\\)");
const int LINE_COUNT_THRESHOLD = 15;
const int PARAMETER_COUNT_THRESHOLD = 3;
const float JACCARD_SIMILARITY_THRESHOLD = 0.75;

class FunctionDetails {
public:
    string functionBody;
    string functionName;
    int parameterCount;
    int lineCount;

    FunctionDetails(string functionBody, string functionName, int parameterCount, int lineCount) :
        functionBody(functionBody),
        functionName(functionName),
        parameterCount(parameterCount),
        lineCount(lineCount) {}
};

vector<FunctionDetails> parseCppSource(string filepath) {
    vector<FunctionDetails> methods;
    string methodBody;
    int openCurlyBraceCount = 0;
    bool skipLineBetweenFunction = true;
    int argumentCount = 0;
    int lineCount = 0;
    string functionName;

    ifstream sourceFile(filepath);
    string line;
    
    while (getline(sourceFile, line)) {
        smatch match;
        if (regex_search(line, match, REGEX_FUNCTION_DEFINITION)) {
            skipLineBetweenFunction = false;

            if (!methodBody.empty()) {
                FunctionDetails methodDetails(methodBody, functionName, argumentCount, lineCount);
                methods.push_back(methodDetails);
                methodBody.clear();
                lineCount = 0;
            }

            functionName = match[1].str();
            argumentCount = count(match[2].first, match[2].second, ',');
            argumentCount = argumentCount == 0 ? 0 : argumentCount + 1;
        }

        if (!skipLineBetweenFunction && !line.empty()) {
            lineCount++;
            methodBody += line + "\n";
        }

        openCurlyBraceCount += count(line.begin(), line.end(), '{');
        if (openCurlyBraceCount > 0) {
            openCurlyBraceCount -= count(line.begin(), line.end(), '}');
            if (openCurlyBraceCount == 0) {
                skipLineBetweenFunction = true;
            }
        }
    }
    if (!methodBody.empty()) {
        FunctionDetails methodDetails(methodBody, functionName, argumentCount, lineCount);
        methods.push_back(methodDetails);
    }

    return methods;
}

unordered_set<string> tokenize(string line) {
    unordered_set<string> set;
    int pos = 0;
    while (pos < line.length()) {
        string word;
        while (pos < line.length() && line[pos] != ' ' && line[pos] != '\n') {
            word += line[pos];
            pos++;
        }
        if (!word.empty()) {
            set.insert(word);
        }
        pos++;
    }
    return set;
}

double jaccardSimilarity(string str1, string str2) {
    // Tokenize the strings into sets of words
    unordered_set<string> set1 = tokenize(str1);
    unordered_set<string> set2 = tokenize(str2);

    // Compute the intersection and union of the sets
    unordered_set<string> intersection;
    for (auto word : set1) {
        if (set2.count(word)) intersection.insert(word);
    }

    unordered_set<string> unionset = set1;
    for (auto word : set2) {
        unionset.insert(word);
    }

    double similarity = static_cast<double>(intersection.size()) / unionset.size();

    return similarity;
}

void longMethodCheck(vector<FunctionDetails> functionDetails) {
    bool isLongFunction = false;
    for (const auto& functionData : functionDetails) {
        if (functionData.lineCount > LINE_COUNT_THRESHOLD) {
            isLongFunction = true;
            cout << "The " << functionData.functionName 
                 << " function is a Long Function. It contains " 
                 << functionData.lineCount << " lines of code." << endl;
        }
    };

    if (!isLongFunction) {
        cout << "No function is a Long Function." << endl;
    }

}

void longParameterListCheck(vector<FunctionDetails> functionDetails) {
    bool isLongParameterList = false;
    for (const auto& functionData : functionDetails) {
        if (functionData.parameterCount > PARAMETER_COUNT_THRESHOLD) {
            isLongParameterList = true;
            cout << functionData.functionName 
                 << " has a Long Parameter List. Its parameter list contains "
                 << functionData.parameterCount << " parameters." << endl;
        }
    };

    if (!isLongParameterList) {
        cout << "No function has a long parameter list." << endl;
    }
}

void duplicateCodeCheck(vector<FunctionDetails> functionDetails) {
    bool isDuplicate = false;
    for (int i = 0; i < functionDetails.size(); i++) {
        for (int j = i + 1; j < functionDetails.size(); j++) {
            if (jaccardSimilarity(functionDetails[i].functionBody, functionDetails[j].functionBody) > JACCARD_SIMILARITY_THRESHOLD) {
                isDuplicate = true;
                cout << functionDetails[i].functionName << " and "
                     << functionDetails[j].functionName << " are duplicated." << endl;
            }
        }
    }
    
    if (!isDuplicate) {
        cout << "No functions are duplicated.";
    }
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <cpp file>" << std::endl;
        return 1;
    }
    
    string filepath = argv[1];

    vector<FunctionDetails> functionDetails = parseCppSource(filepath);

    cout << "Welcome to Code Smell Detection! The file you input contains the following methods/functions:" << endl;
    for (int i = 0; i < functionDetails.size(); i++) {
        cout << functionDetails[i].functionName;
        if (i < functionDetails.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;

    int choice;
    do {
        cout << "Please choose what you want to do now:\n"
             << "1. Long Method/Function Detection\n"
             << "2. Long Parameter List Detection\n"
             << "3. Duplicated Code Detection\n"
             << "4. Quit\n"
             << "Enter your choice: ";

        cin >> choice;
        cout << endl;
        switch (choice) {
            case 1:
                longMethodCheck(functionDetails);
                break;
                
            case 2:
                longParameterListCheck(functionDetails);
                break;
                
            case 3:
                duplicateCodeCheck(functionDetails);
                break;

            case 4:
                cout << "Quitting the program." << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        cout << endl;
    } while (choice != 4);
    
    return 0;
}