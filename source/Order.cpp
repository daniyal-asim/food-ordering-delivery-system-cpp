#include "../headers/Order.h"
#include "../headers/Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Order implementation
Order::Order() {
    orderID = 0;
    studentPhone = "";
    studentName = "";
    itemCount = 0;
    totalPrice = 0.0;
    hostel = "";
    room = "";
    timestamp = "";
    status = PENDING;
    deliveryPerson = "";
}

Order::Order(int id, string phone, string name, CartItem* cartItems, int cartCount, 
             double total, string hostel, string room, string time) {
    this->orderID = id;
    this->studentPhone = phone;
    this->studentName = name;
    this->totalPrice = total;
    this->hostel = hostel;
    this->room = room;
    this->timestamp = time;
    this->status = PENDING;
    this->deliveryPerson = "";
    
    // Copy cart items
    this->itemCount = (cartCount < 20) ? cartCount : 20;
    for (int i = 0; i < this->itemCount; i++) {
        items[i] = OrderItem(
            cartItems[i].item.getItemID(),
            cartItems[i].item.getName(),
            cartItems[i].item.getPrice(),
            cartItems[i].quantity
        );
    }
}

string Order::getStatusString() const {
    switch(status) {
        case PENDING: return "Pending";
        case PREPARING: return "Preparing";
        case READY: return "Ready";
        case DELIVERING: return "Delivering";
        case DELIVERED: return "Delivered";
        default: return "Unknown";
    }
}

void Order::displayShort() const {
    cout << left << setw(6) << orderID
         << setw(18) << studentName
         << "Rs." << right << setw(6) << fixed << setprecision(0) << totalPrice
         << " " << left << setw(14) << (hostel + "-" + room)
         << timestamp.substr(11, 5) << endl;  // Show time only
}

void Order::displayDetailed() const {
    cout << "\n========== ORDER DETAILS ==========\n";
    cout << "Order ID: " << orderID << endl;
    cout << "Student: " << studentName << " (" << studentPhone << ")\n";
    cout << "Location: " << hostel << ", Room " << room << endl;
    cout << "Status: " << getStatusString() << endl;
    cout << "Order Time: " << timestamp << endl;
    if (!deliveryPerson.empty()) {
        cout << "Delivery Person: " << deliveryPerson << endl;
    }
    cout << "\nItems:\n";
    cout << "-----------------------------------\n";
    for (int i = 0; i < itemCount; i++) {
        cout << left << setw(20) << items[i].itemName
             << "x" << items[i].quantity
             << " = Rs." << fixed << setprecision(0) 
             << (items[i].price * items[i].quantity) << endl;
    }
    cout << "-----------------------------------\n";
    cout << "Total: Rs." << fixed << setprecision(0) << totalPrice << endl;
    cout << "===================================\n";
}

// OrderLinkedList implementation
OrderLinkedList::OrderLinkedList() {
    head = nullptr;
    count = 0;
}

OrderLinkedList::~OrderLinkedList() {
    OrderNode* current = head;
    while (current != nullptr) {
        OrderNode* next = current->next;
        delete current;
        current = next;
    }
}

