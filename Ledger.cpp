#include "Ledger.h"
#include <algorithm>
#include <iostream>

Ledger::Ledger()
    : balance(0.0)
{
}

void Ledger::addTransaction(const Transaction &transaction) {
    transactions.push_back(transaction);
    if (transaction.isIncomeTransaction()) {
        balance += transaction.getAmount();
    } else {
        balance -= transaction.getAmount();
    }
}

bool Ledger::removeTransaction(int transactionId) {
    auto it = std::find_if(transactions.begin(), transactions.end(),
                           [transactionId](const Transaction &t) { return t.getId() == transactionId; });

    if (it != transactions.end()) {
        // Update balance before removing
        if (it->isIncomeTransaction()) {
            balance -= it->getAmount();
        } else {
            balance += it->getAmount();
        }
        transactions.erase(it);
        return true;
    }
    return false;
}

double Ledger::getBalance() const {
    return balance;
}

std::vector<Transaction> Ledger::getAllTransactions() const {
    return transactions;
}

void Ledger::printAllTransactions() const {
    for (const auto &t : transactions) {
        std::cout << t.toString() << std::endl;
    }
}

void Ledger::clear() {
    transactions.clear();
    balance = 0.0;
}
