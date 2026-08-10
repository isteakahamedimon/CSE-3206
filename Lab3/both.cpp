#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// ==========================================
// FLYWEIGHT PATTERN COMPONENTS
// ==========================================

// 1. Flyweight: Shared intrinsic data (e.g., limits, tier names)
class AccountTier {
private:
    string tierName;
    int dailyLimit;
    
public:
    AccountTier(string name, int limit) : tierName(name), dailyLimit(limit) {}
    
    int getLimit() const { return dailyLimit; }
    string getName() const { return tierName; }
};

// 2. Flyweight Factory: Ensures we don't duplicate Tier data
class TierFactory {
private:
    unordered_map<string, AccountTier*> tiers;
    
public:
    AccountTier* getTier(const string& tierName) {
        if (tiers.find(tierName) == tiers.end()) {
            if (tierName == "Premium") {
                tiers[tierName] = new AccountTier(tierName, 20000);
            } else {
                tiers[tierName] = new AccountTier("Standard", 5000);
            }
        }
        return tiers[tierName];
    }
    
    ~TierFactory() {
        for (auto pair : tiers) delete pair.second;
    }
};

// ==========================================
// PROXY PATTERN (From PDF) + FLYWEIGHT
// ==========================================

// 3. Real Object: Modified to accept the Flyweight
class BankAccount {
private:
    int balance;
    AccountTier* tier; // Flyweight integration

public:
    // Initialized identically to the PDF logic, but with the Flyweight added
    BankAccount(int b, AccountTier* t) {
        balance = b;
        tier = t;
    }

    void checkBalance() {
        cout << "[" << tier->getName() << "] Balance: " << balance << endl;
    }

    void withdraw(int amount) {
        // Flyweight provides the daily limit constraint
        if (amount > tier->getLimit()) {
            cout << "Exceeds daily limit of " << tier->getLimit() << endl;
        } 
        else if (amount <= balance) {
            balance = balance - amount;
            cout << "Withdraw successful" << endl;
            cout << "Remaining Balance: " << balance << endl;
        } 
        else {
            cout << "Insufficient Balance" << endl;
        }
    }
};

// 4. Proxy: Retaining the exact logic from the PDF
class BankAccountProxy {
private:
    int pin;
    BankAccount account; // Eager initialization via composition (from PDF)

public:
    // Retaining the PDF's hardcoded 50000 initialization
    BankAccountProxy(int p, AccountTier* t) : account(50000, t) {
        pin = p;
    }

    void checkBalance() {
        // Retaining the PDF's hardcoded PIN check
        if (pin == 1234) {
            account.checkBalance();
        } else {
            cout << "Access Denied" << endl;
        }
    }

    void withdraw(int amount) {
        // Retaining the PDF's hardcoded PIN check
        if (pin == 1234) {
            account.withdraw(amount);
        } else {
            cout << "Access Denied" << endl;
        }
    }
};

// ==========================================
// CLIENT CODE
// ==========================================
int main() {
    TierFactory factory;
    
    // Fetch shared Flyweight data
    AccountTier* premiumTier = factory.getTier("Premium");
    AccountTier* standardTier = factory.getTier("Standard");

    cout << "--- Case 1: Correct PIN, Premium Tier ---" << endl;
    BankAccountProxy premiumAccount(1234, premiumTier);
    premiumAccount.checkBalance();
    premiumAccount.withdraw(10000); // Allowed by Premium limit (20000)

    cout << "\n--- Case 2: Correct PIN, Standard Tier ---" << endl;
    BankAccountProxy standardAccount(1234, standardTier);
    standardAccount.withdraw(10000); // Blocked by Standard limit (5000)

    cout << "\n--- Case 3: Wrong PIN ---" << endl;
    BankAccountProxy unauthorizedAccount(1111, premiumTier);
    unauthorizedAccount.checkBalance(); // Blocked by PDF's Access Denied logic
    unauthorizedAccount.withdraw(10000);

    return 0;
}
