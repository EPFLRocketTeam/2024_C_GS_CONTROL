#ifndef BASE_INTEGRATION_TEST_H
#define BASE_INTEGRATION_TEST_H

#include <QApplication>
#include <QtTest>
#include <QSignalSpy>
#include <QTimer>
#include <QTcpSocket>
#include <memory>

#include "Server.h"
#include "ClientManager.h"
#include "MainWindow.h"
#include "Setup.h"

class BaseIntegrationTest : public QObject {
    Q_OBJECT

protected:
    QApplication* app = nullptr;
    Server* server = nullptr;
    std::unique_ptr<ClientManager> originalClientManager;
    QSignalSpy* postSpy = nullptr;
    QSignalSpy* subscribeSpy = nullptr;
    
    static constexpr int TEST_PORT = 12345; // Different from default to avoid conflicts

protected slots:
    virtual void initTestCase();
    virtual void cleanupTestCase();
    virtual void init();

protected:
    // Helper methods
    bool waitForPost(int timeoutMs = 1000);
    QJsonObject getLastPostCommand();
    bool hasPostCommands() const;
    int getPostCommandCount() const;
    void verifyCommand(const QJsonObject& command, GUI_FIELD expectedField, int expectedOrder);
    bool hasPostCommand(GUI_FIELD f, int order);
};

#endif // BASE_INTEGRATION_TEST_H
