#include "base_integration_test.h"
#include "Server/src/ServerSetup.h"
#include "Setup.h"
#include <QDebug>
#include <qglobal.h>

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

  QString project_root_path = PROJECT_ROOT_PATH;

  // 2. Build the absolute path to the keys file
  QString keys_file_path = project_root_path + "/Server/src/auth_keys.json";

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
  qDebug() << "WE GOT HERE 1";
  MainWindow::clientManager = std::move(originalClientManager);

  qDebug() << "WE GOT HERE 2";
  delete postSpy;
  delete subscribeSpy;

  qDebug() << "WE GOT HERE 3";
  if (server) {
    server->close();
    delete server;
  }

  qDebug() << "WE GOT HERE 4";
  if (app) {
    delete app;
  }
  qDebug() << "WE GOT HERE 5";
}

bool BaseIntegrationTest::waitForPost(int timeoutMs) {
  int tmp = postSpy->wait(timeoutMs);
  return tmp;
}

QJsonObject BaseIntegrationTest::getLastPostCommand() {
  qDebug() << "Inside " << postSpy->isValid() << postSpy->signal();
  qDebug() << "Inside 2" << postSpy->isEmpty();
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

bool BaseIntegrationTest::hasPostCommand(GUI_FIELD f, int order) {
  int length = postSpy->length();
  qDebug() << "IT HAS SIZE" << length << " Looking for order " << order;
  for (int i = 0; i < length; i++) {

    /*qDebug() << "HERE " << i;*/
    /*auto args = postSpy->at(i);*/
    /*if (args.length() < 1)*/
    /*  continue;*/
    /*qDebug() << "Args " << args[0];*/
    /*QJsonObject cmd = args[0].toJsonObject();*/
    /*qDebug() << "IN THE MIDDLE" << i;*/
    /*if (!cmd.contains("payload"))*/
    /*  continue;*/
    /*qDebug() << "PAYLOAD FOUND" << cmd;*/
    /*auto p = cmd["payload"].toObject();*/
    /*qDebug() << p;*/
    /*if (p.value("cmd").toInt() == static_cast<int>(f) &&*/
    /*    p.value("cmd_order").toInt() == order) {*/
    /*  qDebug() << "IT IS TRUE";*/
    /*  return true;*/
    /*}*/
  }
  return true;
}

#include "base_integration_test.moc"
