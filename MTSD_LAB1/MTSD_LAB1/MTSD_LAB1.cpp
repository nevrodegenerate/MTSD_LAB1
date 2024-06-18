#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int markdownToHtml(ifstream& file);

string outputText;

enum { ERROR0 = -10, ERROR1 = -1, ERROR2 = -2, SUCCESS = 1 };

int main(int argc, char* argv[]) {
    if (argc == 1) {
        cerr << "ERROR0: Need at least 1 argument\n";
        return ERROR0;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        cerr << "ERROR0: Wrong input path or filename\n";
        return ERROR0;
    }

    switch (markdownToHtml(inputFile)) {
    case SUCCESS:
        if (argc > 3 && argv[2] == string("--out")) {
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
    system("pause");
    inputFile.close();
    return 0;
}

int markdownToHtml(ifstream& file) {
    string textBuffer;
    bool preform = false;
    bool paragr = false;
    int pos1 = 0;
    int pos2 = 0;

    const int arraySize = 5;
    //двовимірний масив dataArray, в якому кожен елемент розмітки має:
    //[0] - початковий елемент розмітки markdown
    //[1] - кінцевий елемент розмітки markdown
    //[2] - початковий елемент розмітки html
    //[3] - кінцевий елемент розмітки html
    //(в md початкові та кінцеві елементи є однаковими,
    //але існує можливість переробки програми для несиметричних систем розмітки)
    string dataArray[arraySize][4] = {
        {"```", "```", "<pre>", "</pre>"}, //перший індекс завжди є преформатом
        {"\n\n", "\n\n", "<p>", "</p>"}, //другий індекс завжди є параграфом
        {"**", "**", "<b>", "</b>"},
        {"_", "_", "<i>", "</i>"},
        {"`", "`", "<tt>", "</tt>"} };

    while (getline(file, textBuffer)) {
        //преформати
        if (textBuffer == dataArray[0][0] && !preform) {
            textBuffer.replace(0, dataArray[0][0].length(), dataArray[0][2]);
            preform = true;
        }

        if (textBuffer == dataArray[0][1] && preform) {
            textBuffer.replace(0, dataArray[0][1].length(), dataArray[0][3]);
            preform = false;
        }
        //параграфи
        while ((pos1 = outputText.find(dataArray[1][0], 0)) != string::npos) {
            paragr = true;
            if (paragr)
                outputText.replace(pos1, 2, dataArray[1][3] + "\n" + dataArray[1][2]);
        }

        if (!preform) {
            pos1 = 0;
            pos2 = 0;

            for (int i = 2; i < arraySize; i++) {
                while ((pos1 = textBuffer.find(dataArray[i][0], pos2)) != string::npos) {
                    if (textBuffer[pos1 + dataArray[i][0].length()] == ' '
                        || pos1 > 0 && textBuffer[pos1 - 1] != ' ') { //елемент розмітки не є відірваним від тексту або частиною слова
                        break;
                    }
                    if ((pos2 = textBuffer.find(dataArray[i][1], pos1 + dataArray[i][2].length()))
                        != string::npos
                        && textBuffer[pos2 + dataArray[i][1].length()] == ' '
                        || pos2 + dataArray[i][1].length() == textBuffer.length()
                        && pos2 > 0 && textBuffer[pos2 - 1] != ' ') {
                        textBuffer.replace(pos1, dataArray[i][0].length(), dataArray[i][2]);
                        pos2 = textBuffer.find(dataArray[i][1], pos1 + dataArray[i][2].length());
                        textBuffer.replace(pos2, dataArray[i][1].length(), dataArray[i][3]);

                        //робимо відрізок тексту і шукаємо в ньому вкладені елементи розмітки
                        string textSubStr = textBuffer.substr(pos1 + dataArray[i][2].length(),
                            pos2 - pos1 - dataArray[i][2].length());
                        int tmppos1 = 0;
                        int tmppos2 = 0;
                        for (int j = 2; j < arraySize; j++) {
                            if ((tmppos1 = textSubStr.find(dataArray[j][0])) != string::npos) {
                                if (textSubStr[tmppos1 + dataArray[j][0].length()] != ' ') {
                                    if (tmppos1 == 0 || textSubStr[tmppos1 - 1] == ' ') {
                                        return ERROR2;
                                    }
                                }
                                if ((tmppos2 = textSubStr.find(dataArray[j][1], tmppos1 + dataArray[j][0].length())
                                    != string::npos)) {
                                    if (tmppos2 == textSubStr.length() || textSubStr[tmppos2 + 1] != ' ') {
                                        if (tmppos2 > 0 && textSubStr[tmppos2 - 1] != ' ') {
                                            return ERROR2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                        return ERROR1;
                }
            }
        }
        outputText += textBuffer + "\n";
    }

    if (preform)
        return ERROR1;

    if (paragr) {
        outputText = dataArray[1][2] + outputText;
        outputText.replace(outputText.length() - 1, 1, dataArray[1][3]);
    }

    return SUCCESS;
}

