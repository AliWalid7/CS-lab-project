#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QString>

class AuthManager : public QObject {
    Q_OBJECT

public:
    explicit AuthManager(QObject* parent = nullptr);

    bool isLoggedIn() const { return m_loggedIn; }
    QString currentUser() const { return m_currentUser; }

    static bool validateUsername(const QString& username, QString* errorOut = nullptr);
    static bool validatePassword(const QString& password, QString* errorOut = nullptr);

public slots:
    void attemptLogin(const QString& username, const QString& password);
    void logout();
    void onServerLoginResponse(bool success, const QString& reason);
    void onConnectionLost();

signals:
    void authRequested(const QString& username, const QString& password);
    void logoutRequested(const QString& username);
    void loginSucceeded(const QString& username);
    void loginFailed(const QString& reason);
    void loggedOut();
    void validationError(const QString& field, const QString& reason);

private:
    bool m_loggedIn = false;
    QString m_currentUser;

    static constexpr int MIN_USERNAME_LEN = 3;
    static constexpr int MAX_USERNAME_LEN = 20;
    static constexpr int MIN_PASSWORD_LEN = 4;
    static constexpr int MAX_PASSWORD_LEN = 64;
};

#endif // AUTHMANAGER_H
