#ifndef LEDGER_H
#define LEDGER_H

#include <vector>
#include "Transaction.h"

/**
 * @brief The Ledger class manages a collection of financial transactions for a specific user and tracks the running balance.
 *
 * The Ledger class allows adding and removing transactions, retrieving all transactions,
 * calculating the current balance based on incomes and expenses, and providing utility
 * functions to display transaction details.
 */
class Ledger {
public:
    /**
     * @brief Default constructor initializes an empty ledger with a zero balance.
     */
    Ledger();

    /**
     * @brief Adds a new transaction to the ledger and updates the running balance.
     *
     * @param transaction The Transaction object to be added.
     */
    void addTransaction(const Transaction &transaction);

    /**
     * @brief Removes a transaction from the ledger by its unique ID and updates the balance.
     *
     * @param transactionId The unique identifier of the transaction to be removed.
     * @return `true` if the transaction was found and removed successfully, `false` otherwise.
     */
    bool removeTransaction(int transactionId);

    /**
     * @brief Retrieves the current running balance of the ledger.
     *
     * @return The current balance as a double.
     */
    double getBalance() const;

    /**
     * @brief Retrieves all transactions stored in the ledger.
     *
     * @return A vector containing all Transaction objects.
     */
    std::vector<Transaction> getAllTransactions() const;

    /**
     * @brief Prints all transactions in the ledger (for debugging).
     */
    void printAllTransactions() const;

    /**
     * @brief Clears all transactions in the ledger and resets balance to zero.
     */
    void clear();



private:
    std::vector<Transaction> transactions; // Collection of all transactions in the ledger.
    double balance;                        // Running balance of the ledger.
};

#endif // LEDGER_H
