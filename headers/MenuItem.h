#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

// MenuItem class - represents a food item
class MenuItem {
private:
    int itemID;
    string name;
    double price;
    string category;  // meal, snack, drink
    bool available;

public:
    // Constructors
    MenuItem();
    MenuItem(int id, string name, double price, string category, bool available = true);
    
    // Getters
    int getItemID() const { return itemID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }
    bool isAvailable() const { return available; }
    
    // Setters
    void setPrice(double newPrice) { price = newPrice; }
    void setAvailable(bool status) { available = status; }
    
    // Display
    void display() const;
    void displayShort() const;
};

// MenuManager class - handles menu operations with ARRAYS
class MenuManager {
private:
    MenuItem menuItems[100];  // array 
    int itemCount;            // Current number of items
    string filename;
    int nextItemID;

public:
    MenuManager(string filename = "data/menu.txt");
    
    // Load/Save
    void loadMenu();
    void saveMenu();
    
    // Menu operations
    void addItem(const MenuItem& item);
    bool removeItem(int itemID);
    MenuItem* findItem(int itemID);
    
    // Display options
    void displayAllItems() const;
    void displayByCategory(const string& category) const;
    void searchByName(const string& searchTerm) const;
    
    // Sorting (Bubble Sort)
    void sortByPrice();
    void sortByName();
       
    // Get count
    int getItemCount() const { return itemCount; }
    int getNextID() const { return nextItemID; }
};

#endif