#pragma once

#pragma once

using namespace std;

#include <iostream>
#include <fstream>
#include <string>

enum { ERROR0 = -10, ERROR1 = -1, ERROR2 = -2, SUCCESS = 1 };

extern int markdownToHtml(ifstream& file, string* outputString);