#include <iostream>
#include <iomanip>
#include "headers/User.h"
#include "headers/Utils.h"
#include "headers/MenuItem.h"
#include "headers/Cart.h"
#include "headers/Order.h"
#include "headers/Graph.h"
using namespace std;

// Function prototypes
void showMainMenu();
void registerNewStudent(UserManager& userManager);
void loginUser(UserManager& userManager, OrderManager& orderManager, CampusGraph& campusGraph);
void studentMenu(const User& user, OrderManager& orderManager);
void kitchenMenu(const User& user, OrderManager& orderManager);
void deliveryMenu(const User& user, OrderManager& orderManager, CampusGraph& campusGraph);
void adminMenu(const User& user, UserManager& userManager, OrderManager& orderManager, CampusGraph& campusGraph);
void createStaffAccount(UserManager& userManager);
void viewMenu(MenuManager& menuManager);
void browseByCategory(MenuManager& menuManager);
void searchMenu(MenuManager& menuManager);
void manageCart(MenuManager& menuManager, Cart& cart, const User& user, OrderManager& orderManager);
void manageMenuItems(MenuManager& menuManager);
void placeOrderFromCart(Cart& cart, const User& user, OrderManager& orderManager);
void viewOrderHistory(const User& user, OrderManager& orderManager);
void viewPendingOrders(OrderManager& orderManager);
void updateOrderStatus(OrderManager& orderManager);
void viewReadyOrders(OrderManager& orderManager);
void acceptDelivery(const User& user, OrderManager& orderManager);
void viewMyDeliveries(const User& user, OrderManager& orderManager);
void markDelivered(OrderManager& orderManager);
void viewCampusMap(CampusGraph& campusGraph);

int main() {
    UserManager userManager;      // User management
    OrderManager orderManager;    // Order management
    CampusGraph campusGraph;      // Graph Map
    campusGraph.initializeGIKICampus();  // Initialize campus map
    
    cout << "\n========================================\n";
    cout << "   RAJU Food Ordering System - GIKI\n";
    cout << "========================================\n";
    Utils::pause();
    
    int choice;
    
    while (true) {
        showMainMenu();
        choice = Utils::getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                registerNewStudent(userManager);
                break;
            case 2:
                loginUser(userManager, orderManager, campusGraph); 
                break;
            case 3:
                cout << "\nThank you for using Raju Food System!\n";
                return 0;
            default:
                cout << "\nInvalid choice! Please try again.\n";
                Utils::pause();
        }
    }
    
    return 0;
}

void showMainMenu() {
    Utils::clearScreen();
    cout << "\n========== RAJU FOOD SYSTEM - MAIN MENU ==========\n";
    cout << "1. Register (Students Only)\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "==================================================\n";
    cout << "\nNote: Staff credentials stored in data/admin_users.txt\n";
}

void registerNewStudent(UserManager& userManager) {
    Utils::clearScreen();
    cout << "\n========== STUDENT REGISTRATION ==========\n";
    cout << "Note: Only students can register here.\n";
    cout << "Staff accounts managed by administration.\n";
    cout << "==========================================\n\n";
    
    string name, phone, password, regNumber;
    
    cout << "Enter Full Name: ";
    getline(cin, name);
    
    cout << "Enter Registration Number (7 digits, e.g., 2024001): ";
    getline(cin, regNumber);
    
    if (!Utils::isValidRegNumber(regNumber)) {
        cout << "\nError: Invalid registration number format!\n";
        cout << "Reg number must be exactly 7 digits.\n";
        Utils::pause();
        return;
    }
    
    if (userManager.regNumberExists(regNumber)) {
        cout << "\nError: This registration number is already registered!\n";
        Utils::pause();
        return;
    }
    
    cout << "Enter Phone Number (11 digits, e.g., 03001234567): ";
    getline(cin, phone);
    
    if (!Utils::isValidPhone(phone)) {
        cout << "\nError: Invalid phone number format!\n";
        cout << "Phone must be 11 digits starting with 03.\n";
        Utils::pause();
        return;
    }
    
    if (userManager.phoneExists(phone)) {
        cout << "\nError: This phone number is already registered!\n";
        Utils::pause();
        return;
    }
    
    cout << "Enter Password: ";
    getline(cin, password);
    
    if (password.length() < 3) {
        cout << "\nError: Password must be at least 3 characters!\n";
        Utils::pause();
        return;
    }
    
    User newUser(name, phone, password, STUDENT, regNumber);
    
    if (userManager.registerStudent(newUser)) {
        cout << "\nRegistration Successful!\n";
        newUser.display();
        cout << "\nYou can login using your Registration Number: " << regNumber << "\n";
    } else {
        cout << "\nError: Registration failed!\n";
    }
    
    Utils::pause();
}

