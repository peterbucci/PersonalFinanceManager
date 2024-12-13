#include "Transaction.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Transaction::Transaction()
    : id(0),
    userId(0),
    date("1970-01-01"),
    category(""),
    subcategory(""),
    amount(0.0),
    type("Expense"),
    taxWithheld(false),
    taxAmount(0.0)
{
}

Transaction::Transaction(int id, int userId, const std::string &date, const std::string &category,
                         const std::string &subcategory, double amount, const std::string &type,
                         bool taxWithheld, double taxAmount)
    : id(id),
    userId(userId),
    date(date),
    category(category),
    subcategory(subcategory),
    amount(amount),
    type(type),
    taxWithheld(taxWithheld),
    taxAmount(taxAmount)
{
}

// Getters
int Transaction::getId() const { return id; }
int Transaction::getUserId() const { return userId; }
std::string Transaction::getDate() const { return date; }
std::string Transaction::getCategory() const { return category; }
std::string Transaction::getSubcategory() const { return subcategory; }
double Transaction::getAmount() const { return amount; }
std::string Transaction::getType() const { return type; }
bool Transaction::isIncomeTransaction() const { return (type == "Income"); }
bool Transaction::isTaxWithheld() const { return taxWithheld; }
double Transaction::getTaxAmount() const { return taxAmount; }

// Setters
void Transaction::setId(int id) { this->id = id; }
void Transaction::setUserId(int userId) { this->userId = userId; }
void Transaction::setDate(const std::string &date) { this->date = date; }
void Transaction::setCategory(const std::string &category) { this->category = category; }
void Transaction::setSubcategory(const std::string &subcategory) { this->subcategory = subcategory; }
void Transaction::setAmount(double amount) { this->amount = amount; }
void Transaction::setType(const std::string &type) { this->type = type; }
void Transaction::setTaxWithheld(bool withheld) { this->taxWithheld = withheld; }
void Transaction::setTaxAmount(double amount) { this->taxAmount = amount; }

double Transaction::calculateNetAmount() const {
    if (type == "Income" && taxWithheld) {
        // Ensure taxAmount represents a valid percentage
        if (taxAmount < 0.0) {
            qWarning() << "Invalid tax percentage:" << taxAmount << ". Setting to 0%.";
            return amount;
        } else if (taxAmount > 100.0) {
            qWarning() << "Tax percentage exceeds 100%:" << taxAmount << ". Setting to 100%.";
            return 0.0;
        }

        // Calculate tax based on percentage
        double tax = (amount * taxAmount) / 100.0;
        return amount - tax;
    }
    return amount;
}

std::string Transaction::toString() const {
    return "ID: " + std::to_string(id) +
           ", UserID: " + std::to_string(userId) +
           ", Date: " + date +
           ", Category: " + category +
           ", Subcategory: " + subcategory +
           ", Amount: " + std::to_string(amount) +
           ", Type: " + type +
           ", TaxWithheld: " + (taxWithheld ? "Yes" : "No") +
           ", TaxAmount: " + std::to_string(taxAmount);
}

std::vector<Transaction> Transaction::readAllTransactions()
{
    std::vector<Transaction> transactions;
    QSqlQuery query("SELECT id, userId, date, category, subcategory, amount, type, taxWithheld, taxAmount FROM transactions ORDER BY date ASC");

    if (!query.exec()) {
        qWarning() << "Failed to read transactions:" << query.lastError().text();
        return transactions;
    }

    while (query.next()) {
        Transaction t;
        t.setId(query.value("id").toInt());
        t.setUserId(query.value("userId").toInt());
        t.setDate(query.value("date").toString().toStdString());
        t.setCategory(query.value("category").toString().toStdString());
        t.setSubcategory(query.value("subcategory").toString().toStdString());
        t.setAmount(query.value("amount").toDouble());
        t.setType(query.value("type").toString().toStdString());
        t.setTaxWithheld(query.value("taxWithheld").toInt() == 1);
        t.setTaxAmount(query.value("taxAmount").toDouble());
        transactions.push_back(t);
    }

    return transactions;
}

bool Transaction::writeTransaction(const Transaction &transaction)
{
    QSqlQuery query;
    query.prepare("INSERT INTO transactions (userId, date, category, subcategory, amount, type, taxWithheld, taxAmount) "
                  "VALUES (:userId, :date, :category, :subcategory, :amount, :type, :taxWithheld, :taxAmount)");
    query.bindValue(":userId", transaction.getUserId());
    query.bindValue(":date", QString::fromStdString(transaction.getDate()));
    query.bindValue(":category", QString::fromStdString(transaction.getCategory()));
    query.bindValue(":subcategory", QString::fromStdString(transaction.getSubcategory()));
    query.bindValue(":amount", transaction.getAmount());
    query.bindValue(":type", QString::fromStdString(transaction.getType()));
    query.bindValue(":taxWithheld", transaction.isTaxWithheld() ? 1 : 0);
    query.bindValue(":taxAmount", transaction.getTaxAmount());

    if (!query.exec()) {
        qWarning() << "Failed to insert transaction:" << query.lastError().text();
        return false;
    }

    return true;
}
