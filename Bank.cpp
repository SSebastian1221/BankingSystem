#include <iostream>
#include <cstdlib>
#include "Bank.hpp"
using namespace std;



Bank::Bank() {
    checking = {0,{}};
    savings = {0,{}};
    transaction_file.open("transaction_log.txt",ios::app);
    Load_Accounts();

}

Bank::~Bank() {

    Save_Accounts();
    if(transaction_file.is_open()){
        transaction_file.close();
    }
}


void Bank::Menu(){

    int user_input;

    cout << "Welcome to Bank Suarez: " << endl;
    cout << "Please choose a option: 1)Login 2)Create Account 3) Exit" << endl;
    cin >> user_input;

    switch (user_input) {
        case 1:
            Login();
            break;
        case 2:
            Create_Account();
            break;
        case 3:
            exit(0);
            break;
        default:
            cout << "Invalid Choice" << endl;
            break;
    }
}



void Bank::Load_Accounts() {

    ifstream infile("accounts.txt");
    if (!infile.is_open()) {
        cout << "Unable to open file" << endl;
        return;
    }

    string username, password;

    while (infile >> username >> password) {
        User_account[username] = password;
    }
    infile.close();
}


void Bank::Save_Accounts() const {
    ofstream outfile("accounts.txt");
    if (!outfile.is_open()) {
        cout << "Unable to open file" << endl;
        return;
    }

    for (const auto& account : User_account) {
        outfile << account.first << " " << account.second << endl;
    }
    outfile.close();
}



void Bank::Login() {

    string username, password;

    cout << "Please enter your Username: ";
    cin >> username;

        cout << "Please enter your Password: ";
        cin >> password;

        if (User_account[username] == password) {
            cout << "Login succeeded" << endl;
            Bank_Account();
        } else {
            cout << "Login failed" << endl;
            Menu();
        }

    Bank_Account();
}



void Bank::Bank_Account() {

    int user_input;
    int account_choice;

    while(true) {
        cout << "Choose an account: 1) Checking 2) Savings 0) Exit" << endl;
        cin >> account_choice;

        if(account_choice == 0){
            cout << "Have a great day! " << getName() << "!" << endl;
            exit(0);
        }


        Account* active_account = (account_choice == 1) ? &checking : &savings;

        cout << "Welcome back! " << getName() << endl;
        cout << "What are you looking to do today? " << endl;
        cout << "1: Deposit " << endl;
        cout << "2: Withdraw " << endl;
        cout << "3: Check Balance" << endl;
        cout << "0: Exit Bank" << endl;
        cin >> user_input;

        switch (user_input) {
            case 1:
                deposit(*active_account);
            break;

            case 2:
                Withdraw(*active_account);
            break;

            case 3:
                Check_balance(*active_account);
            break;

            case 0:
                cout << "Have a great day, see you soon!" << endl;
            exit(0);
            break;

            default:
                cout << "Not a vaild input! " << endl;
            break;
        }
    }
}



void Bank::Create_Account(){

    string User_Name;
    string Password;

    cout << "Enter your User name: ";
    cin >> User_Name;
    cout << "Enter your password: ";
    cin >> Password;

    User_account[User_Name] = Password;

    ofstream outfile("accounts.txt", ios::app);
    if (outfile.is_open()) {
        outfile <<User_Name << " " << Password << endl;
        outfile.close();
    }

    cout << "Created account successful! " << endl;

    Bank_Account();
}



void Bank::deposit(Account(& account)) {

    float money;

    cout << "Enter the amount of money to add: ";
    cin >> money;

    if(money > 0){
        account.transactions.push_back(money);
        account.balance += money;
        if(transaction_file.is_open())
            transaction_file << "Deposit: +" << money << " | New Balance: $" << account.balance << endl;
        cout << "Deposited $" << money << ". New Balance: " << account.balance << endl;
    }else{
        cout << " Invalid amount! " << endl;
    }
}



void Bank::Withdraw(Account& account){

    float amount;

    cout << "Enter the amount you want to withdraw: " << endl;
    cout << "Enter 00 to exit" << endl;
    cin >> amount;

    if(amount <= account.balance){
        account.transactions.push_back(-amount);
        account.balance -= amount;

        if(transaction_file.is_open())
            transaction_file << "Withdraw: -" << amount << " |New Balance: $" << account.balance << endl;
        cout << "Withdrew: $" << amount << endl;
        cout << "New balance: $" << account.balance << endl;
    } else if (amount > account.balance){
        cout << "Insufficient funds! " << endl;
    }else{
        cout << "Invalid amount! " << endl;
    }
}



void Bank::Check_balance(Account& account) {
    cout << "Your current balance is: $" << account.balance << endl;
}


void Bank::transactions(Account& account) {

    cout << "Your bank transactions are: " << account.balance << endl;
}


string Bank::getName() const{
    return name;
}

