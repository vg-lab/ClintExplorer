#ifndef TCPSOCKETASYNCSERVER_H
#define TCPSOCKETASYNCSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QBuffer>
#include <iostream>
#include <string>
#include <manco/manco.h>
#include <sp1common/sp1common.h>

#define CLOSE_SERVER_MESSAGE "exit"

class TcpSocketAsyncServer : public QTcpServer
{
  Q_OBJECT

  public:
    explicit TcpSocketAsyncServer( const quint16& port,
      const std::string& instanceId, QObject* parent = 0 );
    void start( quint16 port );
    ~TcpSocketAsyncServer();

  Q_SIGNALS:
    void closed();

  signals:

  public slots:
    void newConnection();

  private slots:
    void readyRead();

  private:
    //QTcpServer* _serverSocket;
    void manageMessage( const std::string& str );
    std::string _owner;
};

#endif
