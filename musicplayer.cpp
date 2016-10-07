#include "musicplayer.h"
#include <QStyle>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTime>
#include <QToolButton>
#include <QShortcut>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QHBoxLayout>

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent),playButton(0),
      volumeAdjust(0),positionSlider(0),positionLabel(0)
{
    createWidgets();
    createShortcuts();

    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePosition);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);

    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateState);
}

MusicPlayer::~MusicPlayer()
{

}

//开始构建主窗体的样式啦
void MusicPlayer::createWidgets()
{
    playButton = new QToolButton(this);
    playButton->setEnabled(false);
    playButton->setToolTip(tr("Play"));
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    //连接播放音乐的槽函数
    connect(playButton, &QAbstractButton::clicked, this, &MusicPlayer::togglePlayback);

    openButton = new QToolButton(this);
    openButton->setText(tr("..."));
    openButton->setToolTip(tr("Open A File"));
    openButton->setFixedSize(playButton->sizeHint());
    //要打开文件了
    connect(openButton, &QAbstractButton::clicked, this, &MusicPlayer::openFile);

    volumeAdjust = new QSlider(Qt::Horizontal, this);
    volumeAdjust->setToolTip("Adjust The Volume");
    volumeAdjust->setValue(mediaPlayer.volume());
    //是时候调节声音了
    connect(volumeAdjust, &QSlider::valueChanged, &mediaPlayer, &QMediaPlayer::setVolume);

    //接下来这个是进度条了
    positionSlider = new QSlider(Qt::Horizontal, this);
    positionSlider->setEnabled(false);
    positionSlider->setToolTip(tr("Seek"));
    connect(positionSlider, &QAbstractSlider::valueChanged, this, &MusicPlayer::setPosition);


    //显示音乐信息
    infoLabel = new QLabel(this);
    //通过时间显示音乐进度
    positionLabel = new QLabel(tr("00:00"), this);
    positionLabel->setMinimumWidth(positionLabel->sizeHint().width());


    //界面的布局了
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->setMargin(0);
    hlayout1->addWidget(openButton);
    hlayout1->addWidget(positionSlider);
    hlayout1->addWidget(playButton);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->setMargin(0);
    hlayout2->addItem(new QSpacerItem(volumeAdjust->height(),volumeAdjust->width()/10));
    hlayout2->addWidget(volumeAdjust);
    hlayout2->addWidget(positionLabel);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setMargin(0);
    vlayout->addWidget(infoLabel);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);

}

//设置快捷键
void MusicPlayer::createShortcuts()
{
    QShortcut *quitShortcut = new QShortcut(QKeySequence::Quit, this);
    //关闭程序
    connect(quitShortcut, &QShortcut::activated, QCoreApplication::quit);

    //打开文件
    QShortcut *openShortcut = new QShortcut(QKeySequence::Open, this);
    connect(openShortcut, &QShortcut::activated, this, &MusicPlayer::openFile);

    //开始暂停
    QShortcut *toggleShortcut = new QShortcut(Qt::Key_Space, this);
    connect(toggleShortcut, &QShortcut::activated, this, &MusicPlayer::togglePlayback);

    QShortcut *forwardShortcut = new QShortcut(Qt::Key_Right, this);
    connect(forwardShortcut, &QShortcut::activated, this, &MusicPlayer::seekForward);

    QShortcut *backwardShortcut = new QShortcut(Qt::Key_Left, this);
    connect(backwardShortcut, &QShortcut::activated, this, &MusicPlayer::seekBackward);

    QShortcut *increaseShortcut = new QShortcut(Qt::Key_Up, this);
    connect(increaseShortcut, &QShortcut::activated, this, &MusicPlayer::increaseVolume);

    QShortcut *decreaseShortcut = new QShortcut(Qt::Key_Down, this);
    connect(decreaseShortcut, &QShortcut::activated, this, &MusicPlayer::decreaseVolume);

}

//打开文件
void MusicPlayer::openFile()
{
    const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);

    const QString filePath =
            QFileDialog::getOpenFileName(this, tr("Open File"),
                                         musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                                         tr("MP3 files (*.mp3);;All files*.*"));
    if(!filePath.isEmpty())
        playFile(filePath);
}

//播放音乐
void MusicPlayer::playFile(const QString &filePath)
{
    //激活播放按钮
    playButton->setEnabled(true);
    //显示歌名
    infoLabel->setText(QFileInfo(filePath).fileName());
    //设置媒体
    mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
    //播放
    mediaPlayer.play();
}

//根据媒体状态确定播放还是暂停还是打开文件
void MusicPlayer::togglePlayback()
{
    if(mediaPlayer.mediaStatus() == QMediaPlayer::NoMedia)
        openFile();
    else if(mediaPlayer.state() == QMediaPlayer::PlayingState)
        mediaPlayer.pause();
    else
        mediaPlayer.play();
}

//快进
void MusicPlayer::seekForward()
{
    positionSlider->triggerAction(QSlider::SliderPageStepAdd);
}

//快退
void MusicPlayer::seekBackward()
{
    positionSlider->triggerAction(QSlider::SliderPageStepSub);
}
//增加音量
void MusicPlayer::increaseVolume()
{
    volumeAdjust->triggerAction(QSlider::SliderPageStepAdd);
}
//减小音量
void MusicPlayer::decreaseVolume()
{
    volumeAdjust->triggerAction(QSlider::SliderPageStepSub);
}

//负责更新按钮的图标的样式
void MusicPlayer::updateState(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        playButton->setToolTip(tr("Pause"));
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        playButton->setToolTip(tr("Play"));
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

//更新进度条的位置和标签显示的时间
void MusicPlayer::updatePosition(qint64 position)
{
    positionSlider->setValue(position);
    QTime duration(0, position/60000, qRound((position%60000)/1000.0));
    positionLabel->setText(duration.toString(tr("mm:ss")));
}

void MusicPlayer::updateDuration(qint64 duration)
{
    positionSlider->setRange(0,duration);
    positionSlider->setEnabled(duration>0);
    positionSlider->setPageStep(duration/10);
}


void MusicPlayer::setPosition(int position)
{
    if(qAbs(mediaPlayer.position() - position) > 99)
        mediaPlayer.setPosition(position);
}
