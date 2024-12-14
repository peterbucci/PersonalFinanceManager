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

    /**
     * @brief Apply category/subcategory filtering to allTransactions and update the chart.
     * Only show one line (Income or Expenses) based on the selected radio button.
     */
    void applyFiltering();

    /**
     * @brief Updates the chart with the provided data points and adjusts the Y-axis range.
     *
     * This function sets the data points for the chart based on either income or expense transactions.
     * It updates the visible line and scatter series, configures the Y-axis range using the provided
     * minimum and maximum Y-values, and ensures the chart is properly scaled and rendered.
     *
     * @param dataPoints A QVector of QPointF representing the data points (either income or expense).
     *                  Each QPointF contains the X (time in milliseconds since epoch) and Y (amount) coordinates.
     * @param minY       The minimum Y-value from the data points, used to determine the lower bound of the Y-axis.
     * @param maxY       The maximum Y-value from the data points, used to calculate the upper bound of the Y-axis with padding.
     */
    void setData(const QVector<QPointF> &dataPoints, double minY, double maxY);


    /**
         * @brief Handles hover events over scatter plot points to display tooltips.
         *
         * When the mouse hovers over a data point on the scatter plot, this function
         * shows a tooltip displaying the date and amount associated with that point,
         * and changes the cursor to a pointing hand. When the mouse leaves the point,
         * it hides the tooltip and resets the cursor to the default arrow.
         *
         * @param point The QPointF representing the data point being hovered over.
         * @param state True if the mouse is entering the hover state over the point, false if leaving.
         */
    void handleScatterHover(const QPointF &point, bool state);

};

#endif // GRAPHVIEW_H
