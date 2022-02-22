#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About) {
    ui->setupUi(this);

    ui->ImgAboutLabel->setGeometry(72, 20, 64, 64);
    QPixmap *StpWtchImg = new QPixmap(":images/Stopwatch128.png");
    ui->ImgAboutLabel->setPixmap(StpWtchImg->scaled(64, 64, Qt::KeepAspectRatio));
    ui->ImgAboutLabel->show();
}

About::~About() {
    delete ui;
}

void About::on_pushButton_clicked() {
    close();
}

