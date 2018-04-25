#include "TcpSocketAsyncServer.hpp"

TcpSocketAsyncServer::TcpSocketAsyncServer( const quint16& port,
  const std::string& instanceId, const std::string& file, QObject *parent )
  : QTcpServer( parent )
  , _file( file )
{
  _owner = toString( sp1common::ApplicationType::CLINT )
    + instanceId;

  /*_serverSocket = new QTcpServer( this );

  connect(_serverSocket, SIGNAL(newConnection()),
    this, SLOT(newConnection()));

  if ( !_serverSocket->listen( QHostAddress::Any, port ) )
  {
    std::cerr << "Server cannot start on port " << port << std::endl;
  }
  else
  {
    std::cout << "TcpServer started on port " << port << " successfully." << std::endl;
  }*/

  connect(this, SIGNAL(newConnection()),
    this, SLOT(newConnection()));

  if ( !this->listen( QHostAddress::Any, port ) )
  {
    std::cerr << "Server cannot start on port " << port << std::endl;
  }
  else
  {
    std::cout << "TcpServer started on port " << port << " successfully." << std::endl;
  }
}

TcpSocketAsyncServer::~TcpSocketAsyncServer( )
{
  //_serverSocket->close( );
  this->close( );
}

void TcpSocketAsyncServer::newConnection()
{
  /*QTcpSocket* socket = _serverSocket->nextPendingConnection();
  if( socket )
  {
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), socket , SLOT(deleteLater()));
  }*/
    QTcpSocket* socket = this->nextPendingConnection();
    if( socket )
    {
      connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
      connect(socket, SIGNAL(disconnected()), socket , SLOT(deleteLater()));
    }
}

void TcpSocketAsyncServer::readyRead()
{
  QTcpSocket* socket = dynamic_cast<QTcpSocket*>(sender());
  if(socket)
  {
    std::cout << "reading data" << std::endl;

    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    buffer.write(socket->readAll());

    QString message = QString::fromUtf8(buffer.data( ) );
    std::cout << "message: " << message.toStdString( ) << std::endl;

    if ( ( message == "file" ) && ( !_file.empty( ) ) )
    {
      const char* response = sp1common::Files::readRawCsv( _file ).c_str( );
      socket->write(response);
    }
    else
    {
      manageMessage( message.toStdString( ) );
    }
  }
}

void TcpSocketAsyncServer::manageMessage( const std::string& str )
{
  std::cout << "MANAGE MESSAGE" << std::endl;

  if ( str == CLOSE_SERVER_MESSAGE )
  {
    exit(0);
  }
  else
  {
    std::cout << str << std::endl;
    std::string key;
    std::vector<std::string> ids_vector1 = manco::ZeqManager::split(str,"&");
    std::vector<std::string> ids_vector =
    manco::ZeqManager::split(ids_vector1[4],";");

    unsigned int color_red= atoi(ids_vector1[1].c_str());
    unsigned int color_green= atoi(ids_vector1[2].c_str());
    unsigned int color_blue= atoi(ids_vector1[3].c_str());

    std::string key_name = manco::ZeqManager::getKeyOwner(ids_vector1[0],
      _owner );

    manco::ZeqManager::instance().publishSyncGroup( key_name, ids_vector1[0],
      _owner, ids_vector, color_red, color_green, color_blue);
  }
}
