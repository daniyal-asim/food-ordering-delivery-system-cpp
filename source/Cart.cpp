#include "../headers/Cart.h"
#include "../headers/Utils.h"
#include <iostream>
#include <iomanip>

Cart::Cart() {
    itemCount = 0;
}

void Cart::addItem(const MenuItem& item, int quantity) {
    // Check if item already in cart
    for (int i = 0; i < itemCount; i++) {
        if (items[i].item.getItemID() == item.getItemID()) {
            items[i].quantity += quantity;
            
            // stack for undo
            actionHistory.push("ADD:" + to_string(item.getItemID()) + ":" + to_string(quantity));
            
            cout << "Updated quantity of " << item.getName() << " in cart.\n";
            return;
        }
    }
    
    // Add new item
    if (itemCount >= 50) {
        cout << "Error: Cart is full!\n";
        return;
    }
    
    items[itemCount] = CartItem(item, quantity);
    itemCount++;
    
    // stack for undo
    actionHistory.push("ADD:" + to_string(item.getItemID()) + ":" + to_string(quantity));
    
    cout << "Added " << item.getName() << " to cart.\n";
}

bool Cart::removeItem(int itemID) {
    // Linear search 
    for (int i = 0; i < itemCount; i++) {
        if (items[i].item.getItemID() == itemID) {
            int removedQty = items[i].quantity;
            string itemName = items[i].item.getName();
            double itemPrice = items[i].item.getPrice();
            
            // Shift elements left
            for (int j = i; j < itemCount - 1; j++) {
                items[j] = items[j + 1];
            }
            itemCount--;
            
            // stack for undo
            actionHistory.push("REMOVE:" + to_string(itemID) + ":" + to_string(removedQty) + ":" 
                             + itemName + ":" + to_string(itemPrice));
            
            cout << "Removed " << itemName << " from cart.\n";
            return true;
        }
    }
    return false;
}

bool Cart::updateQuantity(int itemID, int newQuantity) {
    if (newQuantity <= 0) {
        return removeItem(itemID);
    }
    
    for (int i = 0; i < itemCount; i++) {
        if (items[i].item.getItemID() == itemID) {
            int oldQty = items[i].quantity;
            items[i].quantity = newQuantity;
            
            // to stack for undo
            actionHistory.push("UPDATE:" + to_string(itemID) + ":" + to_string(oldQty) + ":" + to_string(newQuantity));
            
            cout << "Updated quantity.\n";
            return true;
        }
    }
    return false;
}

void Cart::clearCart() {
    itemCount = 0;
    cout << "Cart cleared.\n";
}

bool Cart::undo() {
    if (actionHistory.empty()) {
        cout << "No actions to undo.\n";
        return false;
    }
    
    string lastAction = actionHistory.top();  // Stack top
    actionHistory.pop();  // Stack pop 
    
    // Parse the action
    size_t pos1 = lastAction.find(':');
    size_t pos2 = lastAction.find(':', pos1 + 1);
    
    string action = lastAction.substr(0, pos1);
    
    if (action == "ADD") {
        // Undo ADD: remove the quantity that was added
        int itemID = stoi(lastAction.substr(pos1 + 1, pos2 - pos1 - 1));
        int quantity = stoi(lastAction.substr(pos2 + 1));
        
        for (int i = 0; i < itemCount; i++) {
            if (items[i].item.getItemID() == itemID) {
                items[i].quantity -= quantity;
                if (items[i].quantity <= 0) {
                    // Remove item
                    for (int j = i; j < itemCount - 1; j++) {
                        items[j] = items[j + 1];
                    }
                    itemCount--;
                }
                break;
            }
        }
        
        cout << "Undo: Removed " << quantity << " item(s) from cart.\n";
        return true;
        
    } else if (action == "REMOVE") {
        // Undo REMOVE: add the item back
        size_t pos3 = lastAction.find(':', pos2 + 1);
        size_t pos4 = lastAction.find(':', pos3 + 1);
        
        int itemID = stoi(lastAction.substr(pos1 + 1, pos2 - pos1 - 1));
        int quantity = stoi(lastAction.substr(pos2 + 1, pos3 - pos2 - 1));
        string itemName = lastAction.substr(pos3 + 1, pos4 - pos3 - 1);
        double itemPrice = stod(lastAction.substr(pos4 + 1));
        
        // Reconstruct the item
        if (itemCount < 50) {
            MenuItem tempItem(itemID, itemName, itemPrice, "meal", true);
            items[itemCount] = CartItem(tempItem, quantity);
            itemCount++;
            
            cout << "Undo: Restored " << itemName << " to cart.\n";
            return true;
        }
        
    } else if (action == "UPDATE") {
        // Undo UPDATE: restore old quantity
        size_t pos3 = lastAction.find(':', pos2 + 1);
        
        int itemID = stoi(lastAction.substr(pos1 + 1, pos2 - pos1 - 1));
        int oldQty = stoi(lastAction.substr(pos2 + 1, pos3 - pos2 - 1));
        
        for (int i = 0; i < itemCount; i++) {
            if (items[i].item.getItemID() == itemID) {
                items[i].quantity = oldQty;
                break;
            }
        }
        
        cout << "Undo: Restored previous quantity.\n";
        return true;
    }
    
    return false;
}

void Cart::displayCart() const {
    if (itemCount == 0) {
        cout << "\nYour cart is empty.\n";
        return;
    }
    
    cout << "\n========== YOUR CART ==========\n";
    cout << left << setw(5) << "ID" 
         << setw(22) << "Item" 
         << right << setw(10) << "Price"
         << setw(8) << "Qty"
         << setw(12) << "Subtotal" << endl;
    cout << "-----------------------------------------------------------\n";
    
    for (int i = 0; i < itemCount; i++) {
        cout << left << setw(5) << items[i].item.getItemID()
             << setw(22) << items[i].item.getName()
             << "Rs. " << right << setw(5) << fixed << setprecision(0) << items[i].item.getPrice()
             << right << setw(8) << items[i].quantity
             << " Rs. " << right << setw(6) << fixed << setprecision(0) << items[i].getSubtotal() << endl;
    }
    
    cout << "===========================================================\n";
    cout << right << setw(42) << "TOTAL: Rs. " << fixed << setprecision(0) << getTotal() << endl;
    cout << "===========================================================\n";
}

double Cart::getTotal() const {
    double total = 0;
    for (int i = 0; i < itemCount; i++) {
        total += items[i].getSubtotal();
    }
    return total;
}
