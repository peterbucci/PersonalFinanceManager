#include "GraphView.h"
#include "ui_GraphView.h"
#include <QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QMouseEvent>
#include <QToolTip>
#include <QCursor>
#include <algorithm>
#include <cmath>
#include <map>

GraphView::GraphView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GraphView)
    , chart(new QChart())
    , incomeLineSeries(new QLineSeries())
    , expenseLineSeries(new QLineSeries())
    , incomeScatterSeries(new QScatterSeries())
    , expenseScatterSeries(new QScatterSeries())
    , axisX(new QDateTimeAxis())
    , axisY(new QValueAxis())
{
    ui->setupUi(this);

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

    // Initialize the line series
    incomeLineSeries->setName("Income");
    incomeLineSeries->setColor(Qt::blue);

    expenseLineSeries->setName("Expenses");
    expenseLineSeries->setColor(Qt::red);

    // Initialize the scatter series
    incomeScatterSeries->setName("Income Points");
    incomeScatterSeries->setColor(Qt::blue);
    incomeScatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    incomeScatterSeries->setMarkerSize(8.0);

    expenseScatterSeries->setName("Expense Points");
    expenseScatterSeries->setColor(Qt::red);
    expenseScatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    expenseScatterSeries->setMarkerSize(8.0);

    // Add series to the chart
    chart->addSeries(incomeLineSeries);
    chart->addSeries(expenseLineSeries);
    chart->addSeries(incomeScatterSeries);
    chart->addSeries(expenseScatterSeries);

    // Initially hide scatter series; visibility controlled based on selection
    incomeScatterSeries->setVisible(true);  // Show by default if income is selected
    expenseScatterSeries->setVisible(false);

    // Hide the legend
    chart->legend()->hide();

    // Default title
    chart->setTitle("All Transactions");

    // Configure x-axis as QDateTimeAxis with rotated labels
    axisX->setFormat("yyyy-MM-dd");
    axisX->setLabelsAngle(-60); // Rotate labels by -60 degrees
    chart->addAxis(axisX, Qt::AlignBottom);
    incomeLineSeries->attachAxis(axisX);
    expenseLineSeries->attachAxis(axisX);
    incomeScatterSeries->attachAxis(axisX);
    expenseScatterSeries->attachAxis(axisX);

    // Configure y-axis as QValueAxis
    axisY->setLabelFormat("$%.2f"); // Set label format to display currency
    chart->addAxis(axisY, Qt::AlignLeft);
    incomeLineSeries->attachAxis(axisY);
    expenseLineSeries->attachAxis(axisY);
    incomeScatterSeries->attachAxis(axisY);
    expenseScatterSeries->attachAxis(axisY);

    // Increase margins
    chart->setMargins(QMargins(20, 20, 20, 60));

    ui->chartWidget->setChart(chart);
    ui->chartWidget->setRenderHint(QPainter::Antialiasing);

    // Hide options group box by default
    ui->optionsGroupBox->setVisible(false);

    // Make label clickable
    ui->label->installEventFilter(this);

    // Connect signals to update graph when filters or transaction type change
    connect(ui->categoryComboBox, &QComboBox::currentTextChanged, this, &GraphView::updateGraphFilters);
    connect(ui->subCategoryLneEdit, &QLineEdit::textChanged, this, &GraphView::updateGraphFilters);
    connect(ui->incomeRadioButton, &QRadioButton::toggled, this, &GraphView::updateGraphFilters);
    connect(ui->expensesRadioButton, &QRadioButton::toggled, this, &GraphView::updateGraphFilters);

    // Connect the hovered signals to display tooltips with $xx.xx format
    connect(incomeScatterSeries, &QScatterSeries::hovered, this, &GraphView::handleScatterHover);
    connect(expenseScatterSeries, &QScatterSeries::hovered, this, &GraphView::handleScatterHover);

    currentCategoryFilter = "";
    currentSubCategoryFilter = "";
}

