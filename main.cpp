#include "main.h"


int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    QApplication app(argc, argv);
    MainWindow mainWindow(argv[1]);
    mainWindow.show();
    return app.exec();
}
