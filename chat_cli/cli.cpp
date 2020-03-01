#include "cli.h"
#include "ui_cli.h"
#include <QWidget>

Cli::Cli(QWidget *parent) :
    QWidget(parent), msgSize(0)
{
    setupUi(this);

    server = new QTcpSocket(this);

    QObject::connect(server, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    QObject::connect(server, SIGNAL(connected()), this, SLOT(connect()));
    QObject::connect(server, SIGNAL(disconnected()), this, SLOT(deconnect()));
    QObject::connect(server, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    setWindowTitle(tr("Chat - CLI"));

}

Cli::~Cli()
{
}

void Cli::on_connexionBouton_clicked()
{
    chatBrowser->append(tr("<em>Tentative de connexion en cours...</em>"));
    connexionBouton->setEnabled(false);

    server->abort(); //Desactiv connexion existantes
    server->connectToHost(ipText->text(), portSpinBox->value());
}

void Cli::on_envoyerBouton_clicked()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    QString msgToSend(tr("<strong>") + pseudoText->text() + tr("</strong> : ") + messageText->text());

    out << (quint16) 0;
    out << msgToSend;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof (quint16));

    server->write(paquet);

    messageText->clear();
    messageText->setFocus(); //Reprend le curseur
}

void Cli::on_messageText_returnPressed()
{
    on_envoyerBouton_clicked();
}

void Cli::donneesRecues()
{
    QDataStream in(server);

    if (msgSize == 0)
    {
        if (server->bytesAvailable() < (int)sizeof (quint16))
        {
            return;
        }
        in >> msgSize;
    }

    if (server->bytesAvailable() < msgSize)
        return;

    QString msgRecu;
    in >> msgRecu;

    chatBrowser->append(msgRecu);

    msgSize = 0;
}

void Cli::connect()
{
    chatBrowser->append(tr("<em>Connexion réussi !</em>"));
    connexionBouton->setEnabled(true);
}

void Cli::deconnect()
{
    chatBrowser->append(tr("<em>Déconnecté du server</em>"));
}

void Cli::erreurSocket(QAbstractSocket::SocketError erreur)
{
    QString msgError(tr("<em>Erreur : "));
    switch (erreur)
    {
        case QAbstractSocket::HostNotFoundError:
            msgError.append(tr("le server n'a pas pu être trouvé. Vérifiez l'IP et le port </em>"));
        break;
        case QAbstractSocket::ConnectionRefusedError:
            msgError.append(tr("le serveur a refusé la connexion. Verifiez si le programme \"server\" a bien été lancé. Ainsi que l'IP et le port</em>"));
        break;
        case QAbstractSocket::RemoteHostClosedError:
            msgError.append(tr("le server a coupé la connexion.</em>"));
        break;

    default:
        msgError.append(server->errorString() + tr("</em>"));
    }
    chatBrowser->append(msgError);

    connexionBouton->setEnabled(true);
}
