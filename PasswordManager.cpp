#include "PasswordManager.h"
#include <QCryptographicHash>
#include <QRegularExpression>

PasswordManager::PasswordManager(QObject *parent)
    : QObject(parent),
    m_strength(0),
    m_passwordsMatch(false)
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

    // Simple password strength criteria
    if (m_password.length() >= 8)
        strength += 25;
    if (m_password.contains(QRegularExpression("[A-Z]")))
        strength += 25;
    if (m_password.contains(QRegularExpression("[a-z]")))
        strength += 15;
    if (m_password.contains(QRegularExpression("[0-9]")))
        strength += 20;
    if (m_password.contains(QRegularExpression("[^a-zA-Z0-9]")))
        strength += 15;

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
