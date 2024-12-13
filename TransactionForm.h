#ifndef TRANSACTIONFORM_H
#define TRANSACTIONFORM_H

#include <QWidget>
#include "User.h"

namespace Ui {
class TransactionForm;
}

/**
 * @brief The TransactionForm class provides a UI for adding a new transaction.
 */
class TransactionForm : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the TransactionForm widget.
     * @param parent The parent widget.
     */
    explicit TransactionForm(QWidget *parent = nullptr);

    /**
     * @brief Destructs the TransactionForm widget.
     */
    ~TransactionForm();

    /**
     * @brief Sets the current user.
     * @param user The current user.
     */
    void setCurrentUser(const User &user);

    /**
     * @brief Resets all UI elements to their default state.
     */
    void resetUI();

signals:
    /**
     * @brief Emitted when a transaction is successfully saved.
     */
    void transactionSaved();

    /**
     * @brief Emitted when the transaction addition is cancelled.
     */
    void transactionCancelled();

private slots:
    /**
     * @brief Saves the transaction after validating input.
     */
    void saveTransaction();

    /**
     * @brief Cancels the transaction addition process.
     */
    void cancelTransaction();

private:
    Ui::TransactionForm *ui; ///< Pointer to the UI components of TransactionForm.
    User currentUser; ///< The current user adding the transaction.

    /**
     * @brief Validates the input fields for the transaction form.
     * @return true if all inputs are valid, false otherwise.
     */
    bool validateTransactionInput();
};

#endif // TRANSACTIONFORM_H
