#ifndef UTILS_H
#define UTILS_H

#include <QPair>
#include <QString>

QPair<QString, QString> ParseTestData(const QString& rawData) {
    int delimiterIndex = rawData.indexOf('|');
    if (delimiterIndex == -1) {
        return qMakePair(QString(), QString());
    }

    QString name = rawData.left(delimiterIndex).trimmed();
    QString json = rawData.mid(delimiterIndex + 1).trimmed();
    return qMakePair(name, json);
}

#endif // UTILS_H
