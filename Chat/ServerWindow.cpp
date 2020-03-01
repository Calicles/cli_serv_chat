#include "ServerWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QTcpSocket>

ServerWindow::ServerWindow()
{
    // Fenetre
    etatServer = new QLabel;
    boutonQuitter = new QPushButton("Quitter");
    connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(etatServer);
    layout->addWidget(boutonQuitter);
    setLayout(layout);
    setWindowTitle("Chat - Server");


    // Init Server
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::LocalHost, 50885)) //Server erreur
    {
        etatServer->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br/>") + server->errorString());
    }else
    {
        etatServer->setText(tr("Le server a été demarré sur le port <strong>") +
                            QString::number(server->serverPort()) +
                            tr("</strong>. <br/>Des clients peuvent maintenant se connecter."));
        connect(server, SIGNAL(newConnection()), this, SLOT(nouvelleConnection()));
    }
    msgSize = 0;
}

ServerWindow::~ServerWindow()
{}

void ServerWindow::nouvelleConnection()
{
    envoyerATous(tr("<em>Un nouveau client vient d'arrivé</em>"));

    QTcpSocket *nouveauClient = server->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionCLient()));
}

void ServerWindow::donneesRecues()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    QDataStream in(socket);

    if (msgSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof (quint16))
        {
            return;
        }
        in >> msgSize;
    }
    if (socket->bytesAvailable() < msgSize)
        return;

    QString msg;
    in >> msg;

    envoyerATous(msg);
    msgSize = 0;
}

void ServerWindow::deconnexionCLient()
{
    envoyerATous(tr("<em>Un client vient de se déconnecté</em>"));

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    clients.removeOne(socket);

    socket->deleteLater();
}

void ServerWindow::envoyerATous(const QString &msg)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << msg;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof (quint16));

    for (int i(0); i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }
}
