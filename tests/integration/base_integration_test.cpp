#include "base_integration_test.h"
// #include "ServerSetup.h"
#include "Server/src/ServerSetup.h"
#include "Setup.h"
#include <QDebug>

void BaseIntegrationTest::initTestCase() {

  // Create QApplication if not exists
  if (!QApplication::instance()) {
    static int argc = 1;
    static char *argv[] = {(char *)"TestApp", nullptr};
    app = new QApplication(argc, argv);
    qDebug() << "Created QApplication";
  } else {
    qDebug() << "QApplication already exists";
  }

  qDebug() << "[DEBUG] Current Working Directory (CWD) is:"
           << QDir::currentPath();
  qDebug() << "[DEBUG] Application Directory Path is:"
           << QCoreApplication::applicationDirPath();

  QString project_root_path = TEST_SOURCE_DIR;

  // 2. Build the absolute path to the keys file
  QString keys_file_path = project_root_path + "/Server/src/auth_key.json";

  auth::loadKeyFromFile(project_root_path + "/GUI/src/.key");
  RequestBuilder::authorizationKey = auth::key;

  auth_server::loadKeysFromFile(keys_file_path);

  // Start the real server
  server = new Server(this);
  bool listening = server->listen(QHostAddress::LocalHost, TEST_PORT);
  qDebug() << "Server listening:" << listening;
  QVERIFY(listening);

  // Set up spies on the server's RequestHandler
  RequestHandler *requestHandler = server->findChild<RequestHandler *>();
  QVERIFY(requestHandler != nullptr);

  qDebug() << "Setting up signal spies...";
  postSpy =
      new QSignalSpy(requestHandler, SIGNAL(post(QJsonObject, QTcpSocket *)));
  subscribeSpy = new QSignalSpy(requestHandler,
                                SIGNAL(subscribe(QJsonObject, QTcpSocket *)));

  // Replace the global ClientManager with test version
  originalClientManager = std::move(MainWindow::clientManager);
  MainWindow::clientManager =
      std::make_unique<ClientManager>(nullptr, "127.0.0.1", TEST_PORT);

  // Wait for connection to establish
  QTest::qWait(300);
}

void BaseIntegrationTest::init() {
  // Clear spies before each test
  if (postSpy) {
    postSpy->clear();
  }
  if (subscribeSpy) {
    subscribeSpy->clear();
    qDebug() << "Cleared all subscibe signals";
  }
}

// ... rest of your methods

void BaseIntegrationTest::cleanupTestCase() {
  // Restore original ClientManager
  MainWindow::clientManager = std::move(originalClientManager);

  delete postSpy;
  delete subscribeSpy;

  if (server) {
    server->close();
    delete server;
  }

  if (app) {
    delete app;
  }
}

bool BaseIntegrationTest::waitForPost(int timeoutMs) {
  return postSpy->wait(timeoutMs);
}

QJsonObject BaseIntegrationTest::getLastPostCommand() {
  if (postSpy->isEmpty()) {
    return QJsonObject(); // Return empty object
  }
  auto args = postSpy->takeLast();
  return args[0].toJsonObject();
}

bool BaseIntegrationTest::hasPostCommands() const {
  return !postSpy->isEmpty();
}

int BaseIntegrationTest::getPostCommandCount() const {
  return postSpy->count();
}

void BaseIntegrationTest::verifyCommand(const QJsonObject &command,
                                        GUI_FIELD expectedField,
                                        int expectedOrder) {
  QVERIFY(command.contains("payload"));
  QJsonObject payload = command["payload"].toObject();
  QVERIFY(payload.contains("cmd"));
  QVERIFY(payload.contains("cmd_order"));
  QCOMPARE(payload["cmd"].toInt(), static_cast<int>(expectedField));
  QCOMPARE(payload["cmd_order"].toInt(), expectedOrder);
}

#include "base_integration_test.moc"
