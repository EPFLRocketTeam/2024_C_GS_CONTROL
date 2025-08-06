#include "MainWindow.h"
#include "Server/src/ServerSetup.h"
#include "Setup.h"
#include "ValveControlView.h"
#include "base_integration_test.h"
#include "components/DataLabel.h"
#include "components/ValveControlButton.h"
#include "tests/helpers/LogSpySignals.h"
#include <QDebug>
#include <QElapsedTimer>
#include <qglobal.h>
#include <qmetaobject.h>
#include <qnamespace.h>
#include <qtestcase.h>
#include <qtestmouse.h>
#include <qtestsupport_core.h>

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
    int tmp = QTest::qWaitForWindowExposed(&window);

    qDebug() << "Before wiat";
    // Give some time for the window to fully initialize
    qDebug() << "After wiat";

    // 2) Find all ValveControlButton children
    auto valves = window.findChildren<ValveControlButton *>();
    qDebug() << "Found valve buttons:" << valves.size();
    QVERIFY(!valves.isEmpty());

    // 3) Find the target button for MAIN_LOX
    ValveControlButton *targetVcb = nullptr;
    for (auto *vcb : valves) {
      auto *tb = vcb->findChild<ToggleButton *>();
      if (tb &&
          tb->fieldSensitivity() == static_cast<int>(GUI_FIELD::MAIN_LOX)) {
        qDebug() << "Found MAIN_LOX button:" << vcb;
        targetVcb = vcb;
        break;
      }
    }

    // Fallback to first button if MAIN_LOX not found
    if (!targetVcb) {
      targetVcb = valves.first();
      qDebug() << "Using first button as fallback:" << targetVcb;
    }
    QVERIFY(targetVcb);

    // 4) Get the ToggleButton child
    auto *tb = targetVcb->findChild<ToggleButton *>();
    QVERIFY(tb);

    // 5) Ensure widgets are properly set up
    targetVcb->ensurePolished();
    tb->ensurePolished();
    QVERIFY(targetVcb->isVisible());
    QVERIFY(targetVcb->isEnabled());
    QVERIFY(tb->isVisible());
    QVERIFY(tb->isEnabled());

    // Debug info
    qDebug() << "ToggleButton geometry:" << tb->geometry();
    qDebug() << "ValveControlButton geometry:" << targetVcb->geometry();
    qDebug() << "ToggleButton field sensitivity:" << tb->fieldSensitivity();

    // 6) Clear any previous server signals
    postSpy->clear();
    QVERIFY(postSpy->isValid());
    QVERIFY(postSpy->isEmpty());

    // 7) Click the ValveControlButton
    QTest::mouseClick(tb, Qt::LeftButton, Qt::NoModifier, tb->rect().center());

    // QTest::mousePress(tb, Qt::LeftButton, Qt::NoModifier,
    //                   tb->rect().center());
    qDebug() << "AFTER";
    // 8) Wait for the server's post signal
    QVERIFY2(waitForPost(2000), "No post signal received within timeout");

    // 9) Debug the spy contents
    // DumpSpySignal(postSpy);

    QVERIFY2(hasPostCommand(GUI_FIELD::MAIN_LOX, 1),
             "Expected MAIN_LOX toggle-on command not found");
  }
};

QTEST_MAIN(FullIntegrationTest)
#include "full_integration_test.moc"
