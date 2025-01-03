#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QScatterSeries>
#include <QTimer>
#include <QGraphicsSimpleTextItem>
#include "User.h"
#include "Transaction.h"

namespace Ui {
class GraphView;
}

/**
 * @brief The GraphView class represents a widget that displays a line chart of transactions over time.
 */
class GraphView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a new GraphView object.
     * @param parent The parent widget.
     */
    explicit GraphView(QWidget *parent = nullptr);

    /**
     * @brief Destroys the GraphView object.
     */
    ~GraphView();

    /**
     * @brief Sets all transactions for filtering and displaying in the graph.
     * @param transactions All user transactions.
     */
    void setAllTransactions(const std::vector<Transaction> &transactions);

    /**
     * @brief Sets the current user.
     * @param user Current user.
     */
    void setCurrentUser(const User &user);

    /**
     * @brief Resets the UI elements to their default state.
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
     * @brief Toggles the visibility of the options group box and changes label text.
     */
    void toggleOptions();

    /**
     * @brief Refilters and updates the graph data based on selected category, subcategory, and transaction type.
     */
    void updateGraphFilters();

    /**
     * @brief Hides the custom tooltip after a delay when the mouse leaves the point.
     */
    void hideTooltip();

private:
    Ui::GraphView *ui; ///< Pointer to the UI components of GraphView.
    QChart *chart; ///< Chart object representing the graph.
    QLineSeries *incomeLineSeries; ///< Line series for income data.
    QLineSeries *expenseLineSeries; ///< Line series for expense data.
    QScatterSeries *incomeScatterSeries; ///< Scatter series for income data points.
    QScatterSeries *expenseScatterSeries; ///< Scatter series for expense data points.
    QDateTimeAxis *axisX; ///< X-axis representing dates.
    QValueAxis *axisY; ///< Y-axis representing total values.
    User currentUser; ///< The current user for whom the graph is displayed.
    std::vector<Transaction> allTransactions; ///< All transactions associated with the current user.
    QString currentCategoryFilter; ///< Current category filter applied to the graph.
    QString currentSubCategoryFilter; ///< Current subcategory filter applied to the graph.
    QTimer tooltipHideTimer;       ///< Timer to delay hiding the tooltip after hover ends.
    bool tooltipVisible;           ///< Flag indicating if the tooltip is currently visible.
    QGraphicsSimpleTextItem *chartTooltip; ///< The custom tooltip graphics item.

    /**
     * @brief Apply category/subcategory filtering to allTransactions and update the chart.
     * Only show one line (Income or Expenses) based on the selected radio button.
     */
    void applyFiltering();

    /**
     * @brief Updates the chart with the provided data points and adjusts the Y-axis range.
     *
     * @param dataPoints A QVector of QPointF representing the data points.
     * @param maxY       The maximum Y-value from the data points, used to calculate the upper bound.
     */
    void setData(const QVector<QPointF> &dataPoints, double maxY);

    /**
     * @brief Handles hover events over scatter plot points to display tooltips.
     *
     * @param point The QPointF representing the data point being hovered over.
     * @param state True if the mouse enters hover state, false if leaving.
     */
    void handleScatterHover(const QPointF &point, bool state);
};

#endif // GRAPHVIEW_H
