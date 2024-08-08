#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "TaskManager.h"

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(quint16 port, QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onMessageReceived(const QString &message);
    void onSocketDisconnected();

private:
    void sendError(QWebSocket *client, int code, const QString &message, const QJsonValue &id);

    QWebSocketServer *m_pWebSocketServer;
    TaskManager *taskManager;  // Add this line to declare the TaskManager member
};

#endif // SERVER_H
