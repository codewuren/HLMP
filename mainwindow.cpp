#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#define DefaultVolume 25

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 创建一个播放器对象
    player = new QMediaPlayer(this);

    // 创建一个音频输出对象
    audioOutput = new QAudioOutput(this);

    // 设置音频输出对象（用来设置音量，QT6 中删除了 QMediaPlayer 中的 setVolume()）
    player->setAudioOutput(audioOutput);

    // 默认百分之25音量
    ui->volumeSlider->setSliderPosition(DefaultVolume);
    MainWindow::ChangeVolume(DefaultVolume);

    // 绑定
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::PlayButton_Clicked);
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::NextMusic);
    connect(ui->lastButton, &QPushButton::clicked, this, &MainWindow::LastMusic);
    connect(ui->choosedirButton, &QPushButton::clicked, this, &MainWindow::OpenFileDirectory);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::ChangeVolume);
    connect(ui->progressSlider, &QSlider::sliderMoved, this, &MainWindow::changeProgress);
    connect(ui->playlistWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::musicList_Clicked);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgress);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::PlayButton_Clicked() {
    switch(player->playbackState()) {
    case QMediaPlayer::PlaybackState::PlayingState:
        ui->playButton->setText("播放");
        PauseMusic();
        break;
    case QMediaPlayer::PlaybackState::PausedState:
        ui->playButton->setText("暂停");
        PlayMusic();
        break;
    case QMediaPlayer::PlaybackState::StoppedState:
        if(dirCheck()) {
            player->setSource(QUrl::fromLocalFile(dirName + "/" + ui->playlistWidget->currentItem()->text()));
            PlayMusic();
        }
        break;
    default:
        break;
    };
}

void MainWindow::PlayMusic() {
    currentIndex = ui->playlistWidget->currentRow();
    player->play();
}

void MainWindow::PauseMusic() {
    player->pause();
}

void MainWindow::StopMusic() {
    player->stop();
}

void MainWindow::LastMusic() {
    if (dirCheck()) {
        StopMusic();
        currentIndex--;
        ui->playlistWidget->setCurrentRow(currentIndex);
        player->setSource(QUrl::fromLocalFile(dirName + "/" + ui->playlistWidget->currentItem()->text()));
        PlayMusic();
    }
}

void MainWindow::NextMusic() {
    if (dirCheck()) {
        StopMusic();
        currentIndex++;
        ui->playlistWidget->setCurrentRow(currentIndex);
        player->setSource(QUrl::fromLocalFile(dirName + "/" + ui->playlistWidget->currentItem()->text()));
        PlayMusic();
    }
}

void MainWindow::ChangeVolume(int vol) {
    audioOutput->setVolume(float(vol) / 100);
}

void MainWindow::OpenFileDirectory() {
    dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirName.isEmpty()) {
        QDir *dir = new QDir(dirName);
        dir->setFilter(QDir::Files);
        dir->setSorting(QDir::Name);
        dir->setNameFilters(QString("*.mp3").split(":"));
        QList musicNames = dir->entryList();
        for (int i = 0; i < musicNames.size(); i++) {
            ui->playlistWidget->addItem(musicNames[i]);
        }
    }
    else {
        dirName = "";
    }
}

void MainWindow::OpenFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Music File"), "", tr("Music Files (*.mp3)"));
    // TODO ...
}

void MainWindow::changeProgress(int value) {
    player->setPosition(value);
}

void MainWindow::updateProgress(qint64 position) {
    ui->progressSlider->setRange(0, player->duration());
    ui->progressSlider->setValue(position);
}

void MainWindow::musicList_Clicked() {
    ui->playButton->setText("暂停");
    player->setSource(QUrl::fromLocalFile(dirName + "/" + ui->playlistWidget->currentItem()->text()));
    PlayMusic();
}

bool MainWindow::dirCheck() {
    if (dirName.isEmpty()) {
        QMessageBox MBox;
        MBox.setWindowTitle("Error!");
        MBox.setText("请选择一个文件夹！");
        MBox.show();
        MBox.exec();
        return false;
    }
    else return true;
}
