#include <../include/chatserver.h>
#include <cstdio>
#include <QtCore>
#include <QtWebSockets>
#include <qobject.h>
#include <qwebsocket.h>
#include <qwebsocketserver.h>
#include <utility>

using namespace std;

static QString getIdentifier(QWebSocket *peer)
{
    return QStringLiteral("%1:%2")
        .arg(peer->peerAddress().toString(),
             QString::number(peer->peerPort()));
}

ChatServer::ChatServer(quint16 port, QObject *parent) : QObject(parent),
                                                        m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Chat Server"),
                                                                                                QWebSocketServer::NonSecureMode, this))

{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        QTextStream(stdout) << "Chat Server Listening on port:  " << port << '\n';
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &ChatServer::onNewConnection);
    }
}

// Close connection
ChatServer::~ChatServer()
{
    m_pWebSocketServer->close();
}

// Connect user
void ChatServer::onNewConnection()
{

    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    QTextStream(stdout) << getIdentifier(pSocket) << " connected";
    pSocket->setParent(this);

    connect(pSocket, &QWebSocket::textMessageReceived, this, &ChatServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &ChatServer::socketDisconnected);

    m_clients << pSocket;
}

// Process and send message
void ChatServer::processMessage(const QString &message)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    for (QWebSocket *pClient : std::as_const(m_clients))
    {
        if (pClient != pSender)
        {
            pClient->sendTextMessage(message);
        }
    }
}

// Disconnect
void ChatServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QTextStream(stdout) << getIdentifier(pClient) << " disconnected";
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
