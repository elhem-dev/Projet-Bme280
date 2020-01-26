#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>
#include <QAbstractSocket>
#include<QFile>
#include <QJsonDocument>
#include <QList>
#include <QJsonObject>
#include <QVariantMap>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _server(this)
{
    ui->setupUi(this);
    _server.listen(QHostAddress::Any, 4242);
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewConnection()
{
   QTcpSocket *clientSocket = _server.nextPendingConnection();
   connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
   connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));

    _sockets.push_back(clientSocket);
    for (QTcpSocket* socket : _sockets) {
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        _sockets.removeOne(sender);
    }
}

void MainWindow::onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray datas = sender->readAll();
    qDebug()<< datas ;




/*
 QFile fichierJSON("test.json");

        QJsonParseError jsonError;
        QJsonDocument flowerJson = QJsonDocument::fromJson(fichierJSON.readAll(),&jsonError);
        if (jsonError.error != QJsonParseError::NoError){
        qDebug() << jsonError.errorString();
        }
        /*QList<QVariant> list = flowerJson.toVariant().toList();
        QMap<QString, QVariant> map = list[0].toMap();
        qDebug() << map["temperature"].toString();*/



    for (QTcpSocket* socket : _sockets) {
        if (socket != sender)
            socket->write(QByteArray::fromStdString(sender->peerAddress().toString().toStdString() + ": " + datas.toStdString()));
    }

   QFile file("/home/elhem/serveur/Json.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       return;
              QTextStream out(&file);
                 out<<datas;
                  file.close();
   file.open(QIODevice::ReadOnly | QIODevice::Text);

   QJsonParseError jsonError;
       QJsonDocument document = QJsonDocument::fromJson(file.readAll(),&jsonError);
             if (jsonError.error != QJsonParseError::NoError){
             qDebug() << jsonError.errorString();
              }
        QList<QVariant> list = document.toVariant().toList();
        QMap<QString, QVariant> map = list[0].toMap();

        qDebug() << map["humidity"].toString();
        qDebug() << map["temperature"].toString();
        qDebug() << map["pressure"].toString();
        qDebug() << map["altitude"].toString();
        ui->lcdNumber1->display(map["temperature"].toString());
        ui->lcdNumber_5->display(map["pressure"].toString());
        ui->lcdNumber_3->display(map["humidity"].toString());




}
