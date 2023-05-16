#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double p = 2600;
    double p0 = 1.205;
    double Dv = 0.0000181;
    double Xmax = 0;
    QLineSeries *s1 = new QLineSeries();
    QLineSeries *s2 = new QLineSeries();

public slots:
    void handleMaterial();
    void handleEnvironment();

    void draw();
    void draw1();
    void draw2();
};
#endif // MAINWINDOW_H
