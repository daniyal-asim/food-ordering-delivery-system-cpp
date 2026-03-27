#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <limits>
using namespace std;

class Utils {
public:
    static void clearScreen();
    static void pause();
    static bool isValidPhone(const string& phone);
    static bool isValidRegNumber(const string& regNum);
    static string getCurrentTimestamp();
    
    static int getIntInput(const string& prompt);
    static double getDoubleInput(const string& prompt);
    static void clearInputBuffer();
};

#endif