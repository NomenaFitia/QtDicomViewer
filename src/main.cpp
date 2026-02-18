#include "platform/mainwindow.h"
#include <tinyobjloader/tiny_obj_loader.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
