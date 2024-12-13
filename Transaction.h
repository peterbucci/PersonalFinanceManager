#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

/**
 * @brief The Transaction class represents a single financial transaction,
 * either income or expense, with associated details, including optional tax withholding.
 */
class Transaction {
public:
    /**
     * @brief Default constructor initializes a Transaction with default values.
     * Sets all numerical values to zero and strings to empty.
     */
    Transaction();

    /**
     * @brief Constructs a Transaction with specified details.
     * @param id The unique identifier for the transaction.
     * @param userId The identifier of the user associated with the transaction.
     * @param date The date of the transaction in "YYYY-MM-DD" format.
     * @param category The main category of the transaction (e.g., "Salary", "Groceries").
     * @param subcategory The subcategory of the transaction (e.g., "Bonus", "Vegetables").
     * @param amount The monetary amount of the transaction.
     * @param type The type of transaction, either "Income" or "Expense".
     * @param taxWithheld Indicates whether tax was withheld for this transaction.
     * @param taxAmount The amount of tax withheld, if any.
     */
    Transaction(int id, int userId, const std::string &date, const std::string &category,
                const std::string &subcategory, double amount, const std::string &type,
                bool taxWithheld = false, double taxAmount = 0.0);

    // Getters

    /**
     * @brief Retrieves the transaction's unique identifier.
     * @return The transaction ID as an integer.
     */
    int getId() const;

    /**
     * @brief Retrieves the user ID associated with the transaction.
     * @return The user ID as an integer.
     */
    int getUserId() const;

    /**
     * @brief Retrieves the date of the transaction.
     * @return The transaction date as a std::string in "YYYY-MM-DD" format.
     */
    std::string getDate() const;

    /**
     * @brief Retrieves the category of the transaction.
     * @return The transaction category as a std::string.
     */
    std::string getCategory() const;

    /**
     * @brief Retrieves the subcategory of the transaction.
     * @return The transaction subcategory as a std::string.
     */
    std::string getSubcategory() const;

    /**
     * @brief Retrieves the amount of the transaction.
     * @return The transaction amount as a double.
     */
    double getAmount() const;

    /**
     * @brief Retrieves the type of the transaction.
     * @return The transaction type as a std::string ("Income" or "Expense").
     */
    std::string getType() const;

    /**
     * @brief Checks if the transaction is an income transaction.
     * @return `true` if the transaction type is "Income", `false` otherwise.
     */
    bool isIncomeTransaction() const;

    /**
     * @brief Checks if tax was withheld for the transaction.
     * @return `true` if tax was withheld, `false` otherwise.
     */
    bool isTaxWithheld() const;

    /**
     * @brief Retrieves the amount of tax withheld for the transaction.
     * @return The tax amount as a double.
     */
    double getTaxAmount() const;

    // Setters

    /**
     * @brief Sets the transaction's unique identifier.
     * @param id The new transaction ID.
     */
    void setId(int id);

    /**
     * @brief Sets the user ID associated with the transaction.
     * @param userId The new user ID.
     */
    void setUserId(int userId);

    /**
     * @brief Sets the date of the transaction.
     * @param date The new transaction date in "YYYY-MM-DD" format.
     */
    void setDate(const std::string &date);

    /**
     * @brief Sets the category of the transaction.
     * @param category The new transaction category.
     */
    void setCategory(const std::string &category);

    /**
     * @brief Sets the subcategory of the transaction.
     * @param subcategory The new transaction subcategory.
     */
    void setSubcategory(const std::string &subcategory);

    /**
     * @brief Sets the amount of the transaction.
     * @param amount The new transaction amount.
     */
    void setAmount(double amount);

    /**
     * @brief Sets the type of the transaction.
     * @param type The new transaction type ("Income" or "Expense").
     */
    void setType(const std::string &type);

    /**
     * @brief Sets whether tax was withheld for the transaction.
     * @param withheld `true` if tax is withheld, `false` otherwise.
     */
    void setTaxWithheld(bool withheld);

    /**
     * @brief Sets the amount of tax withheld for the transaction.
     * @param amount The new tax amount.
     */
    void setTaxAmount(double amount);

    /**
     * @brief Calculates the net amount after applying tax withholding.
     *
     * For income transactions where tax is withheld, this method calculates the net amount
     * by subtracting the tax percentage from the gross amount.
     *
     * @return The net amount after tax withholding if applicable; otherwise, the original amount.
     */
    double calculateNetAmount() const;

    /**
     * @brief Converts the transaction details to a readable string format.
     * @return A std::string representing the transaction details.
     */
    std::string toString() const;

    // DB Methods

    /**
     * @brief Reads all transactions from the database.
     * @return A vector containing all Transaction objects retrieved from the database.
     */
    static std::vector<Transaction> readAllTransactions();

    /**
     * @brief Writes a new transaction to the database.
     * @param transaction The Transaction object to be written to the database.
     * @return `true` if the transaction was successfully written, `false` otherwise.
     */
    static bool writeTransaction(const Transaction &transaction);

private:
    int id; ///< Unique identifier for the transaction.
    int userId; ///< Identifier of the user associated with the transaction.
    std::string date; ///< Date of the transaction in "YYYY-MM-DD" format.
    std::string category; ///< Main category of the transaction.
    std::string subcategory; ///< Subcategory of the transaction.
    double amount; ///< Monetary amount of the transaction.
    std::string type; ///< Type of the transaction ("Income" or "Expense").
    bool taxWithheld; ///< Indicates whether tax was withheld for this transaction.
    double taxAmount; ///< Amount of tax withheld, if any.
};

#endif // TRANSACTION_H
