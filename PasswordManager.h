#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QFrame>
#include <QLabel>
#include <QPair>

/**
 * @brief The PasswordManager class handles password hashing, strength validation, and tooltips.
 */
class PasswordManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the PasswordManager.
     * @param parent The parent QObject.
     */
    explicit PasswordManager(QObject *parent = nullptr);

    /**
     * @brief Hashes the given plain-text password using SHA-256.
     * @param password The plain-text password to hash.
     * @return The hashed password as a hexadecimal QString.
     */
    QString hashPassword(const QString &password);

    /**
     * @brief Retrieves the current password strength.
     * @return An integer representing the strength level (0-100).
     */
    int getPasswordStrength() const;

    /**
     * @brief Checks if the password and confirm password match.
     * @return True if they match, false otherwise.
     */
    bool doPasswordsMatch() const;

    /**
     * @brief Checks if the password contains at least one uppercase letter.
     * @return True if it contains an uppercase letter, false otherwise.
     */
    bool hasUppercase() const;

    /**
     * @brief Checks if the password contains at least one lowercase letter.
     * @return True if it contains a lowercase letter, false otherwise.
     */
    bool hasLowercase() const;

    /**
     * @brief Checks if the password contains at least one digit.
     * @return True if it contains a digit, false otherwise.
     */
    bool hasDigit() const;

    /**
     * @brief Checks if the password contains at least one special character.
     * @return True if it contains a special character, false otherwise.
     */
    bool hasSpecialChar() const;

    /**
     * @brief Checks if the password meets the minimum length requirement.
     * @return True if it meets the minimum length, false otherwise.
     */
    bool meetsMinLength() const;

public slots:
    /**
     * @brief Sets the password and updates its strength.
     * @param password The password entered by the user.
     */
    void setPassword(const QString &password);

    /**
     * @brief Sets the confirm password and updates the match status.
     * @param confirmPassword The confirm password entered by the user.
     */
    void setConfirmPassword(const QString &confirmPassword);

    /**
     * @brief Initializes the tooltip with the given parent widget.
     * @param parent The parent widget for positioning the tooltip.
     */
    void initializeTooltip(QWidget *parent);

    /**
     * @brief Displays the password tooltip relative to the specified widget.
     * @param widget The widget to position the tooltip below.
     */
    void showPasswordTooltip(QWidget *widget);

    /**
     * @brief Hides the password tooltip.
     */
    void hidePasswordTooltip();

signals:
    /**
     * @brief Emitted when the password strength changes.
     * @param strength The new strength value.
     */
    void strengthChanged(int strength);

    /**
     * @brief Emitted when the password match status changes.
     * @param match True if passwords match, false otherwise.
     */
    void matchStatusChanged(bool match);

private:
    QString m_password;          ///< The current password.
    QString m_confirmPassword;   ///< The current confirm password.
    int m_strength;              ///< Current password strength.
    bool m_passwordsMatch;       ///< Current password match status.

    // Regular expressions for password criteria
    QRegularExpression m_uppercaseRegex;      ///< Regex to check for uppercase letters.
    QRegularExpression m_lowercaseRegex;      ///< Regex to check for lowercase letters.
    QRegularExpression m_digitRegex;          ///< Regex to check for digits.
    QRegularExpression m_specialCharRegex;    ///< Regex to check for special characters.

    // Tooltip related
    QFrame *m_tooltipFrame;    ///< Frame to hold the tooltip content.
    QLabel *m_tooltipLabel;    ///< Label to display the tooltip HTML content.

    // Unicode characters for check and cross
    const QString CHECK_MARK = "&#10004;"; // ✔
    const QString CROSS_MARK = "&#10006;"; // ✖

    /**
     * @brief Evaluates the strength of the current password.
     */
    void evaluateStrength();

    /**
     * @brief Evaluates whether the password and confirm password match.
     */
    void evaluateMatch();

    /**
     * @brief Generates the HTML content for the tooltip.
     * @return A QString containing the formatted HTML tooltip.
     */
    QString generateTooltipContent() const;

    /**
     * @brief Maps password strength value to corresponding text and color.
     * @return A pair containing the strength text and its color.
     */
    QPair<QString, QString> getPasswordStrengthInfo() const;

    /**
     * @brief Generates suggestions to improve password strength.
     * @return A QString containing HTML-formatted suggestions.
     */
    QString generateStrengthSuggestions() const;
};

#endif // PASSWORDMANAGER_H
