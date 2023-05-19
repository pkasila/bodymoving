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

    connect(ui->checkBox, &QCheckBox::stateChanged, this, &MainWindow::handlePlotCount);
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

void MainWindow::handlePlotCount() {
    ui->checkBox_2->setEnabled(ui->checkBox->isChecked());
    ui->checkBox_2->setChecked(!ui->checkBox->isChecked() ? false : ui->checkBox_2->isChecked());
    draw();
}

void MainWindow::draw() {
    ui->graphicsView->chart()->removeSeries(s1);
    s1->clear();
    ui->graphicsView->chart()->removeSeries(s2);
    s2->clear();

    if (ui->checkBox->isChecked()) {
            if (ui->checkBox_2->isChecked()) {
                draw1();
            }
            draw2();
    } else {
            draw1();
    }

    ui->graphicsView->chart()->addSeries(s1);
    ui->graphicsView->chart()->addSeries(s2);
    ui->graphicsView->chart()->createDefaultAxes();
}

void MainWindow::draw1() {
    double velocity = ui->lineEdit->text().replace(',', '.').toDouble();
    double phi = ui->lineEdit_2->text().replace(',', '.').toDouble();
    double h = ui->lineEdit_3->text().replace(',', '.').toDouble();
    double r = ui->lineEdit_4->text().replace(',', '.').toDouble();
    double y = ui->lineEdit_5->text().replace(',', '.').toDouble();
    int n = ui->lineEdit_6->text().toInt();

    double t = 0;
    double velocityX = velocity * cos(phi * acos(-1) / 180);
    double velocityY = velocity * sin(phi * acos(-1) / 180);
    double K = (4.5 * Dv / r + 0.15 * p0 * sqrt(velocityX * velocityX + velocityY * velocityY)) / (p * r);
    double x = 0;
    double y0 = y;
    double X = -INT_MAX, tx = 0;
    double Y = -INT_MAX, ty = 0;

    s1->append(x, y);

    for (int i = 0; i < n; i++) {
        t += h;
        double velocityX2 = y < 0 ? 0 : (1 - K * h) * velocityX;
        double velocityY2 = (p0 / p - 1) * 9.81 * h + (1 - K * h) * velocityY;
        K = (4.5 * Dv / r + 0.15 * p0 * sqrt(velocityX * velocityX + velocityY * velocityY)) / (p * r);
        x += velocityX2 == 0 ? 0 : velocityX * h;
        velocityX = velocityX2;
        double tempY = y;
        y += velocityX2 == 0 ? 0 : velocityY * h;
        if (y > Y) {
            Y = y;
            ty = t;
        }
        if (X < 0 && (y < 0 && tempY > 0) || (y > 0 && tempY < 0)) {
            X = x;
            tx = t;
        }
        velocityY = velocityY2;
        s1->append(x, (y > y0 || ui->radioButton_2->isChecked()) ? y : y0);
    }

    ui->lineEdit_7->setText(QString::number(X));
    ui->lineEdit_8->setText(QString::number(tx));
    ui->lineEdit_9->setText(QString::number(Y));
    ui->lineEdit_10->setText(QString::number(ty));

    Xmax = X;
}

void MainWindow::draw2() {
    double tp0 = p0;
    p0 = 0;
    double tDv = Dv;
    Dv = 0;

    double velocity = ui->lineEdit->text().replace(',', '.').toDouble();
    double phi = ui->lineEdit_2->text().replace(',', '.').toDouble();
    double h = ui->lineEdit_3->text().replace(',', '.').toDouble();
    double r = ui->lineEdit_4->text().replace(',', '.').toDouble();
    double y = ui->lineEdit_5->text().replace(',', '.').toDouble();
    int n = ui->lineEdit_6->text().toInt();

    double t = 0;
    double velocityX = velocity * cos(phi * acos(-1) / 180);
    double velocityY = velocity * sin(phi * acos(-1) / 180);
    double K = (4.5 * Dv / r + 0.15 * p0 * sqrt(velocityX * velocityX + velocityY * velocityY)) / (p * r);
    double x = 0;
    double X = -INT_MAX;

    s2->append(x, y);

    for (int i = 0; i < n; i++) {
        t += h;
        double velocityX2 = y < 0 ? 0 : (1 - K * h) * velocityX;
        double velocityY2 = (p0 / p - 1) * 9.81 * h + (1 - K * h) * velocityY;
        K = (4.5 * Dv / r + 0.15 * p0 * sqrt(velocityX * velocityX + velocityY * velocityY)) / (p * r);
        x += velocityX2 == 0 ? 0 : velocityX * h;
        velocityX = velocityX2;
        double tempY = y;
        y += velocityX2 == 0 ? 0 : velocityY * h;

        if (X < 0 && (y < 0 && tempY > 0) || (y > 0 && tempY < 0)) {
            X = x;
        }
        velocityY = velocityY2;

        s2->append(x, y);
    }

    p0 = tp0;
    Dv = tDv;

    ui->lineEdit_11->setText(QString::number(abs(X - Xmax)));
}