void loginUser(UserManager& userManager, OrderManager& orderManager, CampusGraph& campusGraph) {

    Utils::clearScreen();
    cout << "\n========== USER LOGIN ==========\n";
    cout << "1. Student Login\n";
    cout << "2. Staff Login\n";
    cout << "3. Back\n";
    cout << "================================\n";
    
    int choice = Utils::getIntInput("Enter choice: ");
    
    User loggedInUser;
    
    if (choice == 1) {
        Utils::clearScreen();
        cout << "\n========== STUDENT LOGIN ==========\n";
        
        string regNumber, password;
        
        cout << "Enter Registration Number (7 digits): ";
        getline(cin, regNumber);
        
        cout << "Enter Password: ";
        getline(cin, password);
        
        if (userManager.loginStudent(regNumber, password, loggedInUser)) {
            cout << "\nLogin Successful!\n";
            cout << "Welcome, " << loggedInUser.getName() << "!\n";
            Utils::pause();
            studentMenu(loggedInUser, orderManager);
        } else {
            cout << "\nLogin Failed! Invalid registration number or password.\n";
            Utils::pause();
        }
        
    } else if (choice == 2) {
        Utils::clearScreen();
        cout << "\n========== STAFF LOGIN ==========\n";
        
        string username, password;
        
        cout << "Enter Username: ";
        getline(cin, username);
        
        cout << "Enter Password: ";
        getline(cin, password);
        
        if (userManager.loginStaff(username, password, loggedInUser)) {
            cout << "\nLogin Successful!\n";
            cout << "Welcome, " << loggedInUser.getName() << "!\n";
            Utils::pause();
            
            switch (loggedInUser.getRole()) {
                case KITCHEN:
                    kitchenMenu(loggedInUser, orderManager);
                    break;
                case DELIVERY:
                    deliveryMenu(loggedInUser, orderManager, campusGraph);
                    break;
                case ADMIN:
                    adminMenu(loggedInUser, userManager, orderManager, campusGraph);

                    break;
                default:
                    break;
            }
        } else {
            cout << "\nLogin Failed! Invalid username or password.\n";
            Utils::pause();
        }
        
    } else if (choice == 3) {
        return;
    } else {
        cout << "\nInvalid choice!\n";
        Utils::pause();
    }
}

void studentMenu(const User& user, OrderManager& orderManager) {
    MenuManager menuManager;
    Cart cart;
    
    while (true) {
        Utils::clearScreen();
        cout << "\n========== STUDENT MENU ==========\n";
        cout << "Welcome, " << user.getName() << "!\n";
        cout << "Reg #: " << user.getRegNumber() << "\n";
        cout << "Cart Items: " << cart.getItemCount() << " | Total: Rs." << fixed << setprecision(0) << cart.getTotal() << "\n";
        cout << "==================================\n";
        cout << "1. View Full Menu\n";
        cout << "2. Browse by Category\n";
        cout << "3. Search Menu\n";
        cout << "4. My Cart\n";
        cout << "5. Place Order\n";
        cout << "6. Order History\n";
        cout << "7. Logout\n";
        cout << "==================================\n";
        
        int choice = Utils::getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                viewMenu(menuManager);
                break;
            case 2:
                browseByCategory(menuManager);
                break;
            case 3:
                searchMenu(menuManager);
                break;
            case 4:
                manageCart(menuManager, cart, user, orderManager);
                break;
            case 5:
                placeOrderFromCart(cart, user, orderManager);
                break;
            case 6:
                viewOrderHistory(user, orderManager);
                break;
            case 7:
                cout << "\nLogging out...\n";
                Utils::pause();
                return;
            default:
                cout << "\nInvalid choice!\n";
                Utils::pause();
        }
    }
}

