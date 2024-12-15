#include "GraphView.h"
#include "ui_GraphView.h"
#include <QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QMouseEvent>
#include <QCursor>
#include <QPen>
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
    , tooltipVisible(false)
    , chartTooltip(new QGraphicsSimpleTextItem(chart))
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
    incomeLineSeries->setPen(QPen(Qt::blue, 3));

    expenseLineSeries->setName("Expenses");
    expenseLineSeries->setPen(QPen(Qt::red, 3));

    // Initialize the scatter series
    incomeScatterSeries->setName("Income Points");
    incomeScatterSeries->setColor(Qt::blue);
    incomeScatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    incomeScatterSeries->setMarkerSize(12.0);

    expenseScatterSeries->setName("Expense Points");
    expenseScatterSeries->setColor(Qt::red);
    expenseScatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    expenseScatterSeries->setMarkerSize(12.0);

    // Add series to the chart
    chart->addSeries(incomeLineSeries);
    chart->addSeries(expenseLineSeries);
    chart->addSeries(incomeScatterSeries);
    chart->addSeries(expenseScatterSeries);

    // Initially show income and hide expense scatter series
    incomeScatterSeries->setVisible(true);
    expenseScatterSeries->setVisible(false);

    // Hide the legend
    chart->legend()->hide();

    // Default title
    chart->setTitle("All Transactions");

    // Configure x-axis
    axisX->setFormat("yyyy-MM-dd");
    axisX->setLabelsAngle(-60);
    chart->addAxis(axisX, Qt::AlignBottom);
    incomeLineSeries->attachAxis(axisX);
    expenseLineSeries->attachAxis(axisX);
    incomeScatterSeries->attachAxis(axisX);
    expenseScatterSeries->attachAxis(axisX);

    // Configure y-axis
    axisY->setLabelFormat("$%.2f");
    chart->addAxis(axisY, Qt::AlignLeft);
    incomeLineSeries->attachAxis(axisY);
    expenseLineSeries->attachAxis(axisY);
    incomeScatterSeries->attachAxis(axisY);
    expenseScatterSeries->attachAxis(axisY);

    // Increase margins
    chart->setMargins(QMargins(20, 20, 20, 60));

    ui->chartWidget->setChart(chart);
    ui->chartWidget->setRenderHint(QPainter::Antialiasing);

    // Hide options by default
    ui->optionsGroupBox->setVisible(false);

    // Make label clickable
    ui->label->installEventFilter(this);

    // Connect signals to update graph on filter changes
    connect(ui->categoryComboBox, &QComboBox::currentTextChanged, this, &GraphView::updateGraphFilters);
    connect(ui->subCategoryLneEdit, &QLineEdit::textChanged, this, &GraphView::updateGraphFilters);
    connect(ui->incomeRadioButton, &QRadioButton::toggled, this, &GraphView::updateGraphFilters);
    connect(ui->expensesRadioButton, &QRadioButton::toggled, this, &GraphView::updateGraphFilters);

    // Connect hovered signals
    connect(incomeScatterSeries, &QScatterSeries::hovered, this, &GraphView::handleScatterHover);
    connect(expenseScatterSeries, &QScatterSeries::hovered, this, &GraphView::handleScatterHover);

    // Setup the tooltipHideTimer
    tooltipHideTimer.setSingleShot(true);
    connect(&tooltipHideTimer, &QTimer::timeout, this, &GraphView::hideTooltip);

    // Configure the custom tooltip appearance
    chartTooltip->setZValue(11); // Ensure it appears on top of chart elements
    chartTooltip->hide();        // Initially hidden
    QFont f = chartTooltip->font();
    f.setBold(true);
    chartTooltip->setFont(f);

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
    if (state) {
        // Stop any pending hide actions since we're hovering again
        tooltipHideTimer.stop();

        // Show the tooltip
        chart->setCursor(Qt::PointingHandCursor);
        QDateTime date = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x()));
        QString tooltipText = QString("Date: %1\nAmount: $%2")
                                  .arg(date.toString("yyyy-MM-dd"))
                                  .arg(point.y(), 0, 'f', 2);

        chartTooltip->setText(tooltipText);
        chartTooltip->show();

        // Position the tooltip near the hovered point
        QPointF scenePos = chart->mapToPosition(point);
        // Adjust offset so tooltip doesn't cover the point
        scenePos.setY(scenePos.y() - 40);
        chartTooltip->setPos(scenePos);

        tooltipVisible = true;
    } else {
        // Hover ended, start a timer before hiding the tooltip
        tooltipHideTimer.start(200);
    }
}

void GraphView::hideTooltip() {
    // Called after the timer times out, indicating the mouse truly left
    if (tooltipVisible) {
        chart->setCursor(Qt::ArrowCursor);
        chartTooltip->hide();
        tooltipVisible = false;
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

    // Initialize daily totals
    std::map<QString, double> dailyTotals;

    // Prepare for data points
    QVector<QPointF> dataPoints;
    dataPoints.reserve(allTransactions.size());

    // Track min and max Y
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    // Filter and aggregate
    for (const auto &t : allTransactions) {
        // Category filter
        if (!currentCategoryFilter.isEmpty()) {
            QString category = QString::fromStdString(t.getCategory());
            if (category != currentCategoryFilter)
                continue;
        }

        // Subcategory filter
        if (!currentSubCategoryFilter.isEmpty()) {
            QString subcategory = QString::fromStdString(t.getSubcategory());
            if (!subcategory.contains(currentSubCategoryFilter, Qt::CaseInsensitive))
                continue;
        }

        // Transaction type filter
        bool isIncome = t.isIncomeTransaction();
        if (!((showIncome && isIncome) || (showExpenses && !isIncome))) {
            continue;
        }

        // Aggregate by day
        QString dateKey = QString::fromStdString(t.getDate());
        dailyTotals[dateKey] += t.calculateNetAmount();
    }

    // Convert dailyTotals to dataPoints
    for (const auto &entry : dailyTotals) {
        QDateTime dt = QDateTime::fromString(entry.first, "yyyy-MM-dd");
        if (!dt.isValid()) continue;

        qint64 ms = dt.toMSecsSinceEpoch();
        double val = entry.second;
        if (val > 0) {
            dataPoints.append(QPointF(ms, val));
            if (val < minY) minY = val;
            if (val > maxY) maxY = val;
        }
    }

    // Sort by date
    std::sort(dataPoints.begin(), dataPoints.end(), [](const QPointF &a, const QPointF &b){
        return a.x() < b.x();
    });

    // Update chart title
    QString typeStr = showIncome ? "Income" : "Expenses";
    QString categoryStr = currentCategoryFilter.isEmpty() ? "All" : currentCategoryFilter;
    QString title = categoryStr + " " + typeStr;
    if (!currentSubCategoryFilter.isEmpty()) {
        title += ": " + currentSubCategoryFilter;
    }
    chart->setTitle(title);

    // Configure axes
    int uniqueDateCount = static_cast<int>(dailyTotals.size());
    uniqueDateCount = std::max(uniqueDateCount, 2);
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

    setData(dataPoints, maxY);
}

void GraphView::setData(const QVector<QPointF> &dataPoints, double maxY)
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

    double range = maxY;
    if (range < 0) range = 0;
    double padding = range * 0.1;
    padding = std::max(padding, 1.0);
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
