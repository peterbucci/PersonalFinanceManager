# Personal Finance Manager

## Table of Contents

- [Overview](#overview)
- [Installation and Setup](#installation-and-setup)
  - [System Requirements](#system-requirements)
  - [Getting the Source Code](#getting-the-source-code)
  - [Building the Project](#building-the-project)
    - [Using Qt Creator and QMake (Recommended)](#using-qt-creator-and-qmake-recommended)
    - [Using Terminal and CMake](#using-terminal-and-cmake)
  - [Running the Application](#running-the-application)
- [Usage](#usage)
  - [Main Features](#main-features)
  - [Basic Navigation](#basic-navigation)
  - [Adding Transactions](#adding-transactions)
  - [Viewing & Filtering Transactions](#viewing--filtering-transactions)
  - [Viewing Graphs](#viewing-graphs)
  - [Changing Settings](#changing-settings)
- [Code Structure](#code-structure)
  - [Key Components](#key-components)
- [Known Limitations](#known-limitations)
- [License](#license)
- [Author](#author)

## Overview

The **Personal Finance Manager** application is a desktop tool for managing personal finances. It allows users to:

- Create user accounts and securely log in.
- Record income and expense transactions.
- View transaction histories and running balances.
- Filter transactions by category and subcategory.
- Visualize financial data in graphs.
- Update personal details and credentials.

This application uses the **Qt framework** for its graphical interface and **SQLite** for data storage.

---

## Installation and Setup

### System Requirements

**Operating System:**
- Windows, macOS, or Linux

**Software Requirements:**
- **Qt Framework:** Qt 5 or Qt 6 (including Core, Widgets, Charts, and SQL modules)
- **C++ Compiler:** A C++17 compliant compiler (e.g., GCC, Clang, MSVC)
- **SQLite:** Comes included with Qt
- **CMake (optional):** If you prefer a CMake-based build

### Getting the Source Code

Clone the repository using Git:

```bash
# Using Terminal
git clone https://github.com/peterbucci/PersonalFinanceManager.git
cd PersonalFinanceManager
```

### Building the Project

#### Using Qt Creator and QMake (Recommended)

1. **Open Project:**
   - Start Qt Creator and open the `.pro` file in the project directory.

2. **Configure & Build:**
   - From the menu bar, select **Build** > **Build Project "PersonalFinanceManager"**.

#### Using Terminal and CMake

1. **Setting Up the Build Environment:**
   ```bash
   mkdir build && cd build
   cmake ..
   ```

2. **Build:**
   - **macOS/Linux/Unix-like Environments (e.g., Git Bash):**
     ```bash
     make
     ```
   - **Windows:**
     ```bash
     cmake --build .
     ```

**Result:** After a successful build, you’ll have an executable in the appropriate build directory (e.g., `./build/Desktop-Debug/PersonalFinanceManager`).

### Running the Application

1. **Initialize the Database:**
   - On the first run, the application creates an `app.db` file in the project’s build directory. Ensure the application has permission to write to this location.

2. **Launch the Application:**
   - **From Qt Creator:**
     1. On the left sidebar, click the **Kit Selector** (Monitor icon).
     2. Select a Build Configuration:
        - **Debug:** Compiles with debug symbols for troubleshooting.
        - **Profile:** Compiles with profiling tools for performance analysis.
        - **Release:** Compiles with optimizations for performance.
     3. Click the **Run Command** (Green Arrow) to launch the app.

   - **From the Terminal:**
     ```bash
     # macOS/Linux
     ./PersonalFinanceManager
     
     # Windows
     PersonalFinanceManager.exe
     ```

3. **Login or Sign Up:**
   - If no accounts exist, select **Sign Up** to create a new user. Otherwise, log in.

---

## Usage

### Main Features

- **User Accounts:** Create an account, log in, and log out.
- **Secure Password Storage:** All passwords are hashed before being stored.
- **Transaction Management:** Add income and expense transactions.
- **View Transactions:** See all transactions, filter by category/subcategory, and view running balances.
- **Data Visualization:** Display income or expenses over time using line graphs.
- **Settings:** Update user details and change passwords.

### Basic Navigation

After logging in, you are placed on the **View Transactions** screen. Use the navigation combo box at the top to switch between pages:

- **View Transactions:** Shows all your recorded transactions.
- **View Graphs:** Displays filtered financial data over time.
- **Add Transaction:** Add a new income or expense entry.
- **Settings:** Update username, password, and personal details.
- **Logout:** Exit your account and return to the login screen.

### Adding Transactions

1. Go to **Add Transaction**.
2. Select the date, category, and optionally a subcategory.
3. Enter the amount.
4. Choose **Income** or **Expense**. If it’s an income and involves taxes, check the withholding option and enter the tax amount.
5. Click **Save**. The transaction is recorded in the database.

### Viewing & Filtering Transactions

1. Go to **View Transactions**.
2. Click **Show Options** to filter results.
3. Choose a category and/or type a subcategory to narrow the list.
4. The table updates automatically to display filtered transactions.
5. If no filters are selected, you can see running balances per transaction line. With filters, a total row shows the sum at the bottom.

### Viewing Graphs

1. Go to **View Graphs**.
2. Click **Show Options** to filter by category or subcategory.
3. Choose **Expenses** or **Income** to display the corresponding data line.
4. The graph updates to show trends over time.

### Changing Settings

1. Go to **Settings**.
2. Update your username and personal details.
3. Change your password (if desired).
4. Click **Save**.

---

## Code Structure

### Key Components

- **MainWindow:** Handles navigation and coordinates the various screens.
- **LoginWindow & SignUpWindow:** Manage user authentication and account creation.
- **ViewTransactions:** Displays a table of transactions and applies filters.
- **GraphView:** Shows transactions as a line graph with filtering options.
- **TransactionForm:** Allows adding new income or expense entries.
- **Settings:** Lets users update account details and passwords.
- **PasswordManager:** Handles password hashing and validation.
- **User & UserLogin:** Represent user and login details.
- **Transaction & Ledger:** Store and manage financial transactions.

**Database:**
- The SQLite database `app.db` is automatically created and used for storing user credentials and transaction data.

---

## Known Limitations

- **No Transaction Editing/Deletion:** Currently, you can only add transactions. Editing or removing existing transactions will be implemented later.

---

### License

[Insert License Here]

---

### Author

[Peter Bucci](https://github.com/peterbucci)