void kitchenMenu(const User& user, OrderManager& orderManager) {
    MenuManager menuManager;
    
    while (true) {
        Utils::clearScreen();
        cout << "\n========== KITCHEN STAFF MENU ==========\n";
        cout << "Welcome, " << user.getName() << "!\n";
        cout << "========================================\n";
        cout << "1. View Pending Orders\n";
        cout << "2. Update Order Status\n";
        cout << "3. Manage Menu Items\n";
        cout << "4. Logout\n";
        cout << "========================================\n";
        
        int choice = Utils::getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                viewPendingOrders(orderManager);
                break;
            case 2:
                updateOrderStatus(orderManager);
                break;
            case 3:
                manageMenuItems(menuManager);
                break;
            case 4:
                cout << "\nLogging out...\n";
                Utils::pause();
                return;
            default:
                cout << "\nInvalid choice!\n";
                Utils::pause();
        }
    }
}

void deliveryMenu(const User& user, OrderManager& orderManager, CampusGraph& campusGraph) {
    while (true) {
        Utils::clearScreen();
        cout << "\n========== DELIVERY PERSONNEL MENU ==========\n";
        cout << "Welcome, " << user.getName() << "!\n";
        cout << "============================================\n";
        cout << "1. View Ready Orders\n";
        cout << "2. Accept Delivery\n";
        cout << "3. My Active Deliveries\n";
        cout << "4. Mark as Delivered\n";
        cout << "5. View Campus Map\n";
        cout << "6. Logout\n";
        cout << "============================================\n";
        
        int choice = Utils::getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                viewReadyOrders(orderManager);
                break;
            case 2:
                acceptDelivery(user, orderManager);
                break;
            case 3:
                viewMyDeliveries(user, orderManager);
                break;
            case 4:
                markDelivered(orderManager);
                break;
            case 5:
                viewCampusMap(campusGraph);
                break;
            case 6:
                cout << "\nLogging out...\n";
                Utils::pause();
                return;
            default:
                cout << "\nInvalid choice!\n";
                Utils::pause();
        }
    }
}

void createStaffAccount(UserManager& userManager) {
    while (true) {
        Utils::clearScreen();
        cout << "\n========== CREATE STAFF ACCOUNT ==========\n";
        cout << "New staff will be saved to admin_users.txt\n";
        cout << "==========================================\n\n";
        
        string name, phone, password;
        
        cout << "Enter Full Name: ";
        getline(cin, name);
        
        cout << "Enter Username (e.g., kitchen2, delivery2): ";
        getline(cin, phone);
        
        if (userManager.phoneExists(phone)) {
            cout << "\nError: Username '" << phone << "' is already taken!\n";
            cout << "\nWhat would you like to do?\n";
            cout << "1. Try a different username\n";
            cout << "2. Cancel and go back\n";
            
            int retryChoice = Utils::getIntInput("Enter choice: ");
            
            if (retryChoice == 1) {
                continue;
            } else {
                return;
            }
        }
        
        cout << "Enter Password: ";
        getline(cin, password);
        
        cout << "\nSelect Role:\n";
        cout << "1. Kitchen Staff\n";
        cout << "2. Delivery Personnel\n";
        cout << "3. Admin\n";
        
        int roleChoice = Utils::getIntInput("Enter choice: ");
        
        UserRole role;
        switch (roleChoice) {
            case 1: role = KITCHEN; break;
            case 2: role = DELIVERY; break;
            case 3: role = ADMIN; break;
            default:
                cout << "\nInvalid role selection!\n";
                Utils::pause();
                continue;
        }
        
        User newStaff(name, phone, password, role, "");
        
        if (userManager.createStaffUser(newStaff)) {
            cout << "\nStaff Account Created Successfully!\n";
            cout << "Saved to data/admin_users.txt\n";
            newStaff.display();
            Utils::pause();
            return;
        } else {
            cout << "\nError: Failed to create account!\n";
            Utils::pause();
            return;
        }
    }
}

