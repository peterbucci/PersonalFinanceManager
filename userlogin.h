#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QString>

/**
 * @brief The UserLogin class represents a user's login credentials and associated info.
 */
class UserLogin {
public:
    /**
     * @brief Constructs a UserLogin object.
     * @param loginId The unique login ID.
     * @param username The username.
     * @param password The password.
     * @param accessLevel The access level associated with the user.
     * @param userId The user ID associated with this login.
     * @return A UserLogin object.
     */
    UserLogin(int loginId, const QString &username, const QString &password, int accessLevel, int userId);

    /**
     * @brief Gets the login ID.
     * @return The login ID as an integer.
     */
    int getLoginId() const;

    /**
     * @brief Sets the login ID.
     * @param loginId The new login ID.
     */
    void setLoginId(int loginId);

    /**
     * @brief Gets the username.
     * @return The username as a QString.
     */
    QString getUsername() const;

    /**
     * @brief Sets the username.
     * @param username The new username.
     */
    void setUsername(const QString &username);

    /**
     * @brief Gets the password.
     * @return The password as a QString.
     */
    QString getPassword() const;

    /**
     * @brief Sets the password.
     * @param password The new password.
     */
    void setPassword(const QString &password);

    /**
     * @brief Gets the access level.
     * @return The access level as an integer.
     */
    int getAccessLevel() const;

    /**
     * @brief Sets the access level.
     * @param accessLevel The new access level.
     */
    void setAccessLevel(int accessLevel);

    /**
     * @brief Gets the associated user ID.
     * @return The user ID as an integer.
     */
    int getUserId() const;

    /**
     * @brief Sets the associated user ID.
     * @param userId The new user ID.
     */
    void setUserId(int userId);

private:
    int m_loginid; ///< Unique login identifier.
    QString m_username; ///< Username of the user.
    QString m_password; ///< Password of the user.
    int m_accessLevel; ///< Access level of the user, determining permissions.
    int m_userid; ///< Associated user ID linking to the User class.
};

#endif // USERLOGIN_H
