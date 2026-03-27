#ifndef CART_H
#define CART_H

#include <string>
#include <stack>
#include "MenuItem.h"
using namespace std;

// CartItem - item in the cart with quantity
struct CartItem {
    MenuItem item;
    int quantity;
    
    CartItem() : quantity(0) {}
    CartItem(MenuItem item, int qty = 1) : item(item), quantity(qty) {}
    
    double getSubtotal() const {
        return item.getPrice() * quantity;
    }
};

// Cart class - shopping cart with undo using ARRAYS
class Cart {
private:
    CartItem items[50];          //  array
    int itemCount;               // Current number of items
    stack<string> actionHistory; // Stack for undo

public:
    Cart();
    
    // Cart operations
    void addItem(const MenuItem& item, int quantity = 1);
    bool removeItem(int itemID);
    bool updateQuantity(int itemID, int newQuantity);
    void clearCart();
    
    // Undo feature 
    bool undo();
    
    // Display
    void displayCart() const;
    
    // Calculations
    double getTotal() const;
    int getItemCount() const { return itemCount; }
    bool isEmpty() const { return itemCount == 0; }
    
    // Get items (for order placement)
    CartItem* getItems() { return items; }
};

#endif
