#include<iostream>
#include<fstream>
#include<string>
#include<map>
using namespace std;

//exception thrown if min balence is reached
#define MIN_BALANCE 500 

class InsufficientFunds{
    public:
        void show(){
            cout<<"Not enough balance in account"<<endl;
        }
};//exception class

class Account{
    private:
        long long accountNumber;
        string firstName;
        string lastName;
        long double balance;
        static long long nextAccoutNumber;
    public:
        Account(string firstName,string lastName,long double balance);
        //inlined
        Account(){};
        string getFirstName(){return firstName;};
        string getLastName(){return lastName;};
        long long getAccountNumber(){return accountNumber;};
        long double getBalance(){return balance;};
        //seperrately declared
        void diposit(long double amount);
        void withdrawl(long double amount);
        static void setLastAccountNumber(long long accountNumber);
        static long long getLastAccountNumber();
        //operator overloading
        friend ofstream & operator << (ofstream &ofs,Account &acc);
        friend ifstream & operator >> (ifstream &ifs,Account &acc);
        friend ostream & operator << (ostream &os,Account &acc);
};
long long Account::nextAccoutNumber=0;
class Bank{
    private:
        map<long long,Account> accounts; //mapping account number to acccount
    public:
        Bank();
        Account openAccount(string firstName,string lastName,long double balance);
        Account balanceEnquery(long long accountNumber);
        Account diposit(long long accountNumber,long double amount);
        Account withdrawl(long long accountNumber,long double amount);
        void closeAccount(long long accountNumber);
        void showAllAccount();
        ~Bank();
};
int main(){
    Bank b;
    Account acc;
    int choice;
    string fname,lname;
    long long accountNumber;
    long double balance;
    long double amount;

    cout<<"**Basic Banking System**"<<endl;
    do{
        cout<<"\n\tSelect one option below ";
        cout<<"\n\t1 Open an Account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit";
        cout<<"\n\t4 Withdrawal";
        cout<<"\n\t5 Close an Account";
        cout<<"\n\t6 Show All Accounts";
        cout<<"\n\t7 Quit";
        cout<<"\nEnter your choice: ";
        cin>>choice;
        switch(choice){
            case 1:
                cout<<"Enter First Name: ";
                cin>>fname;
                cout<<"Enter Last Name: ";
                cin>>lname;
                cout<<"Enter initil Balance: ";
                cin>>balance;
                acc=b.openAccount(fname,lname,balance);
                cout<<endl<<"Congradulation Account is Created"<<endl;
                cout<<acc;
                break;
            case 2:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                acc=b.balanceEnquery(accountNumber);
                cout<<endl<<"Your Account Details"<<endl;
                cout<<acc;
                break;
            case 3:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                cout<<"Enter Balance:";
                cin>>amount;
                acc=b.diposit(accountNumber, amount);
                cout<<endl<<"Amount is Deposited"<<endl;
                cout<<acc;
                break;
            case 4:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                cout<<"Enter Balance:";
                cin>>amount;
                acc=b.withdrawl(accountNumber, amount);
                cout<<endl<<"Amount Withdrawn"<<endl;
                cout<<acc;
                break;
            case 5:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                b.closeAccount(accountNumber);
                break;
            case 6:
                b.showAllAccount();
                break;
            case 7:
                cout<<"Thanks for using the banking system"<<endl;
                break;
            default:
                cout<<"\nEnter corret choice";
            exit(0);
        }
    }while(choice!=7);
    return 0;
}
//Accounts functions definitions
Account::Account(string firstName,string lastName,long double balance){
    /*
        initializes the account with name and balance 
        and nextAccountNumber incremented by one
        and added as new account number for this account
    */
    nextAccoutNumber++;
    this->accountNumber=nextAccoutNumber;
    this->firstName=firstName;
    this->lastName=lastName;
    if(balance<MIN_BALANCE)
        throw InsufficientFunds();
    this->balance=balance;
}
void Account::diposit(long double amount){
    balance+=amount;
}
void Account::withdrawl(long double amount){
    if(balance-amount<MIN_BALANCE)
        throw InsufficientFunds();
    balance-=amount;
}
void Account::setLastAccountNumber(long long accountNumber){
    nextAccoutNumber=accountNumber;
}
long long Account::getLastAccountNumber(){
    return nextAccoutNumber;
}
//definning 3 operator overloading friend functions
ofstream & operator << (ofstream &ofs,Account &acc){
    ofs<<acc.accountNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastName<<endl;
    ofs<<acc.balance<<endl;
    return ofs;
}
ifstream & operator >> (ifstream &ifs,Account &acc){
    ifs>>acc.accountNumber>>acc.firstName>>acc.lastName>>acc.balance;
    return ifs;
}
//serializing
ostream & operator << (ostream &os,Account &acc){
    os<<"First Name: "<<acc.firstName<<endl;
    os<<"Last Name: "<<acc.lastName<<endl;
    os<<"Account Number: "<<acc.accountNumber<<endl;
    os<<"Balance: "<<acc.balance<<endl;
}
//bank functions implementations
Bank::Bank(){
    /*
        1.at the time of calling the constructor it grabs all the account 
        details from a file, and store them in a map named "accounts"
        2.if it is the first time thenthe targeted text file not present
        then simply returned from there
        3.it sets the last account number, upto which it is loaded,so when ever
        the program loads the last account number remains updated
    */
    Account account;
    ifstream ifs;
    ifs.open("accountDetails.txt");
    if(!ifs.is_open()){
        return;
    }
    while(!ifs.eof()){
        ifs>>account;
        accounts.insert(pair<long long,Account>(account.getAccountNumber(),account));
    }
    Account::setLastAccountNumber(account.getAccountNumber());
    ifs.close();
}
Account Bank::openAccount(string firstName,string lastName,long double balance){
    /*
        rewritting entire text file,again with new entry
        adding to "accounts" map
        returning the added "account" to do print out
    */
    try{
        Account account(firstName,lastName,balance);//<-exception area
        accounts.insert(pair<long long,Account>(account.getAccountNumber(),account));
        ofstream ofs;
        ofs.open("accountDetails.txt",ios::trunc);
        map<long long,Account>::iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++){
            ofs<<itr->second;
        }
        ofs.close();
        return account;
    }
    catch(InsufficientFunds e){
        e.show();
    }
}
Account Bank::balanceEnquery(long long accountNumber){
    map<long long,Account>::iterator itr=accounts.find(accountNumber);
    return itr->second;
}
Account Bank::diposit(long long accountNumber,long double amount){
    map<long long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.diposit(amount);
    return itr->second;
}
Account Bank::withdrawl(long long accountNumber,long double amount){
    map<long long,Account>::iterator itr=accounts.find(accountNumber);
    try{
        itr->second.withdrawl(amount);//<-exception area
        return itr->second;
    }
    catch(InsufficientFunds e){
        e.show();
    }
}
void Bank::closeAccount(long long accountNumber){
    map<long long,Account>::iterator itr=accounts.find(accountNumber);
    cout<<"Account Deleted: "<<endl<<itr->second;
    accounts.erase(accountNumber);
}
void Bank::showAllAccount(){
    map<long long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        cout<<"Account: "<<endl<<itr->first<<endl<<itr->second<<endl;
    }
}
Bank::~Bank(){
    /*
        storing all the accounts data(updated) to a file again
    */
    ofstream ofs;
    ofs.open("accountDetails.txt",ios::trunc);
    map<long long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        ofs<<itr->second;
    }
    ofs.close();
}