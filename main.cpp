#include "main.h"


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        qDebug("аргументы: <inifile in> <inifile out>");
        return 1;
    }
    QApplication app(argc, argv);
    MainWindow mainWindow(argv[1], argv[2]);
    mainWindow.show();
    return app.exec();
}
