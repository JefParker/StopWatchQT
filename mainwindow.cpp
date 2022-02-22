#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    readSettings();

    CountDownDlg->setParent(this);
    AboutDlg->setParent(this);

    bStart = true;
    nLap = 0;
    ui->label_readout->setStyleSheet("background-color:darkred;");
    ui->label_readout->show();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateReadout);
    connect(flashTimer, &QTimer::timeout, this, &MainWindow::flashReadout);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint*)),
            this, SLOT(on_MainWindow_customContextMenuRequested(QPoint*)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_Start_clicked() {
    if (bStart) { // Start
        lStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch() - lElapsedTime + CountDownDlg->lTempCountDownMS;
        CountDownDlg->lTempCountDownMS = 0;
        timer->start(50);
        ui->pushButton_Start->setText("Stop");
        ui->pushButton_Start->setToolTip("Stop");
        ui->pushButton_Lap->setText("Lap");
        ui->pushButton_Lap->setToolTip("Lap");
        ui->pushButton_Lap->setEnabled(true);
        flashTimer->stop();
        if (COUNTTYPE_DOWN == nCountType)
            ui->label_readout->setStyleSheet("background-color:DarkSlateGray;");
        else if (COUNTTYPE_UP == nCountType)
            ui->label_readout->setStyleSheet("background-color:darkgreen;");
        ui->label_readout->show();
        bStart = false;
        nLap = 0;
    }
    else {  // Stop
        lEndTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        lElapsedTime = lEndTime - lStartTime;
        timer->stop();
        ui->pushButton_Start->setText("Start");
        ui->pushButton_Start->setToolTip("Start");
        ui->pushButton_Lap->setText("Reset");
        ui->pushButton_Lap->setToolTip("Reset");
        ui->label_readout->setText(MS2HMST(lElapsedTime));
        if (COUNTTYPE_DOWN == nCountType)
            ui->label_readout->setStyleSheet("background-color:OrangeRed;");
        else if (COUNTTYPE_UP == nCountType)
            ui->label_readout->setStyleSheet("background-color:darkred;");
        ui->label_readout->show();
        bStart = true;
        nLap = 2;
    }
}

void MainWindow::on_pushButton_Lap_clicked() {

    if (0 == nLap) { // Lap
        timer->stop();
        ui->pushButton_Lap->setText("Lap off");
        ui->pushButton_Lap->setToolTip("Lap off");
        if (COUNTTYPE_DOWN == nCountType)
            ui->label_readout->setStyleSheet("background-color:DarkSlateBlue;");
        else if (COUNTTYPE_UP == nCountType)
            ui->label_readout->setStyleSheet("background-color:blue;");
        ui->label_readout->show();
        nLap = 1;
    }
    else if (1 == nLap) { // Lap off
        ui->pushButton_Lap->setText("Lap");
        ui->pushButton_Lap->setToolTip("Lap");
        if (COUNTTYPE_DOWN == nCountType)
            ui->label_readout->setStyleSheet("background-color:DarkSlateGray;");
        else if (COUNTTYPE_UP == nCountType)
            ui->label_readout->setStyleSheet("background-color:darkgreen;");
        ui->label_readout->show();
        nLap = 0;
        timer->start(50);
    }
    else if (2 == nLap) { // Reset
        nCountType = COUNTTYPE_UP;
        timer->stop();
        ui->pushButton_Lap->setText("Lap");
        ui->pushButton_Lap->setToolTip("Lap");
        ui->pushButton_Lap->setEnabled(false);
        lElapsedTime = 0;
        lEndTime = 0;
        lStartTime = 0;
        ui->label_readout->setText("00:00:00.0");
        ui->label_readout->setStyleSheet("background-color:darkred;");
        ui->label_readout->show();
        nLap = 1;
    }
}

QString MainWindow::MS2HMST(long lMS) {
    lMS = (lMS < 0) ? lMS * -1 : lMS;
    long milliseconds = (lMS % 1000) / 100, seconds = (lMS / 1000) % 60,
            minutes = (lMS / (1000 * 60)) % 60,
            hours = (lMS / (1000 * 60 * 60)) % 24;
    QString sHours = QString::number(hours).rightJustified(2, '0');
    QString sMinutes = QString::number(minutes).rightJustified(2, '0');
    QString sSeconds = QString::number(seconds).rightJustified(2, '0');
    QString sMilliseconds = QString::number(milliseconds);
    return sHours + ":" + sMinutes + ":" + sSeconds + "." + sMilliseconds;
}

void MainWindow::updateReadout() {
    long lElapsedTemp = QDateTime::currentDateTime().toMSecsSinceEpoch() - lStartTime;
    if (COUNTTYPE_DOWN == nCountType && lElapsedTemp >= 0) {
        timer->stop();
        nCountType = COUNTTYPE_UP;
        lElapsedTemp = 0;
        bStart = false;
        on_pushButton_Start_clicked();
        nLap = 2;
        on_pushButton_Lap_clicked();
        flashTimer->start(200);
        if (COUNTDOWNTO_SOUND == CountDownDlg->nCountDownToDo) {
            player->setAudioOutput(audioOutput);
            connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
            player->setSource(QUrl::fromLocalFile(CountDownDlg->soundFileName));
            player->setSource(QUrl::fromLocalFile(CountDownDlg->soundFileName)); // soundFileName
            audioOutput->setVolume(100);
            player->play();
        }
        else if (COUNTDOWNTO_LAUNCHPROGRAM == CountDownDlg->nCountDownToDo) {
            qint64 pid;
            QProcess::startDetached(CountDownDlg->sCountDownProgramCLI,
                                    {CountDownDlg->sCountDownProgramARG},
                                    CountDownDlg->sCountDownProgramWorkingDir, &pid);
        }
    }
    ui->label_readout->setText(MS2HMST(lElapsedTemp));
}

void MainWindow::flashReadout() {
    QString aColors[] = {"Red", "SeaGreen", "RebeccaPurple", "OrangeRed", "Teal", "MidnightBlue", "DarkRed",
                        "Red", "SeaGreen", "RebeccaPurple", "OrangeRed", "Teal", "MidnightBlue", "DarkRed"};
    ui->label_readout->setStyleSheet("background-color:"+aColors[nFlashColor++]+";");
    if (nFlashColor > 13) {
        flashTimer->stop();
        nFlashColor = 0;
    }
}

void MainWindow::on_AboutDlg() {
    AboutDlg->exec();
}

void MainWindow::on_actionLabel_triggered() {
    bool bOkay;
    QString sText = QInputDialog::getText(this, tr("StopWatch Label"),
        tr("StopWatch label:"), QLineEdit::Normal,
        this->windowTitle(), &bOkay);
    if (bOkay && !sText.isEmpty())
        this->setWindowTitle(sText);
}

void MainWindow::on_MainWindow_customContextMenuRequested(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Count down...", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(on_CountDownDlg()));
    contextMenu.addAction(&action1);

    QAction action2("Label...", this);
    connect(&action2, SIGNAL(triggered()), this, SLOT(on_actionLabel_triggered()));
    contextMenu.addAction(&action2);

    contextMenu.addAction(action3);

    QAction action4("About...", this);
    connect(&action4, SIGNAL(triggered()), this, SLOT(on_AboutDlg()));
    contextMenu.addAction(&action4);

    QAction action5("Exit", this);
    connect(&action5, SIGNAL(triggered()), this, SLOT(on_actionExit_triggered()));
    contextMenu.addAction(&action5);

    /*QAction action6("Count down...", this);
    connect(&action6, SIGNAL(triggered()), this, SLOT(on_CountDownDlg()));
    contextMenu.addAction(&action6);*/

    contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::on_CountDownDlg() {
    CountDownDlg->soundFileDir = CountDownDlg->soundFileDir;
    CountDownDlg->setControlData();
    if (CountDownDlg->exec() == QDialog::Accepted) {
        CountDownDlg->getControlData();
        settings->setValue("SoundFileName", CountDownDlg->soundFileName);
        if (0 < CountDownDlg->lCountDownMS) {
            bStart = false;
            on_pushButton_Start_clicked();
            nLap = 2;
            on_pushButton_Lap_clicked();
            ui->label_readout->setText(MS2HMST(CountDownDlg->lCountDownMS));
            nCountType = COUNTTYPE_DOWN;
            lElapsedTime = 0;
            lEndTime = 0;
            lStartTime = 0;
            ui->label_readout->setStyleSheet("background-color:OrangeRed;");
            CountDownDlg->lTempCountDownMS = CountDownDlg->lCountDownMS;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    Q_UNUSED(event);
}

void MainWindow::on_actionExit_triggered() {
    saveSettings();
    settings->sync();
    QApplication::exit(0);
}

void MainWindow::readSettings() {
    move(settings->value("pos", QPoint(200, 200)).toPoint());
    CountDownDlg->soundFileName = settings->value("SoundFileName").toString();
    if (!CountDownDlg->soundFileName.length())
        CountDownDlg->soundFileName = "No sound file selected";
    CountDownDlg->lCountDownMS = settings->value("CountDownMS").toInt();

    CountDownDlg->nCountDownToDo = settings->value("CountDownToDo").toInt();
    CountDownDlg->sCountDownProgramCLI = settings->value("CountDownProgramCLI").toString();
    CountDownDlg->sCountDownProgramARG = settings->value("CountDownProgramARG").toString();
    CountDownDlg->sCountDownProgramWorkingDir = settings->value("CountDownProgramWorkingDir").toString();

    connect(action3, SIGNAL(triggered()), this, SLOT(setAlwaysOnTop()));
    action3->setCheckable(true);
    bool bAlwaysTopMost = settings->value("AlwaysOnTop").toBool();
    action3->setChecked(bAlwaysTopMost);
    if (bAlwaysTopMost)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void MainWindow::saveSettings() {
    settings->setValue("pos", pos());
    settings->setValue("CountDownMS", (int)CountDownDlg->lCountDownMS);
    settings->setValue("AlwaysOnTop", action3->isChecked());
    settings->setValue("CountDownToDo", CountDownDlg->nCountDownToDo);
    settings->setValue("CountDownProgramCLI", CountDownDlg->sCountDownProgramCLI);
    settings->setValue("CountDownProgramARG", CountDownDlg->sCountDownProgramARG);
    settings->setValue("CountDownProgramWorkingDir", CountDownDlg->sCountDownProgramWorkingDir);
}

void MainWindow::setAlwaysOnTop()
{
    /*Qt::WindowFlags flags = windowFlags();
    if (action3->isChecked())
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags &= ~Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    show();*/
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Always on Top"));
    msgBox.setText(tr("Requires restart."));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

}

