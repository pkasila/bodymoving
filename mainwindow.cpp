#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto validator = new QDoubleValidator(0, 10000000, 10, this);

    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEdit->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);
    ui->lineEdit_3->setValidator(validator);
    ui->lineEdit_4->setValidator(validator);

    s1->setName("сопротив.");
    s2->setName("нет сопротив.");

    ui->graphicsView->chart()->addSeries(s1);
    ui->graphicsView->chart()->addSeries(s2);

    ui->graphicsView->chart()->createDefaultAxes();

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::handleMaterial);

    connect(ui->radioButton, &QRadioButton::clicked, this, &MainWindow::handleEnvironment);
    connect(ui->radioButton_2, &QRadioButton::clicked, this, &MainWindow::handleEnvironment);

    connect(ui->checkBox, &QCheckBox::stateChanged, this, &MainWindow::draw);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &MainWindow::draw);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::draw);

    draw();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleMaterial() {
    switch (ui->comboBox->currentIndex()) {
        case 0:
            this->p = 2600;
            break;
        case 1:
            this->p = 25;
            break;
        case 2:
            this->p = 450;
            break;
        case 3:
            this->p = 7870;
            break;
    }

    ui->label_2->setText(QString::number(this->p));
    draw();
}

void MainWindow::handleEnvironment() {
    this->p0 = ui->radioButton->isChecked() ? 1.205 : 997;
    draw();
}

void MainWindow::draw() {
    ui->graphicsView->chart()->removeSeries(s1);
    ui->graphicsView->chart()->removeSeries(s2);

    if (ui->checkBox->isChecked()) {
        draw1();
        draw2();
    } else {
        draw1();
        s2->clear();
    }

    ui->graphicsView->chart()->addSeries(s1);
    ui->graphicsView->chart()->addSeries(s2);
    ui->graphicsView->chart()->createDefaultAxes();
}

void MainWindow::draw1() {
    s1->clear();

    double V0 = ui->lineEdit->text().replace(',', '.').toDouble();
    double U = ui->lineEdit_2->text().replace(',', '.').toDouble();
    double h = ui->lineEdit_3->text().replace(',', '.').toDouble();
    double r = ui->lineEdit_4->text().replace(',', '.').toDouble();
    double y = ui->lineEdit_5->text().replace(',', '.').toDouble();
    int n = ui->lineEdit_6->text().toInt();

    double T = 0;
    double Vx = V0 * cos(U * acos(-1) / 180);
    double Vy = V0 * sin(U * acos(-1) / 180);
    double K = (4.5 * Dv / r + 0.15 * p0 * sqrt(Vx * Vx + Vy * Vy)) / (p * r);
    double x = 0;
    double y0 = y;
    double X = -10, Tp = 0, Y = -100, Ty = 0;

    s1->append(x, y);

    for (int i = 0; i < n; i++) {
        T += h;
        double Vx2 = y < 0 ? 0 : (1 - K * h) * Vx;
        double Vy2 = (p0 / p - 1) * 9.81 * h + (1 - K * h) * Vy;
        K = (4.5 * Dv / r + 0.15 * p0 * sqrt(Vx * Vx + Vy * Vy)) / (p * r);
        x += Vx2 == 0 ? 0 : Vx * h;
        Vx = Vx2;
        double ty = y;
        y += Vx2 == 0 ? 0 : Vy * h;
        if (y > Y) {
            Y = y;
            Ty = T;
        }
        if (X == -10 && (y < 0 && ty > 0) || (y > 0 && ty < 0)) {
            X = x;
            Tp = T;
        }
        Vy = Vy2;
        s1->append(x, (y > y0 || ui->radioButton_2->isChecked()) ? y : y0);
    }

    ui->lineEdit_7->setText(QString::number(X));
    ui->lineEdit_8->setText(QString::number(Tp));
    ui->lineEdit_9->setText(QString::number(Y));
    ui->lineEdit_10->setText(QString::number(Ty));

    Xmax = X;
}

void MainWindow::draw2() {
    double tp0 = p0;
    p0 = 0;
    double tDv = Dv;
    Dv = 0;
    s2->clear();
    double V0 = ui->lineEdit->text().replace(',', '.').toDouble();
    double U = ui->lineEdit_2->text().replace(',', '.').toDouble();
    double h = ui->lineEdit_3->text().replace(',', '.').toDouble();
    double r = ui->lineEdit_4->text().replace(',', '.').toDouble();
    int n = ui->lineEdit_6->text().toInt();
    double y = ui->lineEdit_5->text().replace(',', '.').toDouble();

    double T = 0;
    double Vx = V0 * cos(U * acos(-1) / 180);
    double Vy = V0 * sin(U * acos(-1) / 180);
    double K = (4.5 * Dv / r + 0.15 * p0 * sqrt(Vx * Vx + Vy * Vy)) / (p * r);
    double x = 0;
    double X = -10;

    s2->append(x, y);

    for (int i = 0; i < n; i++) {
        T += h;
        double Vx2 = y < 0 ? 0 : (1 - K * h) * Vx;
        double Vy2 = (p0 / p - 1) * 9.81 * h + (1 - K * h) * Vy;
        K = (4.5 * Dv / r + 0.15 * p0 * sqrt(Vx * Vx + Vy * Vy)) / (p * r);
        x += Vx2 == 0 ? 0 : Vx * h;
        Vx = Vx2;
        double ty = y;
        y += Vx2 == 0 ? 0 : Vy * h;

        if (X == -10 && (y < 0 && ty > 0) || (y > 0 && ty < 0)) {
            X = x;
        }
        Vy = Vy2;

        s2->append(x, y);
    }

    p0 = tp0;
    Dv = tDv;

    ui->lineEdit_11->setText(QString::number(abs(X - Xmax)));
}
