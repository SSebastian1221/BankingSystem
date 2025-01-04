#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <mysql.h>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

const char* DB_HOST = std::getenv("DB_HOST");
const char* DB_USER = std::getenv("DB_USER");
const char* DB_PASSWORD = std::getenv("DB_PASSWORD");
const char* DB_NAME = std::getenv("DB_NAME");

// Utility function to execute queries and return results
vector<vector<string>> executeQuery(MYSQL* conn, const string& query) {
    vector<vector<string>> result;

    if (mysql_query(conn, query.c_str())) {
        cerr << "Query error: " << mysql_error(conn) << endl;
        return result;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            vector<string> rowData;
            for (unsigned int i = 0; i < mysql_num_fields(res); i++) {
                rowData.push_back(row[i] ? row[i] : "NULL");
            }
            result.push_back(rowData);
        }
        mysql_free_result(res);
    }

    return result;
}

// Function to connect to the database
MYSQL* connectToDatabase() {
    MYSQL* conn = mysql_init(nullptr);
    if (conn == nullptr) {
        cerr << "mysql_init() failed" << endl;
        return nullptr;
    }

    // Check for missing environment variables
    if (!DB_HOST || !DB_USER || !DB_PASSWORD || !DB_NAME) {
        cerr << "Missing one or more required environment variables." << endl;
        return nullptr;
    }

    // Connect to database
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, 0, nullptr, 0) == nullptr) {
        cerr << "Connection error: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return nullptr;
    }

    cout << "Connected to database successfully." << endl;
    return conn;
}

// Function to register a new user
bool registerUser(MYSQL* conn) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    stringstream query;
    query << "INSERT INTO users (username, password) VALUES ('" << username << "', '" << password << "')";
    if (mysql_query(conn, query.str().c_str())) {
        cerr << "Error registering user: " << mysql_error(conn) << endl;
        return false;
    }
    cout << "User registered successfully." << endl;
    return true;
}

// Function to log in a user
int loginUser(MYSQL* conn) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    stringstream query;
    query << "SELECT id FROM users WHERE username = '" << username << "' AND password = '" << password << "'";
    auto result = executeQuery(conn, query.str());

    if (!result.empty() && !result[0].empty()) {
        cout << "Login successful." << endl;
        return stoi(result[0][0]); // Return the user ID
    } else {
        cout << "Invalid username or password." << endl;
        return -1; // Return -1 for failed login
    }
}

// Function to create account
void createAccount(MYSQL* conn, int userId) {
    stringstream checkQuery;
    checkQuery << "SELECT id FROM accounts WHERE user_id = " << userId;
    auto checkResult = executeQuery(conn, checkQuery.str());
    if (!checkResult.empty()) {
        cout << "User already has an account." << endl;
        return;
    }

    int accountType = 0; // Only checking account for now
    double initialBalance;
    cout << "Enter initial balance: $";
    cin >> initialBalance;
    while (cin.fail() || initialBalance < 0) {
        cout << "Invalid input. Please enter a non-negative number: $";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> initialBalance;
    }

    stringstream query;
    query << "INSERT INTO accounts (user_id, account_type, balance) VALUES (" 
          << userId << ", " << accountType << ", " 
          << fixed << setprecision(2) << initialBalance << ")";
    
    if (mysql_query(conn, query.str().c_str())) {
        cerr << "Error creating account: " << mysql_error(conn) << endl;
    } else {
        cout << "Account created successfully." << endl;
    }
}

// Function to deposit money
void deposit(MYSQL* conn, int userId) {
    stringstream query;
    query << "SELECT id, balance FROM accounts WHERE user_id = " << userId;
    auto result = executeQuery(conn, query.str());
    
    if (result.empty()) {
        cout << "No account found for this user." << endl;
        return;
    }

    int accountId = stoi(result[0][0]);
    double currentBalance = stod(result[0][1]);
    double amount;

    cout << "Enter amount to deposit: $";
    cin >> amount;
    while (cin.fail() || amount <= 0) {
        cout << "Invalid input. Please enter a positive number: $";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> amount;
    }

    double newBalance = currentBalance + amount;
    stringstream updateQuery;
    updateQuery << "UPDATE accounts SET balance = " << fixed << setprecision(2) 
                << newBalance << " WHERE id = " << accountId;

    if (mysql_query(conn, updateQuery.str().c_str())) {
        cerr << "Error making deposit: " << mysql_error(conn) << endl;
    } else {
        cout << "Deposit successful. New balance: $" << newBalance << endl;
    }
}

// Function to withdraw money
void withdraw(MYSQL* conn, int userId) {
    stringstream query;
    query << "SELECT id, balance FROM accounts WHERE user_id = " << userId;
    auto result = executeQuery(conn, query.str());
    
    if (result.empty()) {
        cout << "No account found for this user." << endl;
        return;
    }

    int accountId = stoi(result[0][0]);
    double currentBalance = stod(result[0][1]);
    double amount;

    cout << "Enter amount to withdraw: $";
    cin >> amount;
    while (cin.fail() || amount <= 0) {
        cout << "Invalid input. Please enter a positive number: $";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> amount;
    }

    if (amount > currentBalance) {
        cout << "Insufficient funds." << endl;
        return;
    }

    double newBalance = currentBalance - amount;
    stringstream updateQuery;
    updateQuery << "UPDATE accounts SET balance = " << fixed << setprecision(2) 
                << newBalance << " WHERE id = " << accountId;

    if (mysql_query(conn, updateQuery.str().c_str())) {
        cerr << "Error making withdrawal: " << mysql_error(conn) << endl;
    } else {
        cout << "Withdrawal successful. New balance: $" << newBalance << endl;
    }
}

// Function to display balance
void displayBalance(MYSQL* conn, int userId) {
    stringstream query;
    query << "SELECT balance FROM accounts WHERE user_id = " << userId;
    auto result = executeQuery(conn, query.str());
    
    if (result.empty()) {
        cout << "No account found for this user." << endl;
        return;
    }

    cout << "Current balance: $" << fixed << setprecision(2) << stod(result[0][0]) << endl;
}

int main() {
    MYSQL* conn = connectToDatabase();
    if (conn == nullptr) {
        return 1;
    }

    int choice;
    int currentUserId = -1; // -1 means no user is logged in

    do {
        cout << "\nBank Account Management System\n";
        if (currentUserId == -1) { // If no user is logged in
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "0. Exit\n";
        } else { // If a user is logged in
            cout << "3. Create Account\n";
            cout << "4. Deposit\n";
            cout << "5. Withdraw\n";
            cout << "6. Display Balance\n";
            cout << "7. Logout\n";
            cout << "0. Exit\n";
        }

        cout << "Enter your choice: ";
        cin >> choice;
        while (cin.fail()) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> choice;
        }

        switch (choice) {
            case 1:
                if (currentUserId == -1) registerUser(conn);
                break;
            case 2:
                if (currentUserId == -1) currentUserId = loginUser(conn);
                break;
            case 3:
                if (currentUserId != -1) createAccount(conn, currentUserId);
                break;
            case 4:
                if (currentUserId != -1) deposit(conn, currentUserId);
                break;
            case 5:
                if (currentUserId != -1) withdraw(conn, currentUserId);
                break;
            case 6:
                if (currentUserId != -1) displayBalance(conn, currentUserId);
                break;
            case 7:
                if (currentUserId != -1) {
                    currentUserId = -1;
                    cout << "Logged out successfully.\n";
                }
                break;
            case 0:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    mysql_close(conn);
    return 0;
}