# Console-Based Banking Management System

A robust, console-based Banking Management System implemented in C++. This application simulates core banking operations including customer onboarding, account management, deposits, withdrawals, fund transfers, and real-time transaction history tracking.

---

## 🚀 Features

### 1. Customer Management
* **Onboarding:** Create unique customer profiles with automated ID generation (`CUST1001`, `CUST1002`, etc.).
* **Data Validation:** Integrated validation checks for 10-digit phone numbers and valid email formats.
* **Duplication Prevention:** Ensures uniqueness by blocking duplicate registrations with the same phone number or email.

### 2. Account Management
* **Multiple Account Types:** Supports both **Savings** and **Current** accounts.
* **Minimum Balance Protection:** Enforces a minimum initial deposit and maintaining balance:
    * **Savings:** $500.00
    * **Current:** $1,000.00
* **Account Linking:** Automatically maps new accounts to existing Customer IDs.

### 3. Financial Transactions
* **Deposits & Withdrawals:** Safely add or remove funds while enforcing positive numeric entry and maintaining minimum balance thresholds.
* **Fund Transfers:** Send money securely between two valid accounts with validation checks for identical accounts, inactive accounts, and overdraft protection.
* **Automated Bookkeeping:** Generates automatic ledger records for both successful and failed transaction attempts.

### 4. Records & Auditing
* **Transaction History:** View chronological transaction history (defaulting to the last 10 entries) complete with unique transaction IDs, timestamps, type, and failure logs if applicable.
* **Customer Portfolio Lookup:** Displays comprehensive customer information including details of all linked accounts and their 5 most recent transactions.

---

## 🛠️ Technical Design & Data Structures

The system relies on efficient C++ Standard Template Library (STL) containers to handle data dynamically in-memory:

* **`std::unordered_map`**: Used for `customers` and `accounts` tables to ensure $O(1)$ average time complexity for lookups, insertions, and updates via unique IDs.
* **`std::deque`**: Used inside each account to log transactions. Utilizing `push_front` allows the system to seamlessly prepend new records, putting the latest transaction history at the top ($O(1)$ efficiency).
* **`std::chrono`**: Utilized to capture precise, real-time system timestamps for every transaction ledger entry.

---

## 💻 How to Build and Run

### Prerequisites
Ensure you have a C++ compiler installed that supports **C++11 or higher** (e.g., `g++`, Clang, or MSVC).

### Compilation via Terminal
Open your terminal or command prompt, navigate to the directory containing your source file, and run:

```bash
g++ -std=c++11 main.cpp -o BankingSystem
