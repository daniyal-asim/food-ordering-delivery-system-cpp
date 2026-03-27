#include "../headers/MenuItem.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// MenuItem implementation
MenuItem::MenuItem() {
    itemID = 0;
    name = "";
    price = 0.0;
    category = "";
    available = true;
}

MenuItem::MenuItem(int id, string name, double price, string category, bool available) {
    this->itemID = id;
    this->name = name;
    this->price = price;
    this->category = category;
    this->available = available;
}

void MenuItem::display() const {
    cout << "\n--- Menu Item ---\n";
    cout << "ID: " << itemID << endl;
    cout << "Name: " << name << endl;
    cout << "Price: Rs. " << fixed << setprecision(0) << price << endl;
    cout << "Category: " << category << endl;
    cout << "Status: " << (available ? "Available" : "Not Available") << endl;
}

void MenuItem::displayShort() const {
    cout << left << setw(5) << itemID 
         << setw(25) << name 
         << "Rs." << right << setw(6) << fixed << setprecision(0) << price
         << "  " << left << setw(12) << category
         << (available ? "Available" : "Not Available") << endl;
}

// MenuManager implementation with ARRAYS
MenuManager::MenuManager(string filename) {
    this->filename = filename;
    this->itemCount = 0;
    this->nextItemID = 1;
    loadMenu();
}

void MenuManager::loadMenu() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Note: No menu file found. Creating sample menu...\n";
        
        // Create sample menu items
        addItem(MenuItem(1, "Chicken Burger", 250, "meal", true));
        addItem(MenuItem(2, "Zinger Burger", 300, "meal", true));
        addItem(MenuItem(3, "Beef Burger", 280, "meal", true));
        addItem(MenuItem(4, "Fries (Regular)", 100, "snack", true));
        addItem(MenuItem(5, "Fries (Large)", 150, "snack", true));
        addItem(MenuItem(6, "Samosa", 20, "snack", true));
        addItem(MenuItem(7, "Roll", 80, "snack", true));
        addItem(MenuItem(8, "Cold Drink", 50, "drink", true));
        addItem(MenuItem(9, "Tea", 30, "drink", true));
        addItem(MenuItem(10, "Coffee", 60, "drink", true));
        
        saveMenu();
        cout << "Sample menu created with 10 items.\n";
        return;
    }
    
    string line;
    getline(file, line); 
    
    itemCount = 0;
    while (getline(file, line) && itemCount < 100) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string idStr, name, priceStr, category, availStr;
        
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, category, ',');
        getline(ss, availStr, ',');
        
        int id = stoi(idStr);
        double price = stod(priceStr);
        bool available = (availStr == "1");
        
        menuItems[itemCount] = MenuItem(id, name, price, category, available);
        itemCount++;
        
        if (id >= nextItemID) {
            nextItemID = id + 1;
        }
    }
    
    file.close();
    cout << "Loaded " << itemCount << " menu items.\n";
}

void MenuManager::saveMenu() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not save menu.\n";
        return;
    }
    
    file << "itemID,name,price,category,available\n";
    
    for (int i = 0; i < itemCount; i++) {
        file << menuItems[i].getItemID() << ","
             << menuItems[i].getName() << ","
             << menuItems[i].getPrice() << ","
             << menuItems[i].getCategory() << ","
             << (menuItems[i].isAvailable() ? "1" : "0") << "\n";
    }
    
    file.close();
}

void MenuManager::addItem(const MenuItem& item) {
    if (itemCount >= 100) {
        cout << "Error: Menu is full! Cannot add more items.\n";
        return;
    }
    
    menuItems[itemCount] = item;  // Array
    itemCount++;
    saveMenu();
}

bool MenuManager::removeItem(int itemID) {
    // Linear search
    for (int i = 0; i < itemCount; i++) {
        if (menuItems[i].getItemID() == itemID) {
            // Shift elements left
            for (int j = i; j < itemCount - 1; j++) {
                menuItems[j] = menuItems[j + 1];
            }
            itemCount--;
            saveMenu();
            return true;
        }
    }
    return false;
}

MenuItem* MenuManager::findItem(int itemID) {
    // Linear search
    for (int i = 0; i < itemCount; i++) {
        if (menuItems[i].getItemID() == itemID) {
            return &menuItems[i];
        }
    }
    return nullptr;
}

void MenuManager::displayAllItems() const {
    if (itemCount == 0) {
        cout << "\nNo items in menu.\n";
        return;
    }
    
    cout << "\n========== RAJU FOOD MENU ==========\n";
    cout << left << setw(5) << "ID" 
         << setw(25) << "Item Name" 
         << setw(12) << "Price"
         << setw(14) << "Category"
         << "Status\n";
    cout << "--------------------------------------------------------------------\n";
    
    for (int i = 0; i < itemCount; i++) {
        if (menuItems[i].isAvailable()) {
            menuItems[i].displayShort();
        }
    }
    cout << "====================================================================\n";
}

void MenuManager::displayByCategory(const string& category) const {
    cout << "\n========== " << category << " ITEMS ==========\n";
    cout << left << setw(5) << "ID" 
         << setw(25) << "Item Name" 
         << setw(12) << "Price" << endl;
    cout << "--------------------------------------------\n";
    
    bool found = false;
    for (int i = 0; i < itemCount; i++) {
        if (menuItems[i].getCategory() == category && menuItems[i].isAvailable()) {
            menuItems[i].displayShort();
            found = true;
        }
    }
    
    if (!found) {
        cout << "No items found in this category.\n";
    }
    cout << "============================================\n";
}

void MenuManager::searchByName(const string& searchTerm) const {
    cout << "\n========== SEARCH RESULTS ==========\n";
    
    bool found = false;
    string lowerSearch = searchTerm;
    transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
    
    for (int i = 0; i < itemCount; i++) {
        string lowerName = menuItems[i].getName();
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        if (lowerName.find(lowerSearch) != string::npos) {
            menuItems[i].displayShort();
            found = true;
        }
    }
    
    if (!found) {
        cout << "No items found matching '" << searchTerm << "'\n";
    }
    cout << "====================================\n";
}

void MenuManager::sortByPrice() {
    // Bubble Sort
    for (int i = 0; i < itemCount - 1; i++) {
        for (int j = 0; j < itemCount - i - 1; j++) {
            if (menuItems[j].getPrice() > menuItems[j + 1].getPrice()) {
                // Swap
                MenuItem temp = menuItems[j];
                menuItems[j] = menuItems[j + 1];
                menuItems[j + 1] = temp;
            }
        }
    }
    cout << "Menu sorted by price (low to high).\n";
}

void MenuManager::sortByName() {
    // Bubble Sort
    for (int i = 0; i < itemCount - 1; i++) {
        for (int j = 0; j < itemCount - i - 1; j++) {
            if (menuItems[j].getName() > menuItems[j + 1].getName()) {
                // Swap
                MenuItem temp = menuItems[j];
                menuItems[j] = menuItems[j + 1];
                menuItems[j + 1] = temp;
            }
        }
    }
    cout << "Menu sorted by name (A-Z).\n";
}

