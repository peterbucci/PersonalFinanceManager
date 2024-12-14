#include "ViewTransactions.h"
#include "ui_ViewTransactions.h"
#include <QEvent>
#include <QMouseEvent>
#include <QHeaderView>

ViewTransactions::ViewTransactions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewTransactions)
{
    ui->setupUi(this);

    // Hide the options group box by default
    ui->optionsGroupBox->setVisible(false);
    // Make the label clickable
    ui->label->installEventFilter(this);

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
    ui->categoryComboBox->addItem("All");
    ui->categoryComboBox->addItems(predefinedCategories);

    // Set table properties
    ui->transactionTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->transactionTableWidget->setSortingEnabled(false);
    ui->transactionTableWidget->horizontalHeader()->setSectionsClickable(false);

    connect(ui->categoryComboBox, &QComboBox::currentTextChanged, this, &ViewTransactions::updateFilters);
    connect(ui->subcategoryLineEdit, &QLineEdit::textChanged, this, &ViewTransactions::updateFilters);

    currentCategoryFilter = "";
    currentSubCategoryFilter = "";
}

ViewTransactions::~ViewTransactions()
{
    delete ui;
}

bool ViewTransactions::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->label && event->type() == QEvent::MouseButtonRelease) {
        toggleOptions();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void ViewTransactions::toggleOptions()
{
    bool currentlyVisible = ui->optionsGroupBox->isVisible();
    ui->optionsGroupBox->setVisible(!currentlyVisible);
    ui->label->setText(currentlyVisible ? "Show Options" : "Hide Options");
}

void ViewTransactions::setCurrentUser(const User &user)
{
    currentUser = user;
}

void ViewTransactions::setAllTransactions(const std::vector<Transaction> &transactions)
{
    allTransactions = transactions;
    applyFiltering();
}

void ViewTransactions::updateFilters()
{
    QString selectedCategory = ui->categoryComboBox->currentText();
    // If "All" is selected, no category filter
    currentCategoryFilter = (selectedCategory == "All") ? "" : selectedCategory;
    currentSubCategoryFilter = ui->subcategoryLineEdit->text().trimmed();

    applyFiltering();
}

void ViewTransactions::applyFiltering()
{
    bool hasCategoryFilter = !currentCategoryFilter.isEmpty();
    bool hasSubCategoryFilter = !currentSubCategoryFilter.isEmpty();
    bool filtersApplied = hasCategoryFilter || hasSubCategoryFilter;

    // Preallocate space based on allTransactions size to improve performance
    std::vector<Transaction> filtered;
    filtered.reserve(allTransactions.size());

    // Apply both category and subcategory filters in one loop
    for (const auto &t : allTransactions) {
        // Apply category filter if active
        if (hasCategoryFilter) {
            // Convert category once and reuse
            const QString category = QString::fromStdString(t.getCategory());
            if (category != currentCategoryFilter)
                continue; // Skip transactions that do not match the category
        }

        // Apply subcategory filter if active
        if (hasSubCategoryFilter) {
            // Convert subcategory once and reuse
            const QString subcategory = QString::fromStdString(t.getSubcategory());
            if (!subcategory.contains(currentSubCategoryFilter, Qt::CaseInsensitive))
                continue; // Skip transactions that do not match the subcategory
        }

        // If transaction passes all active filters, add it to the filtered list
        filtered.emplace_back(t);
    }

    // Populate the table with the filtered transactions
    // Parameters:
    // - showBalance: Display balance column if no filters are applied
    // - showTotalRow: Display total row if any filter is applied
    populateViewTable(filtered, !filtersApplied, filtersApplied);
}


void ViewTransactions::populateViewTable(const std::vector<Transaction> &transactions, bool showBalance, bool showTotalRow)
{
    ui->transactionTableWidget->clearContents();
    ui->transactionTableWidget->setRowCount(0);

    // Set headers
    QStringList headers;
    headers << "Date";
    if (showBalance) headers << "Category";
    headers << "Subcategory" << "Amount";
    if (showBalance) headers << "Balance";

    ui->transactionTableWidget->setColumnCount(headers.size());
    ui->transactionTableWidget->setHorizontalHeaderLabels(headers);

    double runningBalance = 0.0;
    for (auto &t : transactions) {
        // Get the transaction amount minus witholdings.
        double netAmount = t.calculateNetAmount();

        // Add or subtract that amount from the running balance.
        if (t.isIncomeTransaction()) {
            runningBalance += netAmount;
        } else {
            runningBalance -= netAmount;
        }

        // Add a new row to the bottom of the table
        int row = ui->transactionTableWidget->rowCount();
        ui->transactionTableWidget->insertRow(row);

        // Insert transaction details into the columns of this row.
        int currColumn = 0;
        ui->transactionTableWidget->setItem(row, currColumn++, new QTableWidgetItem(QString::fromStdString(t.getDate())));

        if (showBalance) {
            ui->transactionTableWidget->setItem(row, currColumn++, new QTableWidgetItem(QString::fromStdString(t.getCategory())));
        }

        ui->transactionTableWidget->setItem(row, currColumn++, new QTableWidgetItem(QString::fromStdString(t.getSubcategory())));

        double amt = t.isIncomeTransaction() ? netAmount : -netAmount;
        ui->transactionTableWidget->setItem(row, currColumn++, new QTableWidgetItem(QString::number(amt, 'f', 2)));

        if (showBalance) {
            ui->transactionTableWidget->setItem(row, currColumn++, new QTableWidgetItem(QString::number(runningBalance, 'f', 2)));
        }
    }

    // If a filter is applied, show the total balance in the last row of the table.
    if (!showBalance && showTotalRow && !transactions.empty()) {
        // Insert TOTAL row
        int totalRow = ui->transactionTableWidget->rowCount();
        ui->transactionTableWidget->insertRow(totalRow);
        ui->transactionTableWidget->setItem(totalRow, 0, new QTableWidgetItem(""));
        ui->transactionTableWidget->setItem(totalRow, 1, new QTableWidgetItem("TOTAL"));
        ui->transactionTableWidget->setItem(totalRow, 2, new QTableWidgetItem(QString::number(runningBalance, 'f', 2)));
    }

    // Always stretch columns
    ui->transactionTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ViewTransactions::resetUI()
{
    ui->categoryComboBox->setCurrentIndex(0);
    ui->subcategoryLineEdit->clear();
    ui->optionsGroupBox->setVisible(false);
    ui->label->setText("Show Options");
    ui->transactionTableWidget->clearContents();
    ui->transactionTableWidget->setRowCount(0);

    currentCategoryFilter = "";
    currentSubCategoryFilter = "";

    // Re-apply filtering with defaults
    applyFiltering();
}