void adminMenu(const User& user, UserManager& userManager, OrderManager& orderManager, CampusGraph& campusGraph) {
    while (true) {
        Utils::clearScreen();
        cout << "\n========== ADMIN MENU ==========\n";
        cout << "Welcome, " << user.getName() << "!\n";
        cout << "================================\n";
        cout << "1. View All Users\n";
        cout << "2. Create Staff Account\n";
        cout << "3. View All Orders\n";
        cout << "4. View Campus Map\n"; 
        cout << "5. Logout\n";
        cout << "================================\n";
        
        int choice = Utils::getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                Utils::clearScreen();
                userManager.displayAllUsers();
                Utils::pause();
                break;
            case 2:
                createStaffAccount(userManager);
                break;
            case 3:
                Utils::clearScreen();
                orderManager.displayAllOrders();
                Utils::pause();
                break;
            case 4:
                viewCampusMap(campusGraph);  
                break;
            case 5:
                cout << "\nLogging out...\n";
                Utils::pause();
                return;
            default:
                cout << "\nInvalid choice! Please enter a number between 1-5.\n";
                Utils::pause();
                break;
        }
    }
}
void viewMenu(MenuManager& menuManager) {
    while (true) {
        Utils::clearScreen();
        menuManager.displayAllItems();
        
        cout << "\nOptions:\n";
        cout << "1. Sort by Price (Low to High)\n";
        cout << "2. Sort by Name (A-Z)\n";
        cout << "3. Back to Main Menu\n";
        
        int choice = Utils::getIntInput("Enter choice: ");
        
        if (choice == 1) {
            menuManager.sortByPrice();
        } else if (choice == 2) {
            menuManager.sortByName();
        } else if (choice == 3) {
            break;
        } else {
            cout << "\nInvalid choice!\n";
            Utils::pause();
        }
    }
}

void browseByCategory(MenuManager& menuManager) {
    while (true) {
        Utils::clearScreen();
        cout << "\n========== BROWSE BY CATEGORY ==========\n";
        cout << "1. Meals\n";
        cout << "2. Snacks\n";
        cout << "3. Drinks\n";
        cout << "4. Back to Main Menu\n";
        cout << "========================================\n";
        
        int choice = Utils::getIntInput("Enter choice: ");
        
        if (choice == 1) {
            Utils::clearScreen();
            menuManager.displayByCategory("meal");
            Utils::pause();
        } else if (choice == 2) {
            Utils::clearScreen();
            menuManager.displayByCategory("snack");
            Utils::pause();
        } else if (choice == 3) {
            Utils::clearScreen();
            menuManager.displayByCategory("drink");
            Utils::pause();
        } else if (choice == 4) {
            break;
        } else {
            cout << "\nInvalid choice!\n";
            Utils::pause();
        }
    }
}

void searchMenu(MenuManager& menuManager) {
    Utils::clearScreen();
    cout << "\n========== SEARCH MENU ==========\n";
    
    string searchTerm;
    cout << "Enter item name to search: ";
    getline(cin, searchTerm);
    
    menuManager.searchByName(searchTerm);
    Utils::pause();
}

