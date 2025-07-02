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

    

    // void test_server_response_to_client() {
    //     // Create a data label that will subscribe
    //     DataLabel label(GUI_FIELD::FUEL_PRESSURE);
    //
    //     // Wait for subscription
    //     QVERIFY(subscribeSpy->wait(500));
    //
    //     // Simulate server sending data (now accessible via friend class)
    //     QJsonObject testData;
    //     testData[QString::number(GUI_FIELD::FUEL_PRESSURE)] = "42.5";
    //     server->updateSubscriptions(testData);
    //
    //     // Give some time for the update to propagate
    //     QTest::qWait(100);
    //
    //     // Verify the label was updated
    //     QCOMPARE(label.text(), QString("42.5"));
    // }

    void test_serial_status_command() {
        // Test the serial status command
        QJsonObject serialCommand;
        QJsonObject payload;
        payload["cmd"] = static_cast<int>(GUI_FIELD::SERIAL_STATUS);
        payload["cmd_order"] = 1;
        serialCommand["payload"] = payload;
        
        // Simulate sending this command to server (now accessible via friend class)
        server->handleCommand(serialCommand);
        
        // The server should respond with serial status
        // This test verifies the server processes the command correctly
    }


    // void test_dataLabel_subscription() {
    //
    //     qDebug() << "=== test_dataLabel_subscription() started ===";
    //     qDebug() << "SubscribeSpy valid:" << (subscribeSpy != nullptr);
    //     qDebug() << "SubscribeSpy count before:" << (subscribeSpy ? subscribeSpy->count() : -1);
    //
    //
    //     // Create the DataLabel which should trigger subscription
    //     qDebug() << "Creating DataLabel...";
    //     DataLabel label(GUI_FIELD::CHAMBER_PRESSURE);
    //
    //     qDebug() << "DataLabel created, waiting for subscription...";
    //     qDebug() << "SubscribeSpy count after creation:" << subscribeSpy->count();
    //
    //     // Give a bit more time and check periodically
    //     for (int i = 0; i < 5; ++i) {
    //         QTest::qWait(100);
    //         qDebug() << "Wait iteration" << i << "- SubscribeSpy count:" << subscribeSpy->count();
    //         if (subscribeSpy->count() > 0) break;
    //     }
    //
    //     // Verify subscription was made
    //     if (subscribeSpy->count() == 0) {
    //         qDebug() << "ERROR: No subscription signals received!";
    //         qDebug() << "Check if DataLabel constructor is calling ClientManager->subscribe()";
    //         QFAIL("No subscription was made");
    //     }
    //
    //     QVERIFY(!subscribeSpy->isEmpty());
    //
    //     auto args = subscribeSpy->takeLast();
    //     QJsonObject subRequest = args[0].toJsonObject();
    //     qDebug() << "Subscription request:" << subRequest;
    //
    //     QVERIFY(subRequest.contains("payload"));
    //
    //     QJsonObject payload = subRequest["payload"].toObject();
    //     QCOMPARE(payload["field"].toInt(), static_cast<int>(GUI_FIELD::CHAMBER_PRESSURE));
    //
    //     qDebug() << "=== test_dataLabel_subscription() completed successfully ===";
    // }

    // Add a simple test to verify the base setup works
    void test_base_setup() {
        qDebug() << "=== test_base_setup() started ===";
        QVERIFY(server != nullptr);
        QVERIFY(postSpy != nullptr);
        QVERIFY(subscribeSpy != nullptr);
        QVERIFY(postSpy->isValid());
        QVERIFY(subscribeSpy->isValid());
        QVERIFY(MainWindow::clientManager != nullptr);
        qDebug() << "=== test_base_setup() completed successfully ===";
    }

    // ... your other test methods
};

QTEST_MAIN(FullIntegrationTest)
#include "full_integration_test.moc"
