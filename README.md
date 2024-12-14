# Personal Finance Manager

## Latest Updates (As of December 12, 2024)

- **Dashboard Removed:**
  - The main dashboard has been removed for now. After logging in, the default view is now the **View Transactions** screen.

- **UI Reset Functionality:**
  - **`resetUI` Methods:** Added `resetUI` methods to all UI classes (Log In, Sign Up, View Transactions, View Graph, Add Transaction, and Settings). These methods reset the UI back to default when navigating away from the screen, ensuring a consistent user experience.

- **User Interface Enhancements:**
  - **Login & Sign Up Screens:** Users can create new accounts, log in with existing credentials, and recover passwords if forgotten.
  - **View Transactions Screen:** The default view after logging in. Users can see a comprehensive list of their transactions.
  - **Add Transaction Form:** A dedicated form that allows adding new income or expense transactions with optional tax withholding.
  - **View Graph Screen (Qt Charts):** A GraphView is implemented to visualize income and expense trends over time, with category and subcategory filters.
  - **Settings Screen:** Users can update their account details (username, password, name, and position).

---

## Table of Contents

- [Project Overview](#project-overview)
- [Current Status](#current-status)
  - [What's Working](#whats-working)
  - [What's Not Working](#whats-not-working)
  - [Known Issues](#known-issues)
- [Timeline to Completion](#timeline-to-completion)
- [Project Requirements](#project-requirements)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
  - [Running the Application](#running-the-application)
- [Usage](#usage)
- [Code Structure](#code-structure)

---

## Project Overview

The Personal Finance Manager is a graphical budget and expense tracker designed to help users manage their personal finances effectively. The application supports multiple user accounts, secure login/logout, and provides a platform for recording and analyzing financial data. Users can track their incomes, expenses, and see visual summaries through charts.

---

## Current Status

### What's Working

- **User Accounts & Authentication:**
  - Full sign-up, login, and logout processes supported.
  - Credentials and user details are stored securely in a SQLite database.

- **Transaction Management:**
  - Users can add income and expense transactions.
  - Each transaction is recorded with Date, Category, Subcategory, and Amount.

- **Data Visualization:**
  - Integrated Qt Charts to display income and expense trends over time.
  - Graph filtering by category and subcategory supported.

- **Settings & Profile Management:**
  - Users can update their username, password, and personal details (first name, last name, position).

### What's Not Working

- **Editing/Deleting Transactions:**
  - Currently, transactions can be added but not edited or deleted.
  - Future updates will address transaction modification and removal.

- **Advanced Validation & Error Handling:**
  - Input validation and error feedback can be improved.
  - More robust error handling for database operations is still needed.

### Known Issues

- **Limited Testing:**
  - Comprehensive testing and debugging are still in progress.
  - Edge cases and exceptional inputs may not be handled gracefully yet.

---

## Timeline to Completion

**Coming Soon:**

- Implement transaction editing and deletion.
- Enhance input validation and error handling.
- Refine UI/UX.

---

## Project Requirements

The project satisfies the following requirements:

1. **Income and Expense Tracking:**
   - Users can add multiple income and expense entries.
   - Optional tax withholding supported for income entries.

2. **Balance Sheet Display:**
   - The View Transactions screen shows transactions in a table with running balances.

3. **Category Filtering:**
   - Transactions can be filtered by category and subcategory.
   - Summarized daily totals visible in both tables and charts.

4. **Data Visualization:**
   - Qt Charts integrated for visualizing financial data over time.
   - Users can view separate income and expense trend lines.

5. **Database Storage (SQLite):**
   - User and transaction data stored in a relational database.
   - Secure and persistent data handling.

6. **User Authentication:**
   - Full sign-up and login process integrated.
   - Credentials validated against database records.

7. **Documentation:**
   - Code documentation improved with user and installation instructions.

---

## Getting Started

### Prerequisites

- **Development Environment:**
  - **Qt Framework**: Qt 5 or Qt 6 (including Core, Widgets, Charts, and SQL modules).
  - **C++ Compiler**: A C++17 compliant compiler (e.g., GCC, Clang, MSVC).
  - **SQLite**: Included by default with Qt.
  - **CMake** (optional): If you prefer building with CMake.

### Building the Project

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/peterbucci/PersonalFinanceManager.git
   cd PersonalFinanceManager
   ```

2. **Open the Project in Qt Creator:**
   - Launch **Qt Creator**.
   - Open the `.pro` file or `CMakeLists.txt` file from the project root directory.

3. **Configure the Project:**
   - If using **qmake**:
     - In Qt Creator, configure the project using `qmake`.
   - If using **CMake**:
     ```bash
     mkdir build
     cd build
     cmake ..
     ```

4. **Build the Project:**
   - In Qt Creator, click the **Build** button.
   - Or if using the terminal with **CMake**:
     ```bash
     make
     ```

### Running the Application

1. **Set Up the Database** (if not already created):
   - The application will create an `app.db` file in the project directory if it doesn't exist.
   - Ensure the application has write permissions for the project directory.

2. **Run the Application:**
   - **From Qt Creator**:
     - Click the **Run** button.
   - **From the Terminal**:
     ```bash
     ./PersonalFinanceManager
     ```

3. **Login or Sign Up**:
   - If no users exist, sign up to create a new account.
   - After logging in, you will be taken to the **View Transactions** screen by default.

---

## Usage

1. **User Registration & Login:**
   - Run the application and create a new user account.
   - Navigate to the Sign-up page and create a new account.
   - Log in using your credentials to access the **View Transactions** screen.

2. **Navigating the Application:**
   - Use the navigation combo box to move between the following screens:
     - **View Transactions**
     - **View Graph**
     - **Add Transaction**
     - **Settings**
     - **Logout**

3. **Adding Transactions:**
   - Open the transaction form from the navigation menu.
   - Enter details and save. Your transaction is recorded in the database.

4. **Filtering & Viewing Data:**
   - Use the **View Transactions** or **Graph View** screens to filter by category and subcategory.
   - Analyze trends over time via charts.

5. **Updating Profile:**
   - In the **Settings** page, update your username, password, and personal details as needed.

---

## Code Structure

- **`MainWindow.cpp/.h`**: Sets up the main application window, manages navigation between different views, and maintains the currently logged-in user and their ledger.
- **`LoginWindow.cpp/.h`**: Handles the user login interface, including "Remember Me" and "Forgot Password" options.
- **`SignUpWindow.cpp/.h`**: Manages the sign-up process for new users.
- **`User.cpp/.h` & `UserLogin.cpp/.h`**: Represents a user and their login credentials.
- **`Ledger.cpp/.h`**: Manages a collection of transactions and maintains a running balance.
- **`Transaction.cpp/.h`**: Represents individual financial transactions and provides database read/write methods.
- **`ViewTransactions.cpp/.h`**: Displays a detailed list of all transactions with filtering capabilities.
- **`GraphView.cpp/.h`**: Implements data visualization logic using Qt Charts with filtering capabilities.
- **`TransactionForm.cpp/.h`**: Provides the interface for adding new transactions.
- **`Settings.cpp/.h`**: Implements the Settings UI, allowing users to update their account details.