GraphView::~GraphView()
{
    delete ui;
}

bool GraphView::eventFilter(QObject *obj, QEvent *event)
{
    // Toggle options show/hide
    if (obj == ui->label && event->type() == QEvent::MouseButtonRelease) {
        toggleOptions();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void GraphView::handleScatterHover(const QPointF &point, bool state) {
    if (state) { // Mouse entered a point
        chart->setCursor(Qt::PointingHandCursor);
        QDateTime date = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x()));
        QString tooltipText = QString("Date: %1\nAmount: $%2")
                                  .arg(date.toString("yyyy-MM-dd"))
                                  .arg(point.y(), 0, 'f', 2); // Format to two decimal places
        QToolTip::showText(QCursor::pos(), tooltipText, ui->chartWidget);
    } else { // Mouse left the point
        chart->setCursor(Qt::ArrowCursor);
        QToolTip::hideText();
    }
}

void GraphView::toggleOptions()
{
    bool currentlyVisible = ui->optionsGroupBox->isVisible();
    ui->optionsGroupBox->setVisible(!currentlyVisible);
    ui->label->setText(currentlyVisible ? "Show Options" : "Hide Options");
}

void GraphView::setAllTransactions(const std::vector<Transaction> &transactions)
{
    allTransactions = transactions;
    applyFiltering();
}

void GraphView::setCurrentUser(const User &user)
{
    currentUser = user;
}

void GraphView::updateGraphFilters()
{
    QString selectedCategory = ui->categoryComboBox->currentText();
    // If "All" is selected, show all categories
    currentCategoryFilter = (selectedCategory == "All") ? "" : selectedCategory;
    currentSubCategoryFilter = ui->subCategoryLneEdit->text().trimmed();

    applyFiltering();
}

void GraphView::applyFiltering()
{
    // Determine transaction type from radio buttons
    bool showIncome = ui->incomeRadioButton->isChecked();
    bool showExpenses = ui->expensesRadioButton->isChecked();

    // Filter transactions by category and subcategory
    std::vector<Transaction> filtered = allTransactions;

    if (!currentCategoryFilter.isEmpty()) {
        filtered.erase(std::remove_if(filtered.begin(), filtered.end(),
                                      [this](const Transaction &t) {
                                          return QString::fromStdString(t.getCategory()) != currentCategoryFilter;
                                      }),
                       filtered.end());
    }

    if (!currentSubCategoryFilter.isEmpty()) {
        filtered.erase(std::remove_if(filtered.begin(), filtered.end(),
                                      [this](const Transaction &t) {
                                          return !QString::fromStdString(t.getSubcategory())
                                          .contains(currentSubCategoryFilter, Qt::CaseInsensitive);
                                      }),
                       filtered.end());
    }

    // Aggregate by day but only for the selected type (income or expense)
    std::map<QString, double> dailyTotals;

    for (const auto &t : filtered) {
        bool isIncome = t.isIncomeTransaction();
        if ((showIncome && isIncome) || (showExpenses && !isIncome)) {
            QString dateKey = QString::fromStdString(t.getDate());
            dailyTotals[dateKey] += t.calculateNetAmount();
        }
    }

    // Convert dailyTotals into dataPoints
    QVector<QPointF> dataPoints;
    for (const auto &entry : dailyTotals) {
        QDateTime dt = QDateTime::fromString(entry.first, "yyyy-MM-dd");
        if (!dt.isValid()) continue;

        qint64 ms = dt.toMSecsSinceEpoch();
        double val = entry.second;
        if (val > 0) {
            dataPoints.append(QPointF(ms, val));
        }
    }

    // Sort points by x (date) ascending
    std::sort(dataPoints.begin(), dataPoints.end(), [](const QPointF &a, const QPointF &b){
        return a.x() < b.x();
    });

    // Update chart title
    // Determine transaction type string
    QString typeStr = showIncome ? "Income" : "Expenses";
    QString categoryStr = currentCategoryFilter.isEmpty() ? "All" : currentCategoryFilter;
    QString title = categoryStr + " " + typeStr;
    if (!currentSubCategoryFilter.isEmpty()) {
        title += ": " + currentSubCategoryFilter;
    }
    chart->setTitle(title);

    // Set the Tick Count on the X-Axis
    int uniqueDateCount = static_cast<int>(dailyTotals.size());
    if (uniqueDateCount < 2) uniqueDateCount = 2;
    axisX->setTickCount(uniqueDateCount);
    axisX->setFormat("yyyy-MM-dd");
    axisX->setLabelsAngle(-45);

    if (!dataPoints.isEmpty()) {
        QDateTime minDate = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(dataPoints.first().x()));
        QDateTime maxDate = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(dataPoints.last().x()));

        QDateTime bufferMinDate = minDate.addDays(-2);
        QDateTime bufferMaxDate = maxDate.addDays(2);
        if (bufferMinDate.isValid() && bufferMaxDate.isValid()) {
            axisX->setRange(bufferMinDate, bufferMaxDate);
        } else {
            axisX->setRange(minDate, maxDate);
        }
    }

    setData(dataPoints);
}

