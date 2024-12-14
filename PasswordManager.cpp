#include "PasswordManager.h"
#include <QCryptographicHash>
#include <QVBoxLayout>
#include <QCursor>
#include <QTimer>
#include <QApplication>
#include <QPalette>

PasswordManager::PasswordManager(QObject *parent)
    : QObject(parent),
    m_strength(0),
    m_passwordsMatch(false),
    m_uppercaseRegex("[A-Z]"),
    m_lowercaseRegex("[a-z]"),
    m_digitRegex("[0-9]"),
    m_specialCharRegex("[^a-zA-Z0-9]"),
    m_tooltipFrame(nullptr),
    m_tooltipLabel(nullptr)
{
}

QString PasswordManager::hashPassword(const QString &password)
{
    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hashed.toHex());
}

int PasswordManager::getPasswordStrength() const
{
    return m_strength;
}

bool PasswordManager::doPasswordsMatch() const
{
    return m_passwordsMatch;
}

bool PasswordManager::hasUppercase() const
{
    return m_uppercaseRegex.match(m_password).hasMatch();
}

bool PasswordManager::hasLowercase() const
{
    return m_lowercaseRegex.match(m_password).hasMatch();
}

bool PasswordManager::hasDigit() const
{
    return m_digitRegex.match(m_password).hasMatch();
}

bool PasswordManager::hasSpecialChar() const
{
    return m_specialCharRegex.match(m_password).hasMatch();
}

bool PasswordManager::meetsMinLength() const
{
    return m_password.length() >= 8;
}

void PasswordManager::setPassword(const QString &password)
{
    m_password = password;
    evaluateStrength();
    evaluateMatch();
}

void PasswordManager::setConfirmPassword(const QString &confirmPassword)
{
    m_confirmPassword = confirmPassword;
    evaluateMatch();
}

void PasswordManager::evaluateStrength()
{
    int strength = 0;

    // Password length
    if (meetsMinLength())
        strength += 25;

    // Uppercase letters
    if (hasUppercase())
        strength += 25;

    // Lowercase letters
    if (hasLowercase())
        strength += 15;

    // Digits
    if (hasDigit())
        strength += 20;

    // Special characters
    if (hasSpecialChar())
        strength += 15;

    // Ensure strength does not exceed 100
    if (strength > 100)
        strength = 100;

    if (strength != m_strength) {
        m_strength = strength;
        emit strengthChanged(m_strength);
    }
}

void PasswordManager::evaluateMatch()
{
    bool match = (m_password == m_confirmPassword);

    if (match != m_passwordsMatch) {
        m_passwordsMatch = match;
        emit matchStatusChanged(m_passwordsMatch);
    }
}

#include <QApplication>
#include <QPalette>

void PasswordManager::initializeTooltip(QWidget *parent)
{
    if (m_tooltipFrame)
        return; // Already initialized

    // Create the tooltip frame
    m_tooltipFrame = new QFrame(parent, Qt::ToolTip);
    m_tooltipFrame->setFrameShape(QFrame::Box);
    m_tooltipFrame->setFrameShadow(QFrame::Raised);

    // Determine if the system is in dark mode
    bool isDarkMode = (QApplication::palette().color(QPalette::Window).lightness() < 128);

    // Set tooltip style based on dark or light mode
    if (isDarkMode) {
        // Dark mode styling: dark background with light text
        m_tooltipFrame->setStyleSheet("background-color: #2E2E2E; border: 1px solid #555555;");
        m_tooltipLabel = new QLabel(m_tooltipFrame);
        m_tooltipLabel->setStyleSheet("color: #FFFFFF;"); // White text
    } else {
        // Light mode styling: light background with dark text
        m_tooltipFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #AAAAAA;");
        m_tooltipLabel = new QLabel(m_tooltipFrame);
        m_tooltipLabel->setStyleSheet("color: #000000;"); // Black text
    }

    // Create the label inside the frame
    m_tooltipLabel->setTextFormat(Qt::RichText);
    m_tooltipLabel->setWordWrap(true);
    m_tooltipLabel->setMargin(5); // Padding inside the label

    // Layout for the frame
    QVBoxLayout *layout = new QVBoxLayout(m_tooltipFrame);
    layout->setContentsMargins(5, 5, 5, 5); // Padding on all four sides
    layout->addWidget(m_tooltipLabel);

    // Initially hide the tooltip
    m_tooltipFrame->hide();
}

void PasswordManager::showPasswordTooltip(QWidget *widget)
{
    if (!widget || !m_tooltipFrame || !m_tooltipLabel)
        return;

    QString tooltipText = generateTooltipContent();

    // Set the tooltip content
    m_tooltipLabel->setText(tooltipText);

    // Calculate position: always below the widget
    QPoint widgetPos = widget->mapToGlobal(QPoint(0, widget->height()));
    m_tooltipFrame->move(widgetPos + QPoint(0, 5)); // Slight offset for better visibility

    // Show the tooltip
    m_tooltipFrame->adjustSize();
    m_tooltipFrame->show();
}

void PasswordManager::hidePasswordTooltip()
{
    if (m_tooltipFrame) {
        m_tooltipFrame->hide();
    }
}

QString PasswordManager::generateTooltipContent() const
{
    QString matchText = m_passwordsMatch ?
                            QString("<span style=\"color:green;\">Passwords match</span>") :
                            QString("<span style=\"color:red;\">Passwords do not match</span>");

    QPair<QString, QString> strengthInfo = getPasswordStrengthInfo();

    QString strengthText = QString("<span style=\"color:%1;\">Password Strength: %2</span>")
                               .arg(strengthInfo.second)
                               .arg(strengthInfo.first);

    QString suggestions = generateStrengthSuggestions();

    // Combine all parts into HTML with padding on all sides
    QString tooltipHtml = QString("<div>"
                                  "%1<br>"
                                  "%2<br><br>"
                                  "<b>Suggestions to improve password strength:</b><br>"
                                  "%3"
                                  "</div>")
                              .arg(matchText)
                              .arg(strengthText)
                              .arg(suggestions);

    return tooltipHtml;
}

QPair<QString, QString> PasswordManager::getPasswordStrengthInfo() const
{
    if (m_strength < 40) {
        return qMakePair(QString("Weak"), QString("red"));
    }
    else if (m_strength < 70) {
        return qMakePair(QString("Moderate"), QString("orange"));
    }
    else {
        return qMakePair(QString("Strong"), QString("green"));
    }
}

QString PasswordManager::generateStrengthSuggestions() const
{
    QString suggestions;

    // List of suggestions with corresponding checks
    struct Suggestion {
        bool met;
        QString text;
    };

    QList<Suggestion> suggestionList = {
        { hasUppercase(), "Add uppercase letters" },
        { hasLowercase(), "Add lowercase letters" },
        { hasDigit(), "Add numbers" },
        { hasSpecialChar(), "Add special characters" },
        { meetsMinLength(), "Increase minimum length to 8 characters" }
    };

    for (const auto &sugg : suggestionList) {
        if (sugg.met) {
            // Color both the check mark and the text green
            suggestions += QString("<span style=\"color:green;\">%1 %2</span><br>")
                               .arg(CHECK_MARK)
                               .arg(sugg.text);
        }
        else {
            // Color both the cross mark and the text red
            suggestions += QString("<span style=\"color:red;\">%1 %2</span><br>")
                               .arg(CROSS_MARK)
                               .arg(sugg.text);
        }
    }

    return suggestions;
}
