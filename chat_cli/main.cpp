#include "cli.h"
#include <QApplication>

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    Cli cli;

    cli.show();

    return app.exec();

}
