#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
/*************
 * 快捷键重复的问题要通过事件过滤来修正
 *
 *
 ***************/
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSlider>
#include <QLabel>
#include <QToolBar>
#include <QAbstractButton>
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>


class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

public slots:
    //打开文件
    void openFile();
    //播放文件
    void playFile(const QString& filePath);
    //切换播放还是暂停
    void togglePlayback();
    //前进
    void seekForward();
    //后退
    void seekBackward();
    void increaseVolume();
    void decreaseVolume();

private slots:
    void updateState(QMediaPlayer::State state);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setPosition(int position);

private:
    void createWidgets();
    void createShortcuts();

    QMediaPlayer mediaPlayer;
    QAbstractButton *playButton;
    QAbstractButton *openButton;
    QSlider *volumeAdjust;
    QSlider *positionSlider;
    QLabel *positionLabel;
    QLabel *infoLabel;
};

#endif // MUSICPLAYER_H
