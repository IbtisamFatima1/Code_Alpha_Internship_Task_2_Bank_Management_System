#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cctype>
#include <limits>
#include <deque>

using namespace std;

class Transaction {
public:
    string transactionID;
    string fromAccountID;
    string toAccountID;
    double amount;
    string type;
    string timestamp;
    string status;
    string failureReason;

    Transaction(string tID, string fID, string tID2, double amt, string typ, string ts, string stat, string failR = "") 
        : transactionID(tID), fromAccountID(fID), toAccountID(tID2), amount(amt), type(typ), timestamp(ts), status(stat), failureReason(failR) {}
};

class Account {
public:
    string accountID;
    string customerID;
    double balance;
    string type;
    bool isActive;
    deque<Transaction> transactions;

    Account() : balance(0.0), isActive(false) {}
    Account(string aID, string cID, string t, double b = 0.0) 
        : accountID(aID), customerID(cID), type(t), balance(b), isActive(true) {}
};

class Customer {
public:
    string customerID;
    string name;
    string phone;
    string email;
    vector<string> accountIDs;

    Customer() {}
    Customer(string cID, string n, string p, string e) 
        : customerID(cID), name(n), phone(p), email(e) {}
};

unordered_map<string, Customer> customers;
unordered_map<string, Account> accounts;

int customerCounter = 1001;
int accountCounter = 2001;
int transactionCounter = 3001;

Customer* findCustomerByID(const string& id) {
    auto it = customers.find(id);
    return it != customers.end() ? &it->second : nullptr;
}

Account* findAccountByID(const string& id) {
    auto it = accounts.find(id);
    return it != accounts.end() ? &it->second : nullptr;
}

string generateCustomerID() {
    return "CUST" + to_string(customerCounter++);
}

string generateAccountID() {
    return "ACC" + to_string(accountCounter++);
}

string generateTransactionID() {
    return "TXN" + to_string(transactionCounter++);
}

string getCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    string ts = ctime(&now_time);
    if (!ts.empty() && ts[ts.length()-1] == '\n') {
        ts.erase(ts.length()-1);
    }
    return ts;
}

