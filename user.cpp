#include "user.h"

User::User(int userId, const QString &firstName, const QString &lastName, const QString &position)
    : m_userid(userId), m_firstname(firstName), m_lastname(lastName), m_position(position)
{
}

int User::getUserId() const {
    return m_userid;
}

void User::setUserId(int userId) {
    m_userid = userId;
}

QString User::getFirstName() const {
    return m_firstname;
}

void User::setFirstName(const QString &firstName) {
    m_firstname = firstName;
}

QString User::getLastName() const {
    return m_lastname;
}

void User::setLastName(const QString &lastName) {
    m_lastname = lastName;
}

QString User::getPosition() const {
    return m_position;
}

void User::setPosition(const QString &position) {
    m_position = position;
}
