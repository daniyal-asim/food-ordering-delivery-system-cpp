#include "../headers/User.h"
#include <iostream>
#include <sstream>

// User class implementation
User::User() {
    name = "";
    phone = "";
    password = "";
    role = STUDENT;
    regNumber = "";
}

User::User(string name, string phone, string password, UserRole role, string regNumber) {
    this->name = name;
    this->phone = phone;
    this->password = password;
    this->role = role;
    this->regNumber = regNumber;
}

string User::getRoleString() const {
    switch(role) {
        case STUDENT: return "Student";
        case KITCHEN: return "Kitchen Staff";
        case DELIVERY: return "Delivery Personnel";
        case ADMIN: return "Admin";
        default: return "Unknown";
    }
}

void User::display() const {
    cout << "\n--- User Information ---\n";
    cout << "Name: " << name << endl;
    
    if (role == STUDENT) {
        cout << "Phone: " << phone << endl;
        cout << "Reg Number: " << regNumber << endl;
    } else {
        cout << "Username: " << phone << endl;
    }
    
    cout << "Role: " << getRoleString() << endl;
}

// UserManager implementation
UserManager::UserManager(string studentFile, string adminFile) {
    this->studentFile = studentFile;
    this->adminFile = adminFile;
    loadUsers();
}

void UserManager::loadUsers() {
    loadAdminUsers();    // Load staff first
    loadStudentUsers();  // Then load students
}

void UserManager::loadAdminUsers() {
    ifstream file(adminFile);
    if (!file.is_open()) {
        cout << "Warning: Could not open admin file. Please check data/admin_users.txt\n";
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    
    int loadedCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string username, name, password, roleStr;
        
        // Format: username,name,password,role
        getline(ss, username, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        getline(ss, roleStr, ',');
        
        UserRole role = ADMIN;
        if (roleStr == "kitchen") role = KITCHEN;
        else if (roleStr == "delivery") role = DELIVERY;
        else if (roleStr == "admin") role = ADMIN;
        
        User user(name, username, password, role, "");
        usersByPhone[username] = user;  // Store by username
        loadedCount++;
    }
    
    file.close();
}

void UserManager::loadStudentUsers() {
    ifstream file(studentFile);
    if (!file.is_open()) {
        cout << "Note: No student file found. Starting fresh.\n";
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    
    int loadedCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string regNumber, name, password, phone;
        
        // Format: regNumber,name,password,phone
        getline(ss, regNumber, ',');
        getline(ss, name, ',');
        getline(ss, password, ',');
        getline(ss, phone, ',');
        
        User user(name, phone, password, STUDENT, regNumber);
        usersByPhone[phone] = user;           // Store by phone
        usersByRegNumber[regNumber] = user;   // Also store by reg number for login
        loadedCount++;
    }
    
    file.close();
    if (loadedCount > 0) {
    }
}

void UserManager::saveStudentUsers() {
    ofstream file(studentFile);
    if (!file.is_open()) {
        cout << "Error: Could not save student users.\n";
        return;
    }
    
    // Header: regNumber,name,password,phone
    file << "regNumber,name,password,phone\n";
    
    // Save only STUDENT users
    for (const auto& pair : usersByPhone) {
        const User& user = pair.second;
        if (user.getRole() == STUDENT) {
            file << user.getRegNumber() << ","
                 << user.getName() << ","
                 << user.getPassword() << ","
                 << user.getPhone() << "\n";
        }
    }
    
    file.close();
}

void UserManager::saveAdminUsers() {
    ofstream file(adminFile);
    if (!file.is_open()) {
        cout << "Error: Could not save admin users.\n";
        return;
    }
    
    // Header: username,name,password,role
    file << "username,name,password,role\n";
    
    // Save only NON-STUDENT users
    for (const auto& pair : usersByPhone) {
        const User& user = pair.second;
        if (user.getRole() != STUDENT) {
            string roleStr;
            switch(user.getRole()) {
                case KITCHEN: roleStr = "kitchen"; break;
                case DELIVERY: roleStr = "delivery"; break;
                case ADMIN: roleStr = "admin"; break;
                default: roleStr = "admin";
            }
            
            file << user.getPhone() << ","  // Phone field contains username for staff
                 << user.getName() << ","
                 << user.getPassword() << ","
                 << roleStr << "\n";
        }
    }
    
    file.close();
}

bool UserManager::registerStudent(const User& user) {
    if (phoneExists(user.getPhone()) || regNumberExists(user.getRegNumber())) {
        return false;
    }
    
    usersByPhone[user.getPhone()] = user;
    usersByRegNumber[user.getRegNumber()] = user;
    saveStudentUsers();
    return true;
}

bool UserManager::createStaffUser(const User& user) {
    if (phoneExists(user.getPhone())) {
        return false;
    }
    
    usersByPhone[user.getPhone()] = user;
    saveAdminUsers();
    return true;
}

bool UserManager::loginStudent(const string& regNumber, const string& password, User& loggedInUser) {
    auto it = usersByRegNumber.find(regNumber);
    
    if (it == usersByRegNumber.end()) {
        return false;  // Student not found
    }
    
    if (it->second.getPassword() == password) {
        loggedInUser = it->second;
        return true;
    }
    
    return false;  // Wrong password
}

bool UserManager::loginStaff(const string& username, const string& password, User& loggedInUser) {
    auto it = usersByPhone.find(username);
    
    if (it == usersByPhone.end()) {
        return false;  // Staff not found
    }
    
    if (it->second.getRole() == STUDENT) {
        return false;
    }
    
    if (it->second.getPassword() == password) {
        loggedInUser = it->second;
        return true;
    }
    
    return false;  // Wrong password
}

bool UserManager::phoneExists(const string& phone) {
    return usersByPhone.find(phone) != usersByPhone.end();
}

bool UserManager::regNumberExists(const string& regNumber) {
    return usersByRegNumber.find(regNumber) != usersByRegNumber.end();
}

void UserManager::displayAllUsers() const {
    if (usersByPhone.empty()) {
        cout << "\nNo users registered yet.\n";
        return;
    }
    
    cout << "\n========== All Registered Users ==========\n";
    for (const auto& pair : usersByPhone) {
        pair.second.display();
        cout << "----------------------------------------\n";
    }
    cout << "Total Users: " << usersByPhone.size() << endl;
}