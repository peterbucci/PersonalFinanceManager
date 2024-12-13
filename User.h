#ifndef USER_H
#define USER_H

#include <QString>

/**
 * @brief The User class represents a user in the system.
 */
class User {
public:
    /**
      * @brief Default constructor sets userid to 0 and names empty.
      */
    User() : m_userid(0) {}

    /**
     * @brief Constructs a User with the given details.
     * @param userId The unique ID of the user.
     * @param firstName The first name of the user.
     * @param lastName The last name of the user.
     * @param position The position/title of the user (e.g., "Admin", "User").
     * @return A User object.
     */
    User(int userId, const QString &firstName, const QString &lastName, const QString &position);

    /**
     * @brief Gets the user's unique ID.
     * @return The user ID as an integer.
     */
    int getUserId() const;

    /**
     * @brief Sets the user's unique ID.
     * @param userId The new user ID.
     */
    void setUserId(int userId);

    /**
     * @brief Gets the user's first name.
     * @return The user's first name as a QString.
     */
    QString getFirstName() const;

    /**
     * @brief Sets the user's first name.
     * @param firstName The new first name.
     */
    void setFirstName(const QString &firstName);

    /**
     * @brief Gets the user's last name.
     * @return The user's last name as a QString.
     */
    QString getLastName() const;

    /**
     * @brief Sets the user's last name.
     * @param lastName The new last name.
     */
    void setLastName(const QString &lastName);

    /**
     * @brief Gets the user's position/title.
     * @return The user's position as a QString.
     */
    QString getPosition() const;

    /**
     * @brief Sets the user's position/title.
     * @param position The new position.
     */
    void setPosition(const QString &position);

private:
    int m_userid; ///< Unique identifier for the user.
    QString m_firstname; ///< User's first name.
    QString m_lastname; ///< User's last name.
    QString m_position; ///< User's position or title.
};

#endif // USER_H