void GraphView::setData(const QVector<QPointF> &dataPoints)
{
    // Clear existing data
    incomeLineSeries->clear();
    expenseLineSeries->clear();
    incomeScatterSeries->clear();
    expenseScatterSeries->clear();

    bool showIncome = ui->incomeRadioButton->isChecked();
    QLineSeries *activeLineSeries = showIncome ? incomeLineSeries : expenseLineSeries;
    QScatterSeries *activeScatterSeries = showIncome ? incomeScatterSeries : expenseScatterSeries;

    incomeLineSeries->setVisible(showIncome);
    expenseLineSeries->setVisible(!showIncome);
    incomeScatterSeries->setVisible(showIncome);
    expenseScatterSeries->setVisible(!showIncome);

    if (!dataPoints.isEmpty()) {
        activeLineSeries->replace(dataPoints);
        activeScatterSeries->replace(dataPoints);
    }

    if (dataPoints.isEmpty()) {
        QDateTime now = QDateTime::currentDateTime();
        axisX->setRange(now, now.addDays(1));
        axisY->setRange(0, 1);
        chart->update();
        return;
    }

    qreal minX = dataPoints.first().x();
    qreal maxX = dataPoints.first().x();
    qreal minY = dataPoints.first().y();
    qreal maxY = dataPoints.first().y();

    for (const QPointF &point : dataPoints) {
        if (point.x() < minX) minX = point.x();
        if (point.x() > maxX) maxX = point.x();
        if (point.y() < minY) minY = point.y();
        if (point.y() > maxY) maxY = point.y();
    }

    double range = maxY;
    if (range < 0) range = 0;
    double padding = range * 0.1;
    if (padding < 1.0) padding = 1.0;
    double maxValForAxis = range + padding;

    int roundedMax = 0;
    if (maxValForAxis < 100) {
        roundedMax = static_cast<int>(std::ceil(maxValForAxis / 10.0)) * 10;
    } else {
        roundedMax = static_cast<int>(std::ceil(maxValForAxis / 100.0)) * 100;
    }

    axisY->setRange(0, roundedMax);
    axisY->setLabelFormat("$%.2f");

    chart->update();
}

void GraphView::resetUI()
{
    // Reset filters
    ui->categoryComboBox->setCurrentIndex(0);
    ui->subCategoryLneEdit->clear();
    ui->incomeRadioButton->setChecked(false);
    ui->expensesRadioButton->setChecked(true);

    // Reset options group box
    ui->optionsGroupBox->setVisible(false);
    ui->label->setText("Show Options");

    currentCategoryFilter = "";
    currentSubCategoryFilter = "";

    // Re-apply filtering with default filters
    applyFiltering();
}
