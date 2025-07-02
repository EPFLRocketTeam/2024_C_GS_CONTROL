#ifndef FILELOCATION_H
#define FILELOCATION_H

#include <QApplication>

const QString appDir = QCoreApplication::applicationDirPath();
const std::string LOG_FILE_PATH =  appDir.toStdString() + "../Log/firehorn2.logs";

#endif // FILELOCATION_H
