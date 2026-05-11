#include "AuthManager.h"
#include <QRegularExpression>

AuthManager::AuthManager(QObject* parent)
    : QObject(parent) {
}

bool AuthManager::validateUsername(const QString& username, QString* errorOut) {
    const auto setErr = [&](const QString& msg) {
        if (errorOut) *errorOut = msg;
    };

    if (username.isEmpty()) {
        setErr(QStringLiteral("Username cannot be empty."));
        return false;
    }
    if (username.length() < MIN_USERNAME_LEN) {
        setErr(QStringLiteral("Username must be at least %1 characters.")
                   .arg(MIN_USERNAME_LEN));
        return false;
    }
    if (username.length() > MAX_USERNAME_LEN) {
        setErr(QStringLiteral("Username cannot exceed %1 characters.")
                   .arg(MAX_USERNAME_LEN));
        return false;
    }
    static const QRegularExpression allowed(QStringLiteral("^[A-Za-z0-9_-]+$"));
    if (!allowed.match(username).hasMatch()) {
        setErr(QStringLiteral("Username may contain only letters, digits, "
                              "underscores and hyphens."));
        return false;
    }

    if (errorOut) errorOut->clear();
    return true;
}

bool AuthManager::validatePassword(const QString& password, QString* errorOut) {
    const auto setErr = [&](const QString& msg) {
        if (errorOut) *errorOut = msg;
    };

    if (password.isEmpty()) {
        setErr(QStringLiteral("Password cannot be empty."));
        return false;
    }
    if (password.length() < MIN_PASSWORD_LEN) {
        setErr(QStringLiteral("Password must be at least %1 characters.")
                   .arg(MIN_PASSWORD_LEN));
        return false;
    }
    if (password.length() > MAX_PASSWORD_LEN) {
        setErr(QStringLiteral("Password cannot exceed %1 characters.")
                   .arg(MAX_PASSWORD_LEN));
        return false;
    }
    if (password.trimmed() != password) {
        setErr(QStringLiteral("Password cannot start or end with whitespace."));
        return false;
    }

    if (errorOut) errorOut->clear();
    return true;
}

bool AuthManager::attemptLogin(const QString& username, const QString& password) {
    QString err;

    if (!validateUsername(username, &err)) {
        emit validationError(QStringLiteral("username") + ": " + err);
        emit loginFailed(err);
        return false;
    }
    if (!validatePassword(password, &err)) {
        emit validationError(QStringLiteral("password") + ": " + err);
        emit loginFailed(err);
        return false;
    }

    m_currentUser = username;
    return true;
}

void AuthManager::logout() {
    if (!m_loggedIn) return;

    const QString user = m_currentUser;
    m_loggedIn = false;
    m_currentUser.clear();
}

void AuthManager::onServerLoginResponse(bool success, const QString& reason) {
    if (success) {
        m_loggedIn = true;
        emit loginSucceeded(m_currentUser);
    } else {
        m_currentUser.clear();
        m_loggedIn = false;
        emit loginFailed(reason.isEmpty()
                             ? QStringLiteral("Login rejected by server.")
                             : reason);
    }
}