void manageCart(MenuManager& menuManager, Cart& cart, const User& user, OrderManager& orderManager) {
    while (true) {
        Utils::clearScreen();
        cart.displayCart();
        
        cout << "\n========== CART OPTIONS ==========\n";
        cout << "1. Add Item to Cart\n";
        cout << "2. Remove Item from Cart\n";
        cout << "3. Update Quantity\n";
        cout << "4. Undo Last Action\n";
        cout << "5. Clear Cart\n";
        cout << "6. Proceed to Checkout\n";
        cout << "7. Back to Menu\n";
        cout << "==================================\n";
        
        int choice = Utils::getIntInput("Enter choice: ");
        
        if (choice == 1) {
            Utils::clearScreen();
            menuManager.displayAllItems();
            
            int itemID = Utils::getIntInput("\nEnter Item ID (0 to cancel): ");
            
            if (itemID == 0) {
                continue;
            }
            
            int quantity = Utils::getIntInput("Enter Quantity: ");
            
            if (quantity <= 0) {
                cout << "Quantity must be greater than 0.\n";
                Utils::pause();
                continue;
            }
            
            MenuItem* item = menuManager.findItem(itemID);
            if (item != nullptr && item->isAvailable()) {
                cart.addItem(*item, quantity);
            } else {
                cout << "Item not found or not available.\n";
            }
            Utils::pause();
            
        } else if (choice == 2) {
            int itemID = Utils::getIntInput("Enter Item ID to remove (0 to cancel): ");
            
            if (itemID == 0) {
                continue;
            }
            
            if (!cart.removeItem(itemID)) {
                cout << "Item not in cart.\n";
            }
            Utils::pause();
            
        } else if (choice == 3) {
            int itemID = Utils::getIntInput("Enter Item ID (0 to cancel): ");
            
            if (itemID == 0) {
                continue;
            }
            
            int newQty = Utils::getIntInput("Enter new quantity: ");
            
            if (!cart.updateQuantity(itemID, newQty)) {
                cout << "Item not in cart.\n";
            }
            Utils::pause();
            
        } else if (choice == 4) {
            cart.undo();
            Utils::pause();
            
        } else if (choice == 5) {
            if (cart.isEmpty()) {
                cout << "Cart is already empty.\n";
                Utils::pause();
                continue;
            }
            
            cin.ignore();
            cout << "Are you sure you want to clear the cart? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            
            if (confirm == 'y' || confirm == 'Y') {
                cart.clearCart();
            } else {
                cout << "Cancelled.\n";
            }
            Utils::pause();
            
        } else if (choice == 6) {
            placeOrderFromCart(cart, user, orderManager);
            
        } else if (choice == 7) {
            break;
        } else {
            cout << "\nInvalid choice!\n";
            Utils::pause();
        }
    }
}

void placeOrderFromCart(Cart& cart, const User& user, OrderManager& orderManager) {
    if (cart.isEmpty()) {
        cout << "\nYour cart is empty! Add items first.\n";
        Utils::pause();
        return;
    }
    
    Utils::clearScreen();
    cart.displayCart();
    
    cout << "\n========== ORDER DETAILS ==========\n";
    
    string hostel, room;
    
    cin.ignore();
    cout << "Enter Hostel (e.g., H1, H2, GH1): ";
    getline(cin, hostel);
    
    cout << "Enter Room Number: ";
    getline(cin, room);
    
    cout << "\n========== ORDER SUMMARY ==========\n";
    cout << "Delivery to: " << hostel << ", Room " << room << endl;
    cout << "Total: Rs." << fixed << setprecision(0) << cart.getTotal() << endl;
    cout << "===================================\n";
    
    cout << "\nConfirm order? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (confirm == 'y' || confirm == 'Y') {
        int orderID = orderManager.placeOrder(
            user.getPhone(),
            user.getName(),
            cart.getItems(),
            cart.getItemCount(),
            cart.getTotal(),
            hostel,
            room
        );
        
        cout << "\nOrder #" << orderID << " placed successfully!\n";
        cout << "You will be notified when it's ready.\n";
        
        cart.clearCart();
    } else {
        cout << "\nOrder cancelled.\n";
    }
    
    Utils::pause();
}

void viewOrderHistory(const User& user, OrderManager& orderManager) {
    Utils::clearScreen();
    orderManager.displayStudentOrders(user.getPhone());
    Utils::pause();
}

