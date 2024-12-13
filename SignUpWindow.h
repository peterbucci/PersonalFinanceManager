#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class SignUpWindow;
}

/**
 * @brief The SignUpWindow class provides the UI and logic for user registration.
 */
class SignUpWindow : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Default constructor for SignUpWindow.
     * Initializes member variables with default values.
     */
    SignUpWindow() {}

    /**
     * @brief Constructs the SignUpWindow.
     * @param db The database connection reference.
     * @param parent The parent widget.
     * @return A SignUpWindow object.
     */
    explicit SignUpWindow(QSqlDatabase db, QWidget *parent = nullptr);

    /**
     * @brief Destructs the SignUpWindow.
     */
    ~SignUpWindow();

    /**
     * @brief Resets all UI elements to their default state.
     */
    void resetUI();

signals:
    /**
     * @brief Emitted when the user clicks to show the Login window.
     */
    void showLogin();

private slots:
    /**
     * @brief Handles the sign up button click.
     */
    void on_signUpPushButton_clicked();

protected:
    /**
     * @brief Overrides the event filter to handle specific events.
     * @param obj The object where the event originated.
     * @param event The event to be filtered.
     * @return true if the event is handled, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::SignUpWindow *ui; ///< Pointer to the UI components of SignUpWindow.
    QSqlDatabase m_db; ///< Database connection reference.
};

#endif // SIGNUPWINDOW_H