void OrderLinkedList::insert(Order order) {
    OrderNode* newNode = new OrderNode(order);
    
    if (head == nullptr) {
        head = newNode;
    } else {
        // Insert at end
        OrderNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    count++;
}

bool OrderLinkedList::remove(int orderID) {
    if (head == nullptr) return false;
    
    // If head is the target
    if (head->data.getOrderID() == orderID) {
        OrderNode* temp = head;
        head = head->next;
        delete temp;
        count--;
        return true;
    }
    
    // Search in rest of list
    OrderNode* current = head;
    while (current->next != nullptr) {
        if (current->next->data.getOrderID() == orderID) {
            OrderNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
            count--;
            return true;
        }
        current = current->next;
    }
    
    return false;
}

Order* OrderLinkedList::find(int orderID) {
    OrderNode* current = head;
    while (current != nullptr) {
        if (current->data.getOrderID() == orderID) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

void OrderLinkedList::displayAll() const {
    if (head == nullptr) {
        cout << "\nNo orders yet.\n";
        return;
    }
    
    cout << "\n========== ALL ORDERS ==========\n";
    cout << left << setw(8) << "ID"
         << setw(20) << "Student"
         << setw(10) << "Total"
         << setw(12) << "Status"
         << "Location\n";
    cout << "--------------------------------------------------------\n";
    
    OrderNode* current = head;
    while (current != nullptr) {
        current->data.displayShort();
        current = current->next;
    }
    cout << "\nTotal Orders: " << count << endl;
}

void OrderLinkedList::displayByStatus(OrderStatus status) const {
    bool found = false;
    OrderNode* current = head;
    int count = 0;
    
    // Count orders first
    OrderNode* temp = head;
    while (temp != nullptr) {
        if (temp->data.getStatus() == status) {
            count++;
        }
        temp = temp->next;
    }
    
    if (count == 0) {
        cout << "No orders with this status.\n";
        return;
    }
    
    // Display table header
    cout << left << setw(6) << "ID"
         << setw(18) << "Student"
         << setw(10) << "Total"
         << setw(15) << "Location"
         << "Time\n";
    cout << "----------------------------------------------------------------\n";
    
    // Display orders
    current = head;
    while (current != nullptr) {
        if (current->data.getStatus() == status) {
            cout << left << setw(6) << current->data.getOrderID()
                 << setw(18) << current->data.getStudentName()
                 << "Rs." << right << setw(6) << fixed << setprecision(0) << current->data.getTotalPrice()
                 << " " << left << setw(14) << (current->data.getHostel() + "-" + current->data.getRoom())
                 << current->data.getTimestamp().substr(11, 5) << endl;  // Show only time HH:MM
        }
        current = current->next;
    }
    
    cout << "----------------------------------------------------------------\n";
    cout << "Total: " << count << " order(s)\n";
}

void OrderLinkedList::displayByStudent(string phone) const {
    bool found = false;
    OrderNode* current = head;
    
    while (current != nullptr) {
        if (current->data.getStudentPhone() == phone) {
            if (!found) {
                cout << "\n========== YOUR ORDERS ==========\n";
                cout << left << setw(6) << "ID"
                     << setw(20) << "Name"
                     << setw(12) << "Total"
                     << setw(15) << "Status"
                     << "Location\n";
                cout << "------------------------------------------------------------\n";
                found = true;
            }
            
            // display for student view
            cout << left << setw(6) << current->data.getOrderID()
                 << setw(20) << current->data.getStudentName()
                 << "Rs." << right << setw(8) << fixed << setprecision(0) << current->data.getTotalPrice()
                 << " " << left << setw(14) << current->data.getStatusString()
                 << current->data.getHostel() << "-" << current->data.getRoom() << endl;
        }
        current = current->next;
    }
    
    if (!found) {
        cout << "\nYou haven't placed any orders yet.\n";
    }
}

void OrderLinkedList::saveToFile(string filename) {
    ofstream file(filename);
    if (!file.is_open()) return;
    
    file << "orderID,phone,name,total,hostel,room,timestamp,status,deliveryPerson,items\n";
    
    OrderNode* current = head;
    while (current != nullptr) {
        Order& order = current->data;
        file << order.getOrderID() << ","
             << order.getStudentPhone() << ","
             << order.getStudentName() << ","
             << order.getTotalPrice() << ","
             << order.getHostel() << ","
             << order.getRoom() << ","
             << order.getTimestamp() << ","
             << (int)order.getStatus() << ","
             << order.getDeliveryPerson() << ",";
        
        // Save items as: id:name:price:qty;id:name:price:qty;...
        for (int i = 0; i < order.getItemCount(); i++) {
            OrderItem* items = order.getItems();
            file << items[i].itemID << ":" 
                 << items[i].itemName << ":" 
                 << items[i].price << ":" 
                 << items[i].quantity;
            if (i < order.getItemCount() - 1) file << ";";
        }
        file << "\n";
        
        current = current->next;
    }
    
    file.close();
}

void OrderLinkedList::loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    
    string line;
    getline(file, line); 
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string idStr, phone, name, totalStr, hostel, room, timestamp, statusStr, deliveryPerson, itemsStr;
        
        getline(ss, idStr, ',');
        getline(ss, phone, ',');
        getline(ss, name, ',');
        getline(ss, totalStr, ',');
        getline(ss, hostel, ',');
        getline(ss, room, ',');
        getline(ss, timestamp, ',');
        getline(ss, statusStr, ',');
        getline(ss, deliveryPerson, ',');
        getline(ss, itemsStr, ',');
        
        // Parse items
        CartItem tempCart[20];
        int itemCount = 0;
        
        stringstream itemStream(itemsStr);
        string itemToken;
        while (getline(itemStream, itemToken, ';') && itemCount < 20) {
            stringstream itemSS(itemToken);
            string itemIdStr, itemName, priceStr, qtyStr;
            
            getline(itemSS, itemIdStr, ':');
            getline(itemSS, itemName, ':');
            getline(itemSS, priceStr, ':');
            getline(itemSS, qtyStr, ':');
            
            int itemId = stoi(itemIdStr);
            double price = stod(priceStr);
            int qty = stoi(qtyStr);
            
            tempCart[itemCount] = CartItem(MenuItem(itemId, itemName, price, "meal", true), qty);
            itemCount++;
        }
        
        Order order(stoi(idStr), phone, name, tempCart, itemCount, 
                   stod(totalStr), hostel, room, timestamp);
        order.setStatus((OrderStatus)stoi(statusStr));
        order.setDeliveryPerson(deliveryPerson);
        
        insert(order);
    }
    
    file.close();
}

// KitchenQueue implementation (Circular Queue)
KitchenQueue::KitchenQueue() {
    front = -1;
    rear = -1;
    size = 0;
    capacity = 50;
}

bool KitchenQueue::enqueue(int orderID) {
    if (isFull()) {
        cout << "Kitchen queue is full!\n";
        return false;
    }
    
    if (isEmpty()) {
        front = 0;
        rear = 0;
    } else {
        rear = (rear + 1) % capacity;
    }
    
    orderIDs[rear] = orderID;
    size++;
    return true;
}

int KitchenQueue::dequeue() {
    if (isEmpty()) {
        return -1;
    }
    
    int orderID = orderIDs[front];
    
    if (front == rear) {
        // Last element
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % capacity;
    }
    
    size--;
    return orderID;
}

int KitchenQueue::peek() const {
    if (isEmpty()) return -1;
    return orderIDs[front];
}

bool KitchenQueue::isEmpty() const {
    return (front == -1);
}

bool KitchenQueue::isFull() const {
    return (size == capacity);
}



// DeliveryQueue implementation
DeliveryQueue::DeliveryQueue() {
    front = -1;
    rear = -1;
    size = 0;
    capacity = 50;
}

bool DeliveryQueue::enqueue(int orderID) {
    if (isFull()) {
        cout << "Delivery queue is full!\n";
        return false;
    }
    
    if (isEmpty()) {
        front = 0;
        rear = 0;
    } else {
        rear = (rear + 1) % capacity;
    }
    
    orderIDs[rear] = orderID;
    size++;
    return true;
}

int DeliveryQueue::dequeue() {
    if (isEmpty()) {
        return -1;
    }
    
    int orderID = orderIDs[front];
    
    if (front == rear) {
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % capacity;
    }
    
    size--;
    return orderID;
}

int DeliveryQueue::peek() const {
    if (isEmpty()) return -1;
    return orderIDs[front];
}

bool DeliveryQueue::isEmpty() const {
    return (front == -1);
}

bool DeliveryQueue::isFull() const {
    return (size == capacity);
}

void DeliveryQueue::displayQueue() const {
    if (isEmpty()) {
        cout << "\nNo orders ready for delivery.\n";
        return;
    }
    
    cout << "\n========== DELIVERY QUEUE ==========\n";
    cout << "Orders ready: " << size << endl;
    cout << "Next order: #" << orderIDs[front] << endl;
    cout << "====================================\n";
}

// OrderManager implementation
OrderManager::OrderManager(string filename) {
    this->filename = filename;
    this->nextOrderID = 1;
    loadOrders();
}

int OrderManager::placeOrder(string studentPhone, string studentName, CartItem* cartItems, 
                             int cartCount, double total, string hostel, string room) {
    string timestamp = Utils::getCurrentTimestamp();
    
    Order newOrder(nextOrderID, studentPhone, studentName, cartItems, cartCount,
                   total, hostel, room, timestamp);
    
    // Add to linked list
    allOrders.insert(newOrder);
    
    // Add to kitchen queue
    kitchenQueue.enqueue(nextOrderID);
    
    cout << "\nOrder placed successfully! Order ID: " << nextOrderID << endl;
    cout << "Your order has been sent to the kitchen.\n";
    
    int placedOrderID = nextOrderID;
    nextOrderID++;
    
    saveOrders();
    return placedOrderID;
}

void OrderManager::displayKitchenQueue() {
    kitchenQueue.displayQueue();
    
    // Show details of next order in queue if available
    if (!kitchenQueue.isEmpty()) {
        int nextOrderID = kitchenQueue.peek();
        Order* order = allOrders.find(nextOrderID);
        if (order != nullptr) {
            cout << "\n========== NEXT ORDER DETAILS ==========\n";
            order->displayDetailed();
        }
    }
}

void KitchenQueue::displayQueue() const {
    if (isEmpty()) {
        cout << "Kitchen queue is empty - no orders waiting.\n";
        return;
    }
    
    cout << "Orders in queue: " << size << endl;
    cout << "Next to prepare: Order #" << orderIDs[front] << endl;
}

bool OrderManager::startPreparingOrder() {
    int orderID = kitchenQueue.dequeue();
    if (orderID == -1) {
        cout << "No orders in queue!\n";
        return false;
    }
    
    Order* order = allOrders.find(orderID);
    if (order != nullptr) {
        order->setStatus(PREPARING);
        saveOrders();
        cout << "Started preparing order #" << orderID << endl;
        return true;
    }
    return false;
}

bool OrderManager::markOrderReady(int orderID) {
    Order* order = allOrders.find(orderID);
    if (order == nullptr) {
        cout << "Error: Order #" << orderID << " not found!\n";
        return false;
    }
    
    // Check if order is in PREPARING status
    if (order->getStatus() != PREPARING) {
        cout << "Error: Order #" << orderID << " is not being prepared!\n";
        cout << "Current status: " << order->getStatusString() << endl;
        cout << "Only orders with status 'Preparing' can be marked as ready.\n";
        return false;
    }
    
    order->setStatus(READY);
    deliveryQueue.enqueue(orderID);
    saveOrders();
    
    cout << "Order #" << orderID << " is ready for delivery!\n";
    return true;
}

void OrderManager::displayDeliveryQueue() {
    deliveryQueue.displayQueue();
    
    if (!deliveryQueue.isEmpty()) {
        int nextOrderID = deliveryQueue.peek();
        Order* order = allOrders.find(nextOrderID);
        if (order != nullptr) {
            order->displayDetailed();
        }
    }
}

bool OrderManager::assignDelivery(int orderID, string deliveryPerson) {
    Order* order = allOrders.find(orderID);
    if (order == nullptr) {
        cout << "Error: Order #" << orderID << " not found!\n";
        return false;
    }
    
    // Check if order is READY for delivery
    if (order->getStatus() != READY) {
        cout << "Error: Order #" << orderID << " is not ready for delivery!\n";
        cout << "Current status: " << order->getStatusString() << endl;
        return false;
    }
    
    // Remove from delivery queue
    int dequeuedID = deliveryQueue.dequeue();
    if (dequeuedID != orderID) {
        cout << "Error: Order #" << orderID << " is not at front of queue!\n";
        // Put the dequeued order back
        if (dequeuedID != -1) {
            deliveryQueue.enqueue(dequeuedID);
        }
        return false;
    }
    
    order->setStatus(DELIVERING);
    order->setDeliveryPerson(deliveryPerson);
    saveOrders();
    
    cout << "Order #" << orderID << " assigned to " << deliveryPerson << endl;
    return true;
}
bool OrderManager::markOrderDelivered(int orderID) {
    Order* order = allOrders.find(orderID);
    if (order == nullptr) {
        cout << "Error: Order #" << orderID << " not found!\n";
        return false;
    }
    
    // Check if order is out for delivery
    if (order->getStatus() != DELIVERING) {
        cout << "Error: Order #" << orderID << " is not out for delivery!\n";
        cout << "Current status: " << order->getStatusString() << endl;
        return false;
    }
    
    order->setStatus(DELIVERED);
    saveOrders();
    
    cout << "Order #" << orderID << " marked as delivered!\n";
    return true;
}
void OrderManager::displayAllOrders() {
    allOrders.displayAll();
}

void OrderManager::displayStudentOrders(string phone) {
    allOrders.displayByStudent(phone);
}

void OrderManager::displayOrdersByStatus(OrderStatus status) {
    allOrders.displayByStatus(status);
}

Order* OrderManager::findOrder(int orderID) {
    return allOrders.find(orderID);
}

void OrderManager::saveOrders() {
    allOrders.saveToFile(filename);
}

void OrderManager::loadOrders() {
    allOrders.loadFromFile(filename);
    
    // Rebuild queues based on order status
    OrderNode* current = allOrders.getHead(); 
    
    while (current != nullptr) {
        Order& order = current->data;
        
        // Add PENDING orders back to kitchen queue
        if (order.getStatus() == PENDING) {
            kitchenQueue.enqueue(order.getOrderID());
        }
        
        // Add READY orders back to delivery queue
        if (order.getStatus() == READY) {
            deliveryQueue.enqueue(order.getOrderID());
        }
        
        current = current->next;
    }
    
    // Update nextOrderID to be higher than any existing order
    current = allOrders.getHead();
    int maxID = 0;
    while (current != nullptr) {
        if (current->data.getOrderID() > maxID) {
            maxID = current->data.getOrderID();
        }
        current = current->next;
    }
    nextOrderID = maxID + 1;
}