void viewPendingOrders(OrderManager& orderManager) {
    Utils::clearScreen();
    
    cout << "\n========== KITCHEN - PENDING ORDERS ==========\n";
    orderManager.displayOrdersByStatus(PENDING);
    
    cout << "\n========== KITCHEN - PREPARING ORDERS ==========\n";
    orderManager.displayOrdersByStatus(PREPARING);
    
    Utils::pause();
}

void updateOrderStatus(OrderManager& orderManager) {
    while (true) {
        Utils::clearScreen();
        
        cout << "\n========== PENDING ORDERS (Not Started) ==========\n";
        orderManager.displayOrdersByStatus(PENDING);
        
        cout << "\n========== PREPARING ORDERS (In Kitchen) ==========\n";
        orderManager.displayOrdersByStatus(PREPARING);
        
        cout << "\n========== UPDATE ORDER STATUS ==========\n";
        cout << "1. Start Preparing Next Order (from queue)\n";
        cout << "2. Mark Order as Ready (move to delivery)\n";
        cout << "3. Back\n";
        cout << "=========================================\n";
        
        int choice = Utils::getIntInput("Enter choice: ");
        
        if (choice == 1) {
            orderManager.startPreparingOrder();
            Utils::pause();
        } else if (choice == 2) {
            int orderID = Utils::getIntInput("Enter Order ID to mark as ready: ");
            orderManager.markOrderReady(orderID);
            Utils::pause();
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice!\n";
            Utils::pause();
        }
    }
}
void viewReadyOrders(OrderManager& orderManager) {
    Utils::clearScreen();
    orderManager.displayDeliveryQueue();
    Utils::pause();
}

void acceptDelivery(const User& user, OrderManager& orderManager) {
    Utils::clearScreen();
    
    // Show ready orders with details
    cout << "\n========== READY FOR DELIVERY ==========\n";
    orderManager.displayOrdersByStatus(READY);
    orderManager.displayDeliveryQueue();
    
    cout << "\nAccept next delivery? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (confirm == 'y' || confirm == 'Y') {
        // Get the order ID from the front of queue
        int orderID = Utils::getIntInput("Enter Order ID to accept: ");
        orderManager.assignDelivery(orderID, user.getName());
    }
    
    Utils::pause();
}

void viewMyDeliveries(const User& user, OrderManager& orderManager) {
    Utils::clearScreen();
    cout << "\n========== MY ACTIVE DELIVERIES ==========\n";
    orderManager.displayOrdersByStatus(DELIVERING);
    Utils::pause();
}

void markDelivered(OrderManager& orderManager) {
    Utils::clearScreen();
    
    // Show orders being delivered
    cout << "\n========== ORDERS OUT FOR DELIVERY ==========\n";
    orderManager.displayOrdersByStatus(DELIVERING);
    
    int orderID = Utils::getIntInput("\nEnter Order ID to mark as delivered (0 to cancel): ");
    
    if (orderID == 0) {
        return;
    }
    
    if (orderManager.markOrderDelivered(orderID)) {
        cout << "\n✓ Order #" << orderID << " marked as delivered!\n";
        cout << "Payment collected. Thank you!\n";
    } else {
        cout << "\n✗ Failed to mark order as delivered!\n";
        cout << "Make sure the order is currently being delivered.\n";
    }
    
    Utils::pause();
}

