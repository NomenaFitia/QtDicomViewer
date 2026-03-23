/**
 *
 * @brief Classe principale de la fenêtre TODO
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qopenglwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QOpenGLWidget* glWidget;

    void createMenus();
    void createToolBar();
    void createDock();
};
#endif // MAINWINDOW_H
