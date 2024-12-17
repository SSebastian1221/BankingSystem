#ifndef BANK_HPP
#define BANK_HPP
#include <string>
#include <vector>
#include <fstream>

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
        void setName(const string& x);
        string getName() const;



    private:
      ofstream transaction_file;
      string name;
      Account checking;
      Account savings;

};

#endif