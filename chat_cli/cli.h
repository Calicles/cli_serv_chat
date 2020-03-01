#ifndef CLI_H
#define CLI_H

#include <QWidget>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "ui_cli.h"

namespace Ui {
class Cli;
}

class Cli : public QWidget, private Ui::Cli
{
    Q_OBJECT

public:
    explicit Cli(QWidget *parent = nullptr);
    ~Cli();

private slots:
    void on_envoyerBouton_clicked();
    void on_connexionBouton_clicked();
    void on_messageText_returnPressed();
    void donneesRecues();
    void connect();
    void deconnect();
    void erreurSocket(QAbstractSocket::SocketError error);

private:
    QTcpSocket *server;
    quint16 msgSize;
};

#endif // CLI_H
