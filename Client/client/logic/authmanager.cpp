#include "authmanager.h"
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
    // Allow letters, digits, underscore, hyphen. No whitespace, no symbols
    // that could break the wire protocol.
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
    // Reject control characters / whitespace at edges.
    if (password.trimmed() != password) {
        setErr(QStringLiteral("Password cannot start or end with whitespace."));
        return false;
    }

    if (errorOut) errorOut->clear();
    return true;
}

void AuthManager::attemptLogin(const QString& username, const QString& password) {
    // Validate before we ever touch the network.
    QString err;

    if (!validateUsername(username, &err)) {
        emit validationError(QStringLiteral("username"), err);
        emit loginFailed(err);
        return;
    }
    if (!validatePassword(password, &err)) {
        emit validationError(QStringLiteral("password"), err);
        emit loginFailed(err);
        return;
    }

    // Input looks fine — hand off to the network layer.
    // We optimistically store the username; it only "sticks" if the
    // server replies with success (see onServerLoginResponse).
    m_currentUser = username;
    emit authRequested(username, password);
}

void AuthManager::logout() {
    if (!m_loggedIn) return;

    const QString user = m_currentUser;
    m_loggedIn = false;
    m_currentUser.clear();

    emit logoutRequested(user);
    emit loggedOut();
}

void AuthManager::onServerLoginResponse(bool success, const QString& reason) {
    if (success) {
        m_loggedIn = true;
        emit loginSucceeded(m_currentUser);
    } else {
        // Roll back the optimistic username we stored in attemptLogin.
        m_currentUser.clear();
        m_loggedIn = false;
        emit loginFailed(reason.isEmpty()
                             ? QStringLiteral("Login rejected by server.")
                             : reason);
    }
}

void AuthManager::onConnectionLost() {
    if (!m_loggedIn) return;
    m_loggedIn = false;
    m_currentUser.clear();
    emit loggedOut();
}
