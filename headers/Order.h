#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <fstream>
#include "MenuItem.h"
#include "Cart.h"
using namespace std;

// Order status enum
enum OrderStatus {
    PENDING,      // Just placed, waiting for kitchen
    PREPARING,    // Kitchen is preparing
    READY,        // Ready for delivery
    DELIVERING,   // Out for delivery
    DELIVERED     // Completed
};

// OrderItem - item within an order
struct OrderItem {
    int itemID;
    string itemName;
    double price;
    int quantity;
    
    OrderItem() : itemID(0), itemName(""), price(0.0), quantity(0) {}
    OrderItem(int id, string name, double p, int qty) 
        : itemID(id), itemName(name), price(p), quantity(qty) {}
};

// Order class
class Order {
private:
    int orderID;
    string studentPhone;
    string studentName;
    OrderItem items[20];     // Array of order items
    int itemCount;
    double totalPrice;
    string hostel;
    string room;
    string timestamp;
    OrderStatus status;
    string deliveryPerson;   // Assigned delivery person

public:
    Order();
    Order(int id, string phone, string name, CartItem* cartItems, int cartCount, 
          double total, string hostel, string room, string time);
    
 
    int getOrderID() const { return orderID; }
    string getStudentPhone() const { return studentPhone; }
    string getStudentName() const { return studentName; }
    double getTotalPrice() const { return totalPrice; }
    string getHostel() const { return hostel; }
    string getRoom() const { return room; }
    string getTimestamp() const { return timestamp; }
    OrderStatus getStatus() const { return status; }
    string getDeliveryPerson() const { return deliveryPerson; }
    int getItemCount() const { return itemCount; }
    OrderItem* getItems() { return items; }
    
    void setStatus(OrderStatus s) { status = s; }
    void setDeliveryPerson(string name) { deliveryPerson = name; }
    
    // Display
    void displayShort() const;
    void displayDetailed() const;
    string getStatusString() const;
};

// OrderNode for Linked List
struct OrderNode {
    Order data;
    OrderNode* next;
    
    OrderNode(Order order) : data(order), next(nullptr) {}
};


class OrderLinkedList {
private:
    OrderNode* head;
    int count;

public:
    OrderLinkedList();
    ~OrderLinkedList();
    
    // Linked List operations 
    void insert(Order order);
    bool remove(int orderID);
    Order* find(int orderID);
    void displayAll() const;
    void displayByStatus(OrderStatus status) const;
    void displayByStudent(string phone) const;
    
    int getCount() const { return count; }
    bool isEmpty() const { return head == nullptr; }
    
    OrderNode* getHead() const { return head; }
    
    
    void saveToFile(string filename);
    void loadFromFile(string filename);
};

// KitchenQueue - Circular Queue using Array 
class KitchenQueue {
private:
    int orderIDs[50];    // Store only order IDs
    int front;
    int rear;
    int size;
    int capacity;

public:
    KitchenQueue();
    
    // Queue operations 
    bool enqueue(int orderID);      // Add order to queue
    int dequeue();                  // Remove and return order ID
    int peek() const;               // View front without removing
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const { return size; }
    
    // Display
    void displayQueue() const;
};

// DeliveryQueue -  Queue 
class DeliveryQueue {
private:
    int orderIDs[50];    // Store only order IDs
    int front;
    int rear;
    int size;
    int capacity;

public:
    DeliveryQueue();
    
    // Queue operations 
    bool enqueue(int orderID);
    int dequeue();
    int peek() const;
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const { return size; }
    
    void displayQueue() const;
};

// OrderManager - Main order management system
class OrderManager {
private:
    OrderLinkedList allOrders;       // All orders (Linked List)
    KitchenQueue kitchenQueue;       // Pending orders queue
    DeliveryQueue deliveryQueue;     // Ready for delivery queue
    int nextOrderID;
    string filename;

public:
    OrderManager(string filename = "data/orders.txt");
    
    // Order operations
    int placeOrder(string studentPhone, string studentName, CartItem* cartItems, 
                   int cartCount, double total, string hostel, string room);
    
    // Kitchen operations
    void displayKitchenQueue();
    bool startPreparingOrder();      // Dequeue from kitchen
    bool markOrderReady(int orderID); // Move to delivery queue
    
    // Delivery operations
    void displayDeliveryQueue();
    bool assignDelivery(int orderID, string deliveryPerson);
    bool markOrderDelivered(int orderID);
    
    // View orders
    void displayAllOrders();
    void displayStudentOrders(string phone);
    void displayOrdersByStatus(OrderStatus status);
    Order* findOrder(int orderID);
    
    int getDeliveryQueueSize() const { return deliveryQueue.getSize(); }
    int getKitchenQueueSize() const { return kitchenQueue.getSize(); }
    bool isDeliveryQueueEmpty() const { return deliveryQueue.isEmpty(); }
    bool isKitchenQueueEmpty() const { return kitchenQueue.isEmpty(); }
    
    void saveOrders();
    void loadOrders();
    
    int getNextOrderID() const { return nextOrderID; }
};

#endif