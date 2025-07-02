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
    void test_dataLabel_subscription() {
        DataLabel label(GUI_FIELD::CHAMBER_PRESSURE);
        
        QVERIFY(subscribeSpy->wait(500));
        QVERIFY(!subscribeSpy->isEmpty());

        
        {
        qDebug().noquote() << "\n--- Inline subscribeSpy Dump ---";
        int signalCount = subscribeSpy->count();
        qDebug() << "Total signals captured:" << signalCount;

        if (signalCount == 0) {
            qDebug() << "No signals were captured by the spy.";
        } else {
            // Iterate through all captured signals without removing them
            for (int i = 0; i < signalCount; ++i) {
                const QVariantList& signalArgs = subscribeSpy->at(i);
                qDebug().noquote() << QString("--- Signal %1 of %2 ---").arg(i + 1).arg(signalCount);

                if (signalArgs.isEmpty()) {
                    qDebug() << "  (Signal had no arguments)";
                    continue;
                }

                // Argument 0: The QJsonObject
                QJsonObject subRequest = signalArgs.at(0).toJsonObject();
                qDebug() << "  Payload:" << subRequest;

                // Argument 1: The QTcpSocket*
                if (signalArgs.count() > 1) {
                    QTcpSocket* socket = qobject_cast<QTcpSocket*>(signalArgs.at(1).value<QObject*>());
                    if (socket) {
                        qDebug() << "  From socket descriptor:" << socket->socketDescriptor();
                    } else {
                        qDebug() << "  From socket: (null)";
                    }
                }
            }
        }
        qDebug().noquote() << "--- End of Inline Dump ---\n";
    }
        
        auto args = subscribeSpy->takeLast();
        QJsonObject subRequest = args[0].toJsonObject();
        QVERIFY(subRequest.contains("payload"));
        
        QJsonObject payload = subRequest["payload"].toObject();
        QCOMPARE(payload["field"].toInt(), static_cast<int>(GUI_FIELD::CHAMBER_PRESSURE));
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