bool validatePhone(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool validateEmail(const string& email) {
    return (email.find('@') != string::npos && email.find('.') != string::npos);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void createCustomer() {
    string name, phone, email;
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Phone (10 digits): ";
    getline(cin, phone);
    cout << "Enter Email: ";
    getline(cin, email);

    if (!validatePhone(phone)) {
        cout << "Error: Invalid phone number.\n";
        return;
    }
    if (!validateEmail(email)) {
        cout << "Error: Invalid email.\n";
        return;
    }

    for (const auto& pair : customers) {
        if (pair.second.phone == phone || pair.second.email == email) {
            cout << "Error: Phone or Email already exists.\n";
            return;
        }
    }

    string cID = generateCustomerID();
    customers[cID] = Customer(cID, name, phone, email);
    cout << "Customer created successfully! Customer ID: " << cID << "\n";
}

void createAccount() {
    string cID, type;
    int typeChoice;
    cout << "Enter Customer ID: ";
    getline(cin, cID);

    Customer* cust = findCustomerByID(cID);
    if (!cust) {
        cout << "Error: Customer not found.\n";
        return;
    }

    cout << "Select Account Type:\n1. Savings\n2. Current\nEnter choice: ";
    if (!(cin >> typeChoice)) {
        cout << "Error: Invalid input.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (typeChoice == 1) type = "Savings";
    else if (typeChoice == 2) type = "Current";
    else {
        cout << "Error: Invalid type choice.\n";
        return;
    }

    double initialBalance = (type == "Savings") ? 500.0 : 1000.0;
    cout << "Initial deposit required for " << type << " is $" << initialBalance << ".\nEnter amount: ";
    double amount;
    if (!(cin >> amount)) {
        cout << "Error: Invalid input.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount < initialBalance) {
        cout << "Error: Insufficient initial deposit.\n";
        return;
    }

    string aID = generateAccountID();
    accounts[aID] = Account(aID, cID, type, amount);
    cust->accountIDs.push_back(aID);

    string tID = generateTransactionID();
    accounts[aID].transactions.push_front(Transaction(tID, "", aID, amount, "Deposit", getCurrentTime(), "Success"));

    cout << "Account created successfully! Account ID: " << aID << "\n";
}

void depositMoney() {
    string aID;
    cout << "Enter Account ID: ";
    getline(cin, aID);

    Account* acc = findAccountByID(aID);
    if (!acc) {
        cout << "Error: Account not found.\n";
        return;
    }

    if (!acc->isActive) {
        cout << "Error: Account is inactive.\n";
        return;
    }

    cout << "Enter Amount: ";
    double amount;
    if (!(cin >> amount)) {
        cout << "Error: Invalid input.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount <= 0) {
        cout << "Error: Amount must be positive.\n";
        string tID = generateTransactionID();
        acc->transactions.push_front(Transaction(tID, "", aID, amount, "Deposit", getCurrentTime(), "Failed", "Negative or zero amount"));
        return;
    }

    acc->balance += amount;
    string tID = generateTransactionID();
    acc->transactions.push_front(Transaction(tID, "", aID, amount, "Deposit", getCurrentTime(), "Success"));
    cout << "Deposit successful! New Balance: $" << fixed << setprecision(2) << acc->balance << "\n";
}

void withdrawMoney() {
    string aID;
    cout << "Enter Account ID: ";
    getline(cin, aID);

    Account* acc = findAccountByID(aID);
    if (!acc) {
        cout << "Error: Account not found.\n";
        return;
    }

    if (!acc->isActive) {
        cout << "Error: Account is inactive.\n";
        return;
    }

    cout << "Enter Amount: ";
    double amount;
    if (!(cin >> amount)) {
        cout << "Error: Invalid input.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount <= 0) {
        cout << "Error: Amount must be positive.\n";
        string tID = generateTransactionID();
        acc->transactions.push_front(Transaction(tID, aID, "", amount, "Withdrawal", getCurrentTime(), "Failed", "Negative or zero amount"));
        return;
    }

    double minBalance = (acc->type == "Savings") ? 500.0 : 1000.0;
    if (acc->balance - amount < minBalance) {
        cout << "Error: Insufficient balance to maintain minimum of $" << minBalance << ".\n";
        string tID = generateTransactionID();
        acc->transactions.push_front(Transaction(tID, aID, "", amount, "Withdrawal", getCurrentTime(), "Failed", "Insufficient balance"));
        return;
    }

    acc->balance -= amount;
    string tID = generateTransactionID();
    acc->transactions.push_front(Transaction(tID, aID, "", amount, "Withdrawal", getCurrentTime(), "Success"));
    cout << "Withdrawal successful! New Balance: $" << fixed << setprecision(2) << acc->balance << "\n";
}

void fundTransfer() {
    string fromID, toID;
    cout << "Enter Source Account ID: ";
    getline(cin, fromID);
    cout << "Enter Destination Account ID: ";
    getline(cin, toID);

    if (fromID == toID) {
        cout << "Error: Cannot transfer to the same account.\n";
        return;
    }

    Account* fromAcc = findAccountByID(fromID);
    Account* toAcc = findAccountByID(toID);

    if (!fromAcc || !toAcc) {
        cout << "Error: One or both accounts not found.\n";
        return;
    }

    if (!fromAcc->isActive || !toAcc->isActive) {
        cout << "Error: One or both accounts are inactive.\n";
        return;
    }

    cout << "Enter Amount: ";
    double amount;
    if (!(cin >> amount)) {
        cout << "Error: Invalid input.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (amount <= 0) {
        cout << "Error: Amount must be positive.\n";
        string tID = generateTransactionID();
        fromAcc->transactions.push_front(Transaction(tID, fromID, toID, amount, "Transfer", getCurrentTime(), "Failed", "Negative or zero amount"));
        toAcc->transactions.push_front(Transaction(tID, fromID, toID, amount, "Transfer", getCurrentTime(), "Failed", "Negative or zero amount"));
        return;
    }

    double minBalanceFrom = (fromAcc->type == "Savings") ? 500.0 : 1000.0;
    if (fromAcc->balance - amount < minBalanceFrom) {
        cout << "Error: Source account has insufficient balance to maintain minimum.\n";
        string tID = generateTransactionID();
        fromAcc->transactions.push_front(Transaction(tID, fromID, toID, amount, "Transfer", getCurrentTime(), "Failed", "Insufficient balance"));
        toAcc->transactions.push_front(Transaction(tID, fromID, toID, amount, "Transfer", getCurrentTime(), "Failed", "Insufficient balance"));
        return;
    }

    fromAcc->balance -= amount;
    toAcc->balance += amount;

    string tID = generateTransactionID();
    fromAcc->transactions.push_front(Transaction(tID, fromID, toID, amount, "Transfer", getCurrentTime(), "Success"));
    toAcc->transactions.push_front(Transaction(tID, fromID, toID, amount, "Transfer", getCurrentTime(), "Success"));

    cout << "Transfer successful!\n";
}

void viewTransactionHistory() {
    string aID;
    cout << "Enter Account ID: ";
    getline(cin, aID);

    Account* acc = findAccountByID(aID);
    if (!acc) {
        cout << "Error: Account not found.\n";
        return;
    }

    cout << "Enter number of transactions to view (default 10): ";
    string input;
    getline(cin, input);
    int n = 10;
    if (!input.empty()) {
        try {
            n = stoi(input);
        } catch (...) {
            n = 10;
        }
    }

    if (n <= 0) n = 10;

    cout << "\n--- Transaction History ---\n";
    int count = 0;
    for (const auto& t : acc->transactions) {
        if (count >= n) break;
        cout << "ID: " << t.transactionID << " | Type: " << t.type << " | Amount: $" << fixed << setprecision(2) << t.amount 
             << " | Time: " << t.timestamp << " | Status: " << t.status;
        if (t.status == "Failed") cout << " (" << t.failureReason << ")";
        cout << "\n";
        count++;
    }
    cout << "---------------------------\n";
}

void displayCustomerInfo() {
    string cID;
    cout << "Enter Customer ID: ";
    getline(cin, cID);

    Customer* cust = findCustomerByID(cID);
    if (!cust) {
        cout << "Error: Customer not found.\n";
        return;
    }

    cout << "\n--- Customer Info ---\n";
    cout << "Name: " << cust->name << "\nPhone: " << cust->phone << "\nEmail: " << cust->email << "\n";
    cout << "Accounts:\n";

    for (const string& aID : cust->accountIDs) {
        Account* a = findAccountByID(aID);
        if (a) {
            cout << "  ID: " << a->accountID << " | Type: " << a->type << " | Balance: $" << fixed << setprecision(2) << a->balance 
                 << " | Status: " << (a->isActive ? "Active" : "Inactive") << "\n";
            cout << "  Recent Transactions:\n";
            int count = 0;
            for (const auto& t : a->transactions) {
                if (count >= 5) break;
                cout << "    " << t.transactionID << " | " << t.type << " | $" << t.amount << " | " << t.status << "\n";
                count++;
            }
        }
    }
    cout << "---------------------\n";
}

void showMenu() {
    cout << "\n=== BANKING SYSTEM ===\n";
    cout << "1. Create Customer\n";
    cout << "2. Create Account\n";
    cout << "3. Deposit Money\n";
    cout << "4. Withdraw Money\n";
    cout << "5. Fund Transfer\n";
    cout << "6. View Transaction History\n";
    cout << "7. Display Customer Info\n";
    cout << "8. Exit\n";
    cout << "Select an option: ";
}

int main() {
    while (true) {
        showMenu();
        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Try again.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: createCustomer(); break;
            case 2: createAccount(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: fundTransfer(); break;
            case 6: viewTransactionHistory(); break;
            case 7: displayCustomerInfo(); break;
            case 8: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}
