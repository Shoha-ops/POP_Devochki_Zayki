# POP_Devochki_Zayki 

A C++ shopping system project with user management, shop management, and order processing.

## Project Description

This is an Object-Oriented Programming (OOP) project implementing a complete shopping system with the following features:
- **User Management**: Registration, login, and user profile management
- **Shop Management**: Product management and inventory control
- **Order Management**: Create, track, and manage customer orders
- **Admin Panel**: Administrative operations

## Project Structure

```
POP_Devochki_Zayki/
├── CMakeLists.txt           # Build configuration
├── .gitignore               # Git ignore rules
├── README.md               # This file
│
├── main.cpp                # Application entry point
├── check.h                 # Input validation utilities
├── Core Classes.h          # Core domain classes (Product, Order)
├── Data Storage.h          # Data persistence functions
│
├── User/                   # User management module
│   ├── User.h             # User class definition
│   └── UserManager.h      # User management logic
│
├── Shop/                   # Shop management module
│   ├── Shop.h             # Shop class definition
│   ├── ShopManager.h      # Shop management logic
│   └── OrderManager.h     # Order management logic
│
└── Admin/                  # Admin management module
    └── AdminManager.h     # Admin operations
```


## Features

### 1. User Management
- User registration with validation
- Secure login system
- User profile management
- Persistent user storage

### 2. Shop System
- Add products to shop
- Update product quantities
- View available products
- Manage inventory

### 3. Order Processing
- Create new orders
- Assign orders to shops
- Track order status
- Update order information

### 4. Admin Functions
- Add/manage administrators
- System operations

## Usage

1. **Run the application**:
   ```
   ./POP_Devochki_Zayki
   ```

2. **Menu Options**:
   - `1` - Register as a new user
   - `2` - Log in to existing account
   - `0` - Exit the application

3. **Registration**:
   - Enter desired username
   - Enter password
   - Choose role (1 for Customer, 2 for Seller)

4. **Login**:
   - Enter username
   - Enter password
   - Access main shopping menu

## Class Hierarchy

### Core Classes (`Core Classes.h`)
- `Product`: Represents a product with ID, name, price, quantity
- `Order`: Represents customer order with ID, user ID, shop ID, products, status

### User Module (`User/`)
- `User`: User entity with profile information
- `UserManager`: Manages user registration, login, and data persistence

### Shop Module (`Shop/`)
- `Shop`: Shop entity with inventory management
- `ShopManager`: Manages multiple shops
- `OrderManager`: Handles order creation and status updates

### Admin Module (`Admin/`)
- `AdminManager`: Manages administrator operations

## File Descriptions

| File             | Purpose |
|------------------|---------|
| `main.cpp`       | Entry point with user interface menu |
| `check.h`        | Template for safe input validation |
| `Core Classes.h` | Base domain model definitions |
| `Data Storage.h` | Persistence layer functions |
| `CMakeLists.txt` | Build configuration for CMake |

## Data Persistence

- User data is saved to `users.txt`
- Order data can be saved to storage files
- Data is loaded automatically on application startup

