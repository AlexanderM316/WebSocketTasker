#include "Server.h"
#include "TaskManager.h"
#include <QJsonObject>
#include <QJsonArray>

Server::Server(quint16 port, QObject *parent)
    : QObject(parent), m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Task Scheduler Server"), QWebSocketServer::NonSecureMode, this)),
      taskManager(new TaskManager()) {  // Заменяем ObjectManager на TaskManager
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
        qDebug() << "Server listening on port:" << port;
    }
}

void Server::onNewConnection() {
    QWebSocket *socket = m_pWebSocketServer->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &Server::onMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &Server::onSocketDisconnected);
}

void Server::onMessageReceived(const QString &message) {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (!client) {
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return sendError(client, -32700, "Parse error", QJsonValue());
    }

    QJsonObject json = doc.object();
    QString jsonrpc = json["jsonrpc"].toString();
    QString method = json["method"].toString();
    QJsonValue params = json["params"];
    QJsonValue id = json["id"];

    if (jsonrpc != "2.0") {
        return sendError(client, -32600, "Invalid Request", id);
    }

    QJsonObject result;

    if (method == "createTask") {  // Новый метод
        if (!params.isObject() || !params["name"].isString()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        QString name = params["name"].toString();
        taskManager->createTask(name);
        result["result"] = "Task created";
    } else if (method == "getTasks") { // Новый метод
        QJsonArray tasks;
        for (Task *task : taskManager->getTasks()) {
            QJsonObject taskJson;
            taskJson["id"] = task->getId();
            taskJson["name"] = task->getName();
            taskJson["completed"] = task->isCompleted();
            tasks.append(taskJson);
        }
        result["result"] = tasks;
    } else if (method == "updateTask") { // Новый метод
        if (!params.isObject() || !params["id"].isDouble() || !params["name"].isString()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int id = params["id"].toInt();
        QString name = params["name"].toString();
        taskManager->updateTask(id, name);
        result["result"] = "Task updated";
    } else if (method == "deleteTask") { // Новый метод
        if (!params.isObject() || !params["id"].isDouble()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int id = params["id"].toInt();
        taskManager->deleteTask(id);
        result["result"] = "Task deleted";
    } else if (method == "completeTask") { // Новый метод
        if (!params.isObject() || !params["id"].isDouble()) {
            return sendError(client, -32602, "Invalid params", id);
        }
        int id = params["id"].toInt();
        taskManager->completeTask(id);
        result["result"] = "Task marked as completed";
    } else {
        return sendError(client, -32601, "Method not found", id);
    }

    result["jsonrpc"] = "2.0";
    result["id"] = id;

    client->sendTextMessage(QJsonDocument(result).toJson(QJsonDocument::Compact));
}

void Server::sendError(QWebSocket *client, int code, const QString &message, const QJsonValue &id) {
    QJsonObject error;
    error["jsonrpc"] = "2.0";
    error["error"] = QJsonObject{{"code", code}, {"message", message}};
    if (!id.isNull()) {
        error["id"] = id;
    }
    client->sendTextMessage(QJsonDocument(error).toJson(QJsonDocument::Compact));
}

void Server::onSocketDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        socket->deleteLater();
    }
}
