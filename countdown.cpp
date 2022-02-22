#include "countdown.h"
#include "ui_countdown.h"
#include "mainwindow.h"

CountDown::CountDown(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountDown) {
    ui->setupUi(this);
    QPixmap *StpWtchImg = new QPixmap(":images/MusicFolder64.png");
    ui->pbCDSound->setIcon(StpWtchImg->scaled(34, 34, Qt::KeepAspectRatio));
    ui->SoundFileLbl->setAlignment(Qt::AlignLeft);

    connect(ui->pbCDSound, &QPushButton::released, this, &CountDown::btnBrowseForSound);

    //connect(ui->dbbCountDown, &QDialogButtonBox::accepted, this, &MainWindow::on_CDOkay_triggered);
    //connect(ui->dbbCountDown, &QDialogButtonBox::rejected, this, &MainWindow::on_actionCDCancel_triggered);

}

CountDown::~CountDown() {
    delete ui;
}

void CountDown::setControlData() {
    int nLastIndex = soundFileName.lastIndexOf("/");
    QString sTemp = soundFileName;
    ui->SoundFileLbl->setText(sTemp.remove(0, nLastIndex+1));
    if (COUNTDOWNTO_NOTHING == nCountDownToDo)
        ui->CountDownNothing->setChecked(true);
    else if (COUNTDOWNTO_SOUND == nCountDownToDo)
        ui->CountDownSound->setChecked(true);
    else if (COUNTDOWNTO_LAUNCHPROGRAM == nCountDownToDo)
        ui->CountDownProgram->setChecked(true);

    int nHours = (lCountDownMS / (1000 * 60 * 60)) % 24;
    ui->sbHours->setValue(nHours);

    int nMinutes = (lCountDownMS / (1000 * 60)) % 60;
    ui->sbMinutes->setValue(nMinutes);

    int nSeconds = (lCountDownMS / 1000) % 60;
    ui->sbSeconds->setValue(nSeconds);

    ui->CountDownProgramCLI->setPlainText(sCountDownProgramCLI);
    ui->CountDownProgramARG->setPlainText(sCountDownProgramARG);
    ui->CountDownProgramWorkingDir->setPlainText(sCountDownProgramWorkingDir);
}

void CountDown::getControlData() {
    int nHours = ui->sbHours->value();
    int nMinutes = ui->sbMinutes->value();
    int nSeconds = ui->sbSeconds->value();
    lCountDownMS = (nSeconds*1000) + (nMinutes*60000) + (nHours*3600000);
    sCountDownProgramCLI = ui->CountDownProgramCLI->toPlainText().trimmed();
    sCountDownProgramARG = ui->CountDownProgramARG->toPlainText().trimmed();
    sCountDownProgramWorkingDir = ui->CountDownProgramWorkingDir->toPlainText().trimmed();
}

void CountDown::btnBrowseForSound() {
    QString sTempDir;
    if ("No sound file selected" != soundFileName) {
        int nDirIndex = soundFileName.lastIndexOf("/") + 1;
        sTempDir = soundFileName;
        sTempDir.remove(nDirIndex, 0);
        soundFileDir = sTempDir;
    }
    else
        sTempDir = soundFileDir;

    QString sTempSoundFileName = QFileDialog::getOpenFileName(this,
        tr("Select Audio File"), sTempDir,
        tr("Audio Files (*.aac *.flac *.mp3 *m4a *mp4 *.ogg *.opus *.wav *.wma)"));

    soundFileName = sTempSoundFileName.length() ? sTempSoundFileName : soundFileName;
    int nLastIndex = soundFileName.lastIndexOf("/");
    QString sFileTemp = soundFileName;
    ui->SoundFileLbl->setText(sFileTemp.remove(0, nLastIndex+1));

    //settings->setValue("SoundFileName", soundFileName);
}

void CountDown::on_CountDownNothing_clicked(bool checked) {
    if (checked)
        nCountDownToDo = COUNTDOWNTO_NOTHING;
}

void CountDown::on_CountDownSound_clicked(bool checked) {
    if (checked)
        nCountDownToDo = COUNTDOWNTO_SOUND;
}

void CountDown::on_CountDownProgram_clicked(bool checked) {
    if (checked)
        nCountDownToDo = COUNTDOWNTO_LAUNCHPROGRAM;
}