void manageMenuItems(MenuManager& menuManager) {
    while (true) {
        Utils::clearScreen();
        cout << "\n========== MENU MANAGEMENT ==========\n";
        cout << "1. View All Items\n";
        cout << "2. Add New Item\n";
        cout << "3. Remove Item\n";
        cout << "4. Update Item Price\n";
        cout << "5. Toggle Availability\n";
        cout << "6. Back\n";
        cout << "=====================================\n";
        
        int choice = Utils::getIntInput("Enter choice: ");
        
        if (choice == 1) {
            Utils::clearScreen();
            menuManager.displayAllItems();
            Utils::pause();
            
        } else if (choice == 2) {
            while (true) {
                Utils::clearScreen();
                cout << "\n========== ADD NEW ITEM ==========\n";
                
                string name, category;
                double price;
                
                cin.ignore();
                cout << "Enter item name (or 'cancel' to go back): ";
                getline(cin, name);
                
                if (name == "cancel" || name == "Cancel") {
                    break;
                }
                
                price = Utils::getDoubleInput("Enter price: ");
                
                if (price <= 0) {
                    cout << "\nPrice must be greater than 0!\n";
                    cout << "\nWhat would you like to do?\n";
                    cout << "1. Try again\n";
                    cout << "2. Cancel\n";
                    
                    int retryChoice = Utils::getIntInput("Enter choice: ");
                    
                    if (retryChoice == 1) {
                        continue;
                    } else {
                        break;
                    }
                }
                
                cin.ignore();
                cout << "Enter category (meal/snack/drink): ";
                getline(cin, category);
                
                if (category != "meal" && category != "snack" && category != "drink") {
                    cout << "\nInvalid category! Must be: meal, snack, or drink\n";
                    cout << "\nWhat would you like to do?\n";
                    cout << "1. Try again\n";
                    cout << "2. Cancel\n";
                    
                    int retryChoice = Utils::getIntInput("Enter choice: ");
                    
                    if (retryChoice == 1) {
                        continue;
                    } else {
                        break;
                    }
                }
                
                int newID = menuManager.getNextID();
                MenuItem newItem(newID, name, price, category, true);
                menuManager.addItem(newItem);
                
                cout << "\nItem added successfully! ID: " << newID << "\n";
                Utils::pause();
                break;
            }
            
        } else if (choice == 3) {
            Utils::clearScreen();
            menuManager.displayAllItems();
            
            int itemID = Utils::getIntInput("\nEnter Item ID to remove (0 to cancel): ");
            
            if (itemID == 0) {
                continue;
            }
            
            cin.ignore();
            cout << "Are you sure you want to remove this item? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            
            if (confirm == 'y' || confirm == 'Y') {
                if (menuManager.removeItem(itemID)) {
                    cout << "Item removed successfully!\n";
                } else {
                    cout << "Item not found.\n";
                }
            } else {
                cout << "Cancelled.\n";
            }
            Utils::pause();
            
        } else if (choice == 4) {
            Utils::clearScreen();
            menuManager.displayAllItems();
            
            int itemID = Utils::getIntInput("\nEnter Item ID (0 to cancel): ");
            
            if (itemID == 0) {
                continue;
            }
            
            double newPrice = Utils::getDoubleInput("Enter new price: ");
            
            if (newPrice <= 0) {
                cout << "Price must be greater than 0!\n";
                Utils::pause();
                continue;
            }
            
            MenuItem* item = menuManager.findItem(itemID);
            if (item != nullptr) {
                item->setPrice(newPrice);
                menuManager.saveMenu();
                cout << "Price updated to Rs." << fixed << setprecision(0) << newPrice << "\n";
            } else {
                cout << "Item not found.\n";
            }
            Utils::pause();
            
        } else if (choice == 5) {
            Utils::clearScreen();
            menuManager.displayAllItems();
            
            int itemID = Utils::getIntInput("\nEnter Item ID (0 to cancel): ");
            
            if (itemID == 0) {
                continue;
            }
            
            MenuItem* item = menuManager.findItem(itemID);
            if (item != nullptr) {
                bool newStatus = !item->isAvailable();
                item->setAvailable(newStatus);
                menuManager.saveMenu();
                cout << "Item is now: " << (newStatus ? "Available" : "Not Available") << "\n";
            } else {
                cout << "Item not found.\n";
            }
            Utils::pause();
            
        } else if (choice == 6) {
            break;
        } else {
            cout << "\nInvalid choice!\n";
            Utils::pause();
        }
    }
}

void viewCampusMap(CampusGraph& campusGraph) {
    Utils::clearScreen();
    campusGraph.displayMap();
    Utils::pause();
}