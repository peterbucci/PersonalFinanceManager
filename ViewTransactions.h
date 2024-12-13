#ifndef VIEWTRANSACTIONS_H
#define VIEWTRANSACTIONS_H

#include <QWidget>
#include <vector>
#include "Transaction.h"
#include "User.h"

namespace Ui {
class ViewTransactions;
}

/**
 * @brief The ViewTransactions class provides a UI for viewing and filtering user transactions.
 */
class ViewTransactions : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the ViewTransactions widget.
     * @param parent The parent widget.
     */
    explicit ViewTransactions(QWidget *parent = nullptr);

    /**
     * @brief Destructs the ViewTransactions widget.
     */
    ~ViewTransactions();

    /**
     * @brief Sets the current user.
     * @param user Current user.
     */
    void setCurrentUser(const User &user);

    /**
     * @brief Sets all transactions for this user.
     * @param transactions Vector of all user transactions.
     */
    void setAllTransactions(const std::vector<Transaction> &transactions);

    /**
     * @brief Resets all UI elements to their default state.
     */
    void resetUI();

protected:
    /**
     * @brief Overrides the event filter to handle specific events.
     * @param obj The object where the event originated.
     * @param event The event to be filtered.
     * @return true if the event is handled, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    /**
     * @brief Toggles the visibility of the options group box.
     */
    void toggleOptions();

    /**
     * @brief Updates the transaction table based on filters (category/subcategory).
     */
    void updateFilters();

private:
    Ui::ViewTransactions *ui; ///< Pointer to the UI components of ViewTransactions.
    User currentUser; ///< The current user whose transactions are being viewed.
    std::vector<Transaction> allTransactions; ///< All transactions associated with the current user.
    QString currentCategoryFilter; ///< Current category filter applied to the transactions.
    QString currentSubCategoryFilter; ///< Current subcategory filter applied to the transactions.

    /**
     * @brief Apply category/subcategory filtering and populate the transaction table.
     */
    void applyFiltering();

    /**
     * @brief Populate the transactionTableWidget based on given transactions and display mode.
     * @param transactions The filtered transactions.
     * @param showBalance If true, show balance column and no TOTAL row.
     * @param showTotalRow If true, shows TOTAL row at bottom (when no balance).
     */
    void populateViewTable(const std::vector<Transaction> &transactions, bool showBalance, bool showTotalRow);
};

#endif // VIEWTRANSACTIONS_H
