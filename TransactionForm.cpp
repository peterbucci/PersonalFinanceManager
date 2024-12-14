#include "TransactionForm.h"
#include "ui_TransactionForm.h"
#include <QMessageBox>
#include <QDebug>
#include "Transaction.h"

TransactionForm::TransactionForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransactionForm)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    const QStringList predefinedCategories = {
        "Pay",
        "Groceries",
        "Rent",
        "Utilities",
        "Transportation",
        "Entertainment",
        "Healthcare",
        "Education",
        "Savings"
    };

    ui->categoryComboBox->addItems(predefinedCategories);

    ui->taxWithheldCheckBox->setEnabled(false);
    ui->taxAmountLineEdit->setEnabled(false);

    connect(ui->incomeRadioButton, &QRadioButton::toggled, this, [this](bool checked) {
        ui->taxWithheldCheckBox->setEnabled(checked);
        if (!checked) {
            ui->taxWithheldCheckBox->setChecked(false);
            ui->taxAmountLineEdit->clear();
            ui->taxAmountLineEdit->setEnabled(false);
        }
    });

    connect(ui->taxWithheldCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        ui->taxAmountLineEdit->setEnabled(checked);
        if (!checked) {
            ui->taxAmountLineEdit->clear();
        }
    });

    connect(ui->savePushButton, &QPushButton::clicked, this, &TransactionForm::saveTransaction);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &TransactionForm::cancelTransaction);
}

TransactionForm::~TransactionForm()
{
    delete ui;
}

void TransactionForm::setCurrentUser(const User &user)
{
    currentUser = user;
}

void TransactionForm::saveTransaction()
{
    if (!validateTransactionInput()) {
        ui->errorLabel->setText("Invalid input. Please fill in all required fields.");
        return;
    }

    QDate date = ui->dateEdit->date();
    if (!date.isValid()) {
        ui->errorLabel->setText("Invalid date selected.");
        return;
    }

    std::string dateStr = date.toString("yyyy-MM-dd").toStdString();
    std::string category = ui->categoryComboBox->currentText().toStdString();
    std::string subcategory = ui->subcategoryLineEdit->text().toStdString();
    double amount = ui->amountLineEdit->text().toDouble();
    std::string type = ui->incomeRadioButton->isChecked() ? "Income" : "Expense";

    if (amount <= 0.0) {
        ui->errorLabel->setText("Amount must be greater than zero.");
        return;
    }

    Transaction transaction;
    transaction.setId(0);
    transaction.setUserId(currentUser.getUserId());
    transaction.setDate(dateStr);
    transaction.setCategory(category);
    transaction.setSubcategory(subcategory);
    transaction.setAmount(amount);
    transaction.setType(type);

    bool isIncome = ui->incomeRadioButton->isChecked();
    bool taxWithheld = isIncome && ui->taxWithheldCheckBox->isChecked();
    transaction.setTaxWithheld(taxWithheld);

    double taxAmount = 0.0;
    if (taxWithheld) {
        taxAmount = ui->taxAmountLineEdit->text().toDouble();
        if (taxAmount < 0.0) {
            ui->errorLabel->setText("Tax amount cannot be negative.");
            return;
        }
        transaction.setTaxAmount(taxAmount);
    } else {
        transaction.setTaxAmount(0.0);
    }

    if (Transaction::writeTransaction(transaction)) {
        ui->errorLabel->setText("Transaction saved successfully!");
        emit transactionSaved();
    } else {
        ui->errorLabel->setText("Failed to write transaction to database.");
    }
}

void TransactionForm::cancelTransaction() {
    emit transactionCancelled();
}

bool TransactionForm::validateTransactionInput()
{
    bool isTypeSelected = ui->incomeRadioButton->isChecked() || ui->expenseRadioButton->isChecked();
    bool isAmountOk = !ui->amountLineEdit->text().isEmpty() && ui->amountLineEdit->text().toDouble() > 0.0;
    bool isCategoryOk = !ui->categoryComboBox->currentText().isEmpty();

    return isTypeSelected && isAmountOk && isCategoryOk;
}

void TransactionForm::resetUI()
{
    ui->dateEdit->setDate(QDate::currentDate());
    ui->categoryComboBox->setCurrentIndex(0);
    ui->subcategoryLineEdit->clear();
    ui->amountLineEdit->clear();
    ui->incomeRadioButton->setChecked(true);
    ui->expenseRadioButton->setChecked(false);
    ui->taxWithheldCheckBox->setChecked(false);
    ui->taxWithheldCheckBox->setEnabled(true);
    ui->taxAmountLineEdit->clear();
    ui->taxAmountLineEdit->setEnabled(false);
    ui->errorLabel->clear();
}
