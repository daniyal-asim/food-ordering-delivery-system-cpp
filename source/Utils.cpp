#include "../headers/Utils.h"
#include <ctime>
#include <cstdlib>

void Utils::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Utils::pause() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

bool Utils::isValidPhone(const string& phone) {
    if (phone.length() != 11) return false;
    if (phone[0] != '0' || phone[1] != '3') return false;
    
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool Utils::isValidRegNumber(const string& regNum) {
    if (regNum.length() != 7) return false;
    
    for (char c : regNum) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string Utils::getCurrentTimestamp() {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

int Utils::getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();  // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear buffer
        }
    }
}


double Utils::getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();  // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear buffer
        }
    }
}

void Utils::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}