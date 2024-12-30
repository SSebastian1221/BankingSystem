#ifndef BANK_HPP
#define BANK_HPP
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

struct Account {
    float balance;
    vector <float> transactions;
};

class Bank{

public:
    Bank();
    ~Bank();
    void Menu();
    void Create_Account ();
    void deposit(Account& account);
    void Withdraw(Account& account);
    void Check_balance(Account& account);
    void transactions(Account& account);
    void Set_Name(const string& X, const string& Y);
    void getAccount() const;
   // void setName(const string& x);
    string getName() const;



private:
    unordered_map<string, string> User_account;
    ofstream transaction_file;
    string name;
    string password;
    Account checking;
    Account savings;

};

#endif
