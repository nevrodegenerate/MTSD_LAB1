#include "MDtoHTML.h"

using namespace std;

string outputText;

int main(int argc, char* argv[]) {
    bool out = false;

    if (argc == 1) {
        cerr << "ERROR0: Need at least 1 argument\n";
        return ERROR0;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        cerr << "ERROR0: Wrong input path or filename\n";
        return ERROR0;
    }

    if (argc > 2) {
        if (argv[2] == string("--out") && argc > 3) {
            out = true;
        }
    }

    switch (markdownToHtml(inputFile, &outputText)) {
    case SUCCESS:
        if (out) {
            ofstream outputFile(argv[3]);
            outputFile << outputText << "\n";
            cout << "File has been saved to: " << argv[3] << "\n";
            outputFile.close();
        }
        else
            cout << outputText << "\n";
        break;
    case ERROR1:
        cerr << "ERROR1: There is a start element but no end element\n";
        return ERROR1;
        break;
    case ERROR2:
        cerr << "ERROR2: There is an element between two other elements\n";
        return ERROR1;
        break;
    default:
        cerr << "ERROR: unknown error\n";
        return ERROR0;
        break;
    }
    inputFile.close();
    return 0;
}
