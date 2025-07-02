#include "ServerSetup.h"
#include "base_integration_test.h"
#include "MainWindow.h"
#include "ValveControlView.h"
#include "components/DataLabel.h"
#include <QDebug>
#include <qtestcase.h>
#include <qtestsupport_core.h>
#include "Setup.h"

class FullIntegrationTest : public BaseIntegrationTest {
    Q_OBJECT



private slots:
    void test_mainWindow_creation() {
        MainWindow window;
        window.show();

        // Verify window was created
        QVERIFY(window.isVisible());

        // Find some key components
        auto* valveControlView = window.findChild<ValveControlView*>();
        QVERIFY(valveControlView != nullptr);
    }


    void test_serial_status_command() {
        // Test the serial status command
        QJsonObject serialCommand;
        QJsonObject payload;
        payload["cmd"] = static_cast<int>(GUI_FIELD::SERIAL_STATUS);
        payload["cmd_order"] = 1;
        serialCommand["payload"] = payload;

        // Simulate sending this command to server (now accessible via friend class)
        server->handleCommand(serialCommand);
    }
};

QTEST_MAIN(FullIntegrationTest)
#include "full_integration_test.moc"
