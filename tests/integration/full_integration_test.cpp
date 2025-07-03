#include "MainWindow.h"
#include "Server/src/ServerSetup.h"
#include "Setup.h"
#include "ValveControlView.h"
#include "base_integration_test.h"
#include "components/DataLabel.h"
#include "components/ValveControlButton.h"
#include <QDebug>
#include <qglobal.h>
#include <qtestcase.h>
#include <qtestsupport_core.h>
#include "tests/helpers/LogSpySignals.h"

class FullIntegrationTest : public BaseIntegrationTest {
  Q_OBJECT

private slots:
  void test_mainWindow_creation() {
    MainWindow window;
    Q_INIT_RESOURCE(resources);
    window.show();

    // Verify window was created
    QVERIFY(window.isVisible());

    // Find some key components
    auto *valveControlView = window.findChild<ValveControlView *>();
    QVERIFY(valveControlView != nullptr);
  }

  void test_mainWindow_buttonClickSendsCommand() {
    // 1) Create & show the main window
    MainWindow window;
    Q_INIT_RESOURCE(resources);
    window.show();
    // Optionally wait until Qt considers it exposed:
    QTest::qWaitForWindowExposed(&window);

    // 2) Find all ValveControlButton children
    auto valves = window.findChildren<ValveControlButton *>();
    qDebug() << "TTTTTTTTTT valve " << valves << "\n";
    QVERIFY(!valves.isEmpty());

    // 3) Pick the one for MAIN_LOX
    ValveControlButton *targetVcb = nullptr;
    for (auto *vcb : valves) {
      if (vcb->findChild<ToggleButton *>()->fieldSensitivity() ==
          static_cast<int>(GUI_FIELD::MAIN_LOX)) {
        qDebug() << "TTTTTTTTTT FOUNDDDD " << vcb << "\n";
        targetVcb = vcb;
        break;
      }
    }

    // If fieldSensitivity isn't exposed as a property, fallback to first:
    if (!targetVcb)
      targetVcb = valves.first();
    QVERIFY(targetVcb);

    // 4) Get its ToggleButton child
    auto *tb = targetVcb->findChild<ToggleButton *>();
    QVERIFY(tb);

    // 5) Clear any previous server signals
    postSpy->clear();
    // get the child’s center in global screen coordinates
QPoint global = tb->mapToGlobal(tb->rect().center());
// now convert that back into the MainWindow’s local coords
QPoint windowClick = window.mapFromGlobal(global);
QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier, windowClick);        // QTest::mouseClick(tb, Qt::LeftButton);
    QMetaObject::invokeMethod(
        tb,
        "setChecked",
        Qt::QueuedConnection,
        Q_ARG(bool, true)
    );

    // waitForPost(1000);
    qDebug() << "OUT OF IT" << postSpy->isValid();

    // 7) Wait for the server's post(...) signal
    QVERIFY(waitForPost(1300));

    DumpSpySignal(postSpy);

    // 8) Verify the JSON that arrived
    QJsonObject cmd = getLastPostCommand();
    verifyCommand(cmd,
                  GUI_FIELD::MAIN_LOX, // or whatever field you expect
                  1                    // toggled => 1
    );
  }
};

QTEST_MAIN(FullIntegrationTest)
#include "full_integration_test.moc"
