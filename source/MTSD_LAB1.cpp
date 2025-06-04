#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

enum { ERROR0 = -10, ERROR1 = -11, ERROR2 = -12, ERROR3 = -13, ERROR4 = -14 };
enum { SUCCESS0 = -1, SUCCESS1 = 0, SUCCESS2 = 1};

int InputKey();
int InputFile(ifstream& file);
int IsStringValid(string input);
int EquationSolver(float k1, float k2, float k3);

int main(int argc, char* argv[]) {
    if (argc == 1) return InputKey();
    else if (argc == 2) {
        ifstream userfile(argv[1]);
        return InputFile(userfile);
    }
    else {
        cout << "ERROR0: Too many arguments";
        return ERROR0;
    }
}

int InputKey() {
    string input;
    const int COUNT = 3;
    float coeficients[COUNT];

    string messages[COUNT] = { 
    "Enter the first coeficient (a): ", 
    "Enter the second coeficient (b): ",
    "Enter the third coeficient (c): " };

    for (int cindex = 0; cindex < COUNT; cindex++) {
        cout << messages[cindex];
        cin >> input;

        if (!IsStringValid(input)) {
            cout << input << " is not a valid value (enter float with '.')\n";
            cindex--;
            continue;
        }
        coeficients[cindex] = stof(input);

        if (!cindex && !coeficients[cindex]) {
            cout << "First argument cannot be 0\n";
            cindex--;
            continue;
        }
    }

    return EquationSolver(coeficients[0], coeficients[1], coeficients[2]);
}

int InputFile(ifstream& file) {
    if (!file.is_open()) {
        cout << "ERROR1: Wrong input path or filename\n";
        return ERROR1;
    }

    string text;
    text.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    string textBuffer = "";

    int cindex = 0; //coeficient index
    const int COUNT = 3;
    float coeficients[COUNT];

    for (int i = 0; i < text.length(); i++) {
        if (text[i] == ' ' || i == text.length() - 1) {
            if (IsStringValid(textBuffer)) {
                coeficients[cindex] = stof(textBuffer);
                cindex++;
                textBuffer.clear();

                if (!(cindex - 1) && coeficients[0] == 0) {
                    cout << "ERROR3: First coeficient can not be 0";
                    return ERROR3;
                }
                if (cindex == COUNT) {
                    if (i == text.length() - 1 && text[i] == '\n') {
                       return EquationSolver(coeficients[0], coeficients[1], coeficients[2]);
                    }
                    else {
                        cout << "ERROR2: Invalid text fromat. Text must end with a single '\\n' and no characters between last coeficiient and '\\n'";
                        return ERROR2;
                    }
                }
                continue;
            }
            else {
                cout << "ERROR2: Invalid text format. Characters can not be converted to float";
                return ERROR2;
            }
        }
        textBuffer += text[i];
    }
}

int IsStringValid(string input) {
    for (int i = 0; i < input.length(); i++) {
        if (!isdigit(input[i]) && input[i] != '.' && input[i] != '-') { //if string isnt just digits and '.' return 0
            return 0;
        }
    }
    return 1;
}

int EquationSolver(float k1, float k2, float k3) {
    float discriminant = k2 * k2 - (4 * k1 * k3);

    cout << "Your equation is (" << k1 << ")^2 * x + (" << k2 << ") * x + (" << k3 << ") = 0\n";
    if (!discriminant) { //d == 0
        float solution = -k2 / (2 * k1);
        cout << "There is 1 root\nX = " << solution;
        return SUCCESS1;
    }
    else if (discriminant > 0) {
        float solution1 = (-k2 + sqrt(discriminant)) / (2 * k1);
        float solution2 = (-k2 - sqrt(discriminant)) / (2 * k1);
        cout << "There are 2 roots\nX1 = " << solution1 << "\nX2 = " << solution2;
        return SUCCESS2;
    }
    else {
        cout << "There are no roots";
        return SUCCESS0;
    }
}