#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStandardItem>
#include <QUrl>
#include <QMessageBox>
#include <Qstring>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void PlayButton_Clicked();
    void PlayMusic();
    void PauseMusic();
    void StopMusic();
    void LastMusic();
    void NextMusic();
    void ChangeVolume(int vol);
    void OpenFileDirectory();
    void OpenFile();
    void changeProgress(int value);
    void updateProgress(qint64 position);
    void musicList_Clicked();
    bool dirCheck();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    int currentIndex;
    QString dirName = "";
};
#endif // MAINWINDOW_H
