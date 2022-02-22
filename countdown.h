#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QDialog>
// #include <QSound>
#include <QMediaPlayer>
#include <QFileDialog>

namespace Ui {
class CountDown;
}

class CountDown : public QDialog
{
    Q_OBJECT

public:
    explicit CountDown(QWidget *parent = nullptr);
    ~CountDown();
    void setControlData();
    void getControlData();
    QString soundFileName = "No sound file selected";
    QString soundFileDir = "/usr/share/sounds/";
    int nCountDownToDo = 0;
    long lCountDownMS = 0, lTempCountDownMS = 0;
    QString sCountDownProgramCLI = "";
    QString sCountDownProgramARG = "";
    QString sCountDownProgramWorkingDir = "";

private slots:

    void on_CountDownSound_clicked(bool checked);
    void on_CountDownNothing_clicked(bool checked);
    void on_CountDownProgram_clicked(bool checked);

private:
    Ui::CountDown *ui;

    void btnBrowseForSound();


};

#endif // COUNTDOWN_H
