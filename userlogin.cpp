#include "userlogin.h"

UserLogin::UserLogin(int loginId, const QString &username, const QString &password, int accessLevel, int userId)
    : m_loginid(loginId), m_username(username), m_password(password), m_accessLevel(accessLevel), m_userid(userId)
{
}

int UserLogin::getLoginId() const {
    return m_loginid;
}

void UserLogin::setLoginId(int loginId) {
    m_loginid = loginId;
}

QString UserLogin::getUsername() const {
    return m_username;
}

void UserLogin::setUsername(const QString &username) {
    m_username = username;
}

QString UserLogin::getPassword() const {
    return m_password;
}

void UserLogin::setPassword(const QString &password) {
    m_password = password;
}

int UserLogin::getAccessLevel() const {
    return m_accessLevel;
}

void UserLogin::setAccessLevel(int accessLevel) {
    m_accessLevel = accessLevel;
}

int UserLogin::getUserId() const {
    return m_userid;
}

void UserLogin::setUserId(int userId) {
    m_userid = userId;
}
