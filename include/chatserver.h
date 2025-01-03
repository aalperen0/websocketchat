#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QFile)

class ChatServer : public QObject
{

    Q_OBJECT

public:
    explicit ChatServer(quint16 port, QObject *parent = nullptr);
    ~ChatServer() override;

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif
