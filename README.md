# food-ordering-delivery-system-cpp


# Raju Food Ordering & Delivery Management System

A console-based food ordering and delivery management system built in C++ with role-based access control, real-time order tracking, and a graph-based campus routing system.

---

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [DSA Concepts](#dsa-concepts)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Data Persistence](#data-persistence)

---

## Features

- **Role-based access** — Student, Kitchen Staff, Delivery Staff, and Admin each have dedicated menus and permissions
- **Menu management** — Browse, search, filter by category, and sort by price or name
- **Cart system** — Add, remove, update quantities, and undo the last action
- **Order lifecycle** — Full pipeline from placement to delivery: `Pending → Preparing → Ready → Delivering → Delivered`
- **Delivery queue** — Ready orders managed via a circular queue; delivery staff accept and complete them
- **Campus map** — Graph-based visualization of hostel locations and distances for delivery routing
- **File persistence** — All users, menu items, and orders are saved to and loaded from text files

---

## Project Structure

```
├── main.cpp
├── data/
│   ├── admin_users.txt
│   ├── menu.txt
│   ├── orders.txt
│   └── users.txt
├── headers/
│   ├── Cart.h
│   ├── Graph.h
│   ├── MenuItem.h
│   ├── Order.h
│   ├── User.h
│   └── Utils.h
└── source/
    ├── Cart.cpp
    ├── Graph.cpp
    ├── MenuItem.cpp
    ├── Order.cpp
    ├── User.cpp
    └── Utils.cpp
```

---

## DSA Concepts

| Feature | Data Structure / Algorithm | Complexity |
|---|---|---|
| User login | Hash map | O(1) |
| Menu display | Array | O(n) |
| Menu sorting | Bubble sort | O(n²) |
| Menu search | Linear search | O(n) |
| Cart with undo | Stack + array | O(1) |
| Order management | Singly linked list | O(n) |
| Delivery queue | Circular queue | O(1) |
| Campus map | Graph (adjacency matrix) | O(1) edge lookup |

---

## Getting Started

### Prerequisites

- C++17 or later
- g++ (GCC / MinGW)

### Build

```bash
g++ main.cpp source/Cart.cpp source/Graph.cpp source/MenuItem.cpp source/Order.cpp source/User.cpp source/Utils.cpp -o raju
```

### Run

```bash
# Linux / macOS
./raju

# Windows
raju.exe
```

---

## Usage

### Student
- Register with registration number, phone number, and password
- Browse and search the menu, manage cart, place orders with hostel/room details, and track order status

### Kitchen Staff
- Login with admin-assigned credentials
- View and update order statuses, manage menu items (add, remove, update price, toggle availability)

### Delivery Staff
- Login with admin-assigned credentials
- Accept ready orders, track active deliveries, mark orders as delivered, view campus map

### Admin
- Login with `admin` / `admin`
- View all users, create staff accounts, view all orders, view campus map

---

## Data Persistence

All data is stored in plain text files under `data/` and loaded automatically on startup.

| File | Contents |
|---|---|
| `users.txt` | Registered student accounts |
| `admin_users.txt` | Staff credentials |
| `menu.txt` | Menu items |
| `orders.txt` | Full order history |
