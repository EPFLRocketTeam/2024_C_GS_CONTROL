#include "commons/ERT_RF_Protocol_Interface/Protocol.h"
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

    ui_elements::init_views();
    MainWindow window(
        nullptr, &ui_elements::controlMap, ui_elements::leftPlaceholder,
        ui_elements::middlePlaceholder, ui_elements::rightPlaceholder);

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
    ui_elements::init_views();

    MainWindow window(
        nullptr, &ui_elements::controlMap, ui_elements::leftPlaceholder,
        ui_elements::middlePlaceholder, ui_elements::rightPlaceholder);
    Q_INIT_RESOURCE(resources);
    window.show();
    int tmp = QTest::qWaitForWindowExposed(&window);

    // 2) Find all ValveControlButton children
    auto valves = window.findChildren<ValveButton *>();
    qDebug() << "Found valve buttons:" << valves.size();
    QVERIFY(!valves.isEmpty());

    // 3) Find the target button for MAIN_LOX
    ValveButton *targetVcb = nullptr;
    for (auto *vcb : valves) {
      if (vcb &&
          vcb->fieldSensivity() == static_cast<int>(GUI_FIELD::MAIN_LOX)) {
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
    ValveButton *tb = targetVcb;
    QVERIFY(tb);
    QVERIFY(tb != nullptr);
    QVERIFY(tb->isVisible());
    QVERIFY(tb->isEnabled());
    QVERIFY(tb->window()->isActiveWindow());

    // 5) Ensure widgets are properly set up
    tb->ensurePolished();
    QApplication::processEvents();
    QVERIFY(tb->isVisible());
    QVERIFY(tb->isEnabled());
    // 6) Clear any previous server signals
    postSpy->clear();
    QVERIFY(postSpy->isValid());
    QVERIFY(postSpy->isEmpty());

    qDebug() << "Before Click";
    QTimer::singleShot(100, [&]() {
    // Find the confirmation dialog after it appears
    QMessageBox *msgBox = nullptr;
    for (int attempts = 0; attempts < 50; ++attempts) {
        QTest::qWait(10);
        
        // Search through all top-level widgets
        for (QWidget *widget : QApplication::topLevelWidgets()) {
            msgBox = qobject_cast<QMessageBox *>(widget);
            if (msgBox && msgBox->isVisible()) {
                qDebug() << "Found QMessageBox:" << msgBox;
                break;
            }
        }
        if (msgBox) break;
    }
    
    if (msgBox) {
        QAbstractButton *yesBtn = msgBox->button(QMessageBox::Yes);
        if (yesBtn) {
            qDebug() << "Clicking Yes button...";
            QTest::mouseClick(yesBtn, Qt::LeftButton);
        }
    }
});
    // 7) Click the ValveControlButton
    QTest::mouseClick(targetVcb, Qt::LeftButton, Qt::NoModifier, targetVcb->rect().center());

    qDebug() << "After Click";
    // 8) Wait for and handle the confirmation dialog
    QVERIFY(waitForPost(2000));
    qDebug() << "Received a post ";
    // 9) Debug the spy contents
    // DumpSpySignal(postSpy);

    QVERIFY2(hasPostCommand(GUI_FIELD::MAIN_LOX, 0),
             "Expected MAIN_LOX toggle-on command not found");
    
    return;

  }
};

QTEST_MAIN(FullIntegrationTest)
#include "full_integration_test.moc"
