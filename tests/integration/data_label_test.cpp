#include "ServerSetup.h"
#include "base_integration_test.h"
#include "MainWindow.h"
#include "ValveControlView.h"
#include "components/DataLabel.h"
#include <QDebug>
#include <qtestcase.h>
#include <qtestsupport_core.h>
#include "Setup.h"

class DataLabelTest : public BaseIntegrationTest {
    Q_OBJECT



private slots:
    void test_dataLabel_subscription() {
        DataLabel label(GUI_FIELD::CHAMBER_PRESSURE);
        
        QVERIFY(subscribeSpy->wait(500));
        QVERIFY(!subscribeSpy->isEmpty());

        
                
        auto args = subscribeSpy->takeLast();
        QJsonObject subRequest = args[0].toJsonObject();
        QVERIFY(subRequest.contains("payload"));
        
        QJsonObject payload = subRequest["payload"].toObject();
        QCOMPARE(payload["field"].toInt(), static_cast<int>(GUI_FIELD::CHAMBER_PRESSURE));
    }

    void test_server_response_to_client() {
        // Create a data label that will subscribe
        DataLabel label(GUI_FIELD::FUEL_PRESSURE);

        // Wait for subscription
        QVERIFY(subscribeSpy->wait(500));

        // Simulate server sending data (now accessible via friend class)
        QJsonObject testData;
        testData[QString::number(GUI_FIELD::FUEL_PRESSURE)] = "42.5";
        server->updateSubscriptions(testData);

        // Give some time for the update to propagate
        QTest::qWait(100);

        // Verify the label was updated
        QCOMPARE(label.text(), QString("42.5"));
    }


};

QTEST_MAIN(DataLabelTest)
#include "data_label_test.moc"
