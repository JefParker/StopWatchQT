#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QInputDialog>
#include <QMenu>
#include <QSpinBox>
#include <QDialogButtonBox>
//#include <QSound>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QSettings>
#include <QEvent>
#include "about.h"
#include "countdown.h"
#include <QProcess>

#define COUNTDOWNTO_NOTHING 0
#define COUNTDOWNTO_SOUND 1
#define COUNTDOWNTO_LAUNCHPROGRAM 2

#define COUNTTYPE_UP 0
#define COUNTTYPE_DOWN 1
#define COUNTTYPE_TOTIME 2

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_actionLabel_triggered();
    void on_actionExit_triggered();


private slots:
    void on_pushButton_Start_clicked();

    void on_pushButton_Lap_clicked();

    void on_AboutDlg();
    void on_CountDownDlg();

    void saveSettings();
    void readSettings();

    void setAlwaysOnTop();

    void flashReadout();
    void updateReadout();

    void on_MainWindow_customContextMenuRequested(const QPoint &pos);

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    bool bStart;
    int nLap;
    QTimer *timer = new QTimer(this);
    QTimer *flashTimer = new QTimer(this);
    int nFlashColor = 0;
    long lStartTime = 0, lEndTime = 0, lElapsedTime = 0;
    QString MS2HMST(long lMS);
    About *AboutDlg = new About(this);
    CountDown *CountDownDlg = new CountDown(this);

    int nCountType = COUNTTYPE_UP;
    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    QLabel *SoundFileLbl = new QLabel();
    QSettings *settings = new QSettings("JeffWare", "StopWatch");
    QAction *action3 = new QAction("Always on Top", this);
};
#endif // MAINWINDOW_H
