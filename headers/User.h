#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

// User roles
enum UserRole {
    STUDENT,
    KITCHEN,
    DELIVERY,
    ADMIN
};

// User class
class User {
private:
    string name;
    string phone;          // For students: phone number, For staff: username
    string password;
    UserRole role;
    string regNumber;      // Only for students

public:
    // Constructors
    User();
    User(string name, string phone, string password, UserRole role, string regNumber = "");
    
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getPassword() const { return password; }
    UserRole getRole() const { return role; }
    string getRegNumber() const { return regNumber; }
    
    string getRoleString() const;
    
    void display() const;
};

// UserManager class - handles all user operations
class UserManager {
private:
    unordered_map<string, User> usersByPhone;      // phone/username - User
    unordered_map<string, User> usersByRegNumber;  // regNumber - User (for students)
    string studentFile;
    string adminFile;
    
public:
    UserManager(string studentFile = "data/users.txt", string adminFile = "data/admin_users.txt");
    
    // Load users from both files
    void loadUsers();
    
    // Load from student file
    void loadStudentUsers();
    
    // Load from admin file
    void loadAdminUsers();
    
    // Save student users
    void saveStudentUsers();
    
    // Save admin/staff users
    void saveAdminUsers();
    
    // Register new STUDENT user
    bool registerStudent(const User& user);
    
    // Create staff user (admin only)
    bool createStaffUser(const User& user);
    
    // Login for students (using reg number)
    bool loginStudent(const string& regNumber, const string& password, User& loggedInUser);
    
    // Login for staff (using username)
    bool loginStaff(const string& username, const string& password, User& loggedInUser);
    
    // Check if phone already exists
    bool phoneExists(const string& phone);
    
    // Check if reg number exists
    bool regNumberExists(const string& regNumber);
    
    // Display all users (admin only)
    void displayAllUsers() const;
    
    // Get user count
    int getUserCount() const { return usersByPhone.size(); }
};

#endif