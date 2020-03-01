#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QTcpServer>
#include <QLabel>
#include <QPushButton>
#include <QList>

class ServerWindow : public QWidget
{
    Q_OBJECT
public:
    ServerWindow();
    virtual ~ServerWindow();
    void envoyerATous(const QString &msg);

private slots:
    void nouvelleConnection();
    void donneesRecues();
    void deconnexionCLient();


private:
    QLabel *etatServer;
    QPushButton *boutonQuitter;

    QTcpServer *server;
    QList<QTcpSocket *> clients;
    quint16 msgSize;
};

#endif // SERVERWINDOW_H
