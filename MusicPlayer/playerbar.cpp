#include "playerbar.h"
#include "ui_playerbar.h"

PlayerBar::PlayerBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerBar)
{
    ui->setupUi(this);

    initUI();
    initMedia();
    initConnect(parent);
    initConfig();
}

void PlayerBar::initUI()
{
    m_pBackgroundFrame = new QFrame(this);
    m_pBackgroundFrame->setObjectName("backgroundFrame");

    m_pTimeWidget = new QWidget(this);

    m_pTitleLable = new QLabel(this);
    m_pIconLable = new QLabel(this);
    m_pProgressLable = new QLabel(this);
    m_pDivisionLable = new QLabel(this);
    m_pDurationLable = new QLabel(this);

    m_pPlayButton = new PlayButton("playButton", "播放", this);
    m_pPauseButton = new PlayButton("pauseButton", "暂停", this, false);
    m_pStopButton = new PlayButton("stopButton", "停止", this);
    m_pPreviousButton = new PlayButton("previousButton", "前一首", this);
    m_pNextButton = new PlayButton("nextButton", "后一首", this);

    m_pCurrentItemOnceButton = new PlayButton("currentItemOnceButton", "播放模式：单曲播放", this);
    m_pCurrentItemInLoopButton = new PlayButton("currentItemInLoopButton", "播放模式：单曲循环", this);
    m_pSequentialButton = new PlayButton("sequentialButton", "播放模式：顺序播放", this);
    m_pLoopButton = new PlayButton("loopButton", "播放模式：循环播放", this);
    m_pRandomButton = new PlayButton("randomButton", "播放模式：随机播放", this);
    m_ButtonList<<m_pCurrentItemOnceButton<<m_pCurrentItemInLoopButton<<m_pSequentialButton<<m_pLoopButton<<m_pRandomButton;

    m_pVolumeLoudButton = new PlayButton("volumeLoudButton", "音量高", this);
    m_pVolumeQuietButton = new PlayButton("volumeQuietButton", "音量低", this);
    m_pVolumeOffButton = new PlayButton("volumeOffButton", "静音", this);

    m_pPlayListButton = new PlayButton("playlistButton", "播放列表", this);

    m_pPlaySlider = new QSlider(Qt::Horizontal, this);
    m_pVolumeSlider = new QSlider(Qt::Vertical, this);

    m_pProgressLable->setText("00:00:00");
    m_pDivisionLable->setText("/");
    m_pDurationLable->setText("00:00:00");

    QHBoxLayout *pLayout = new QHBoxLayout;
    m_pTimeWidget->setLayout(pLayout);
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(m_pProgressLable);
    pLayout->addWidget(m_pDivisionLable);
    pLayout->addWidget(m_pDurationLable);

    loadQss(":/qss/qss/playerbar.qss", this);
}

void PlayerBar::initMedia()
{
    m_pPlayer = new QMediaPlayer;
    m_pPlayerList = new QMediaPlaylist;
}

void PlayerBar::initConnect(QWidget *parent)
{
    connect(m_pPlayButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
    connect(m_pPauseButton, SIGNAL(clicked()), this, SLOT(pauseButton_clicked()));
    connect(m_pStopButton, SIGNAL(clicked()), this, SLOT(stopButton_clicked()));
    connect(m_pPreviousButton, SIGNAL(clicked()), this, SLOT(previousButton_clicked()));
    connect(m_pNextButton, SIGNAL(clicked()), this, SLOT(nextButton_clicked()));

    connect(m_pCurrentItemOnceButton, SIGNAL(clicked()), this, SLOT(currentItemOnceButton_clicked()));
    connect(m_pCurrentItemInLoopButton, SIGNAL(clicked()), this, SLOT(currentItemInLoopButton_clicked()));
    connect(m_pSequentialButton, SIGNAL(clicked()), this, SLOT(sequentialButton_clicked()));
    connect(m_pLoopButton, SIGNAL(clicked()), this, SLOT(loopButton_clicked()));
    connect(m_pRandomButton, SIGNAL(clicked()), this, SLOT(randomButton_clicked()));

    connect(m_pVolumeLoudButton, SIGNAL(clicked()), this, SLOT(volumeOnButton_clicked()));
    connect(m_pVolumeQuietButton, SIGNAL(clicked()), this, SLOT(volumeOnButton_clicked()));
    connect(m_pVolumeOffButton, SIGNAL(clicked()), this, SLOT(volumeOffButton_clicked()));

    connect(m_pPlayListButton, SIGNAL(clicked()), this, SLOT(playlistButton_clicked()));

    connect(m_pPlaySlider, SIGNAL(valueChanged(int)), this, SLOT(playSlider_valueChanged(int)));
    connect(m_pVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeSlider_valueChanged(int)));

    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(m_pPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(getduration(qint64)));
    connect(m_pPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playButtonChange(QMediaPlayer::State)));
    connect(m_pPlayer, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(nowPlaying(QMediaContent)));
    connect(m_pPlayer, SIGNAL(volumeChanged(int)), this, SLOT(volumeButtonChange(int)));

    connect(m_pPlayerList, SIGNAL(mediaInserted(int,int)), this, SLOT(showMediaList(int,int)));
    connect(m_pPlayerList, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)), this, SLOT(modeButtonChange(QMediaPlaylist::PlaybackMode)));

    connect(parent, SIGNAL(send_filePath(QList<QMediaContent>)), this, SLOT(recv_filePath(QList<QMediaContent>)));
    connect(parent, SIGNAL(send_itemDoubleClicked(int)), this, SLOT(recv_itemDoubleClicked(int)));
}

void PlayerBar::initConfig()
{
    m_pPlayer->setPlaylist(m_pPlayerList);

    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->CurrentItemOnce);

    m_pVolumeSlider->setMinimum(0);
    m_pVolumeSlider->setMaximum(100);
    m_pVolumeSlider->setSingleStep(1);
    m_pVolumeSlider->setValue(50);
    m_pPlayer->setVolume(50);
}

void PlayerBar::resizeEvent(QResizeEvent *event)
{
    m_pBackgroundFrame->setGeometry(0,0,width(),height());

    m_pPreviousButton->setGeometry(30, 15, 50, 50);
    m_pPlayButton->setGeometry(90, 15, 50, 50);
    m_pPauseButton->setGeometry(90, 15, 50, 50);
    m_pNextButton->setGeometry(150, 15, 50, 50);

    m_pStopButton->setGeometry(210, 15, 50, 50);

    m_pTitleLable->setGeometry(270,15,150,20);
    m_pPlaySlider->setGeometry(270,32,width()-270-230,20);
    m_pTimeWidget->setGeometry(width()-230-105,15,105,20);

    m_pCurrentItemOnceButton->setGeometry(width()-220, 15, 50, 50);
    m_pCurrentItemInLoopButton->setGeometry(width()-220, 15, 50, 50);
    m_pSequentialButton->setGeometry(width()-220, 15, 50, 50);
    m_pLoopButton->setGeometry(width()-220, 15, 50, 50);
    m_pRandomButton->setGeometry(width()-220, 15, 50, 50);

    m_pVolumeSlider->setGeometry(width()-160, 15, 10, 50);

    m_pVolumeLoudButton->setGeometry(width()-140, 15, 50, 50);
    m_pVolumeQuietButton->setGeometry(width()-140, 15, 50, 50);
    m_pVolumeOffButton->setGeometry(width()-140, 15, 50, 50);

    m_pPlayListButton->setGeometry(width()-80, 15, 50, 50);

    event->ignore();
}

void PlayerBar::playButton_clicked()
{
    m_pPlayer->play();
}

void PlayerBar::pauseButton_clicked()
{
    m_pPlayer->pause();
}

void PlayerBar::stopButton_clicked()
{
    m_pPlayer->stop();
}

void PlayerBar::previousButton_clicked()
{
    m_pPlayer->playlist()->previous();
}

void PlayerBar::nextButton_clicked()
{
    m_pPlayer->playlist()->next();
}

void PlayerBar::volumeOnButton_clicked()
{
    m_pPlayer->setVolume(0);
}

void PlayerBar::volumeOffButton_clicked()
{
    m_pPlayer->setVolume(m_pVolumeSlider->value());
}

void PlayerBar::volumeButtonChange(int value)
{
    m_pVolumeSlider->setValue(value);

    if(value == 0)
    {
        m_pVolumeOffButton->setVisible(true);
        m_pVolumeLoudButton->setVisible(false);
        m_pVolumeQuietButton->setVisible(false);
    }
    else if(value > 50)
    {
        m_pVolumeOffButton->setVisible(false);
        m_pVolumeLoudButton->setVisible(true);
        m_pVolumeQuietButton->setVisible(false);
    }
    else
    {
        m_pVolumeOffButton->setVisible(false);
        m_pVolumeLoudButton->setVisible(false);
        m_pVolumeQuietButton->setVisible(true);
    }
}

void PlayerBar::playlistButton_clicked()
{
    emit send_operate_playListBar();
}

void PlayerBar::currentItemOnceButton_clicked()
{
    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->CurrentItemInLoop);
}

void PlayerBar::currentItemInLoopButton_clicked()
{
    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->Sequential);
}

void PlayerBar::sequentialButton_clicked()
{
    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->Loop);
}

void PlayerBar::loopButton_clicked()
{
    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->Random);
}

void PlayerBar::randomButton_clicked()
{
    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->CurrentItemOnce);
}

QString PlayerBar::settime(qint64 playtime)
{
    qint64 hour, minute, second;

    qint64 time = playtime / 1000;  //获得的 playtime 时间是以毫秒为单位的

    hour = time / 3600;
    minute = (time - hour * 3600) / 60;
    second = time - hour * 3600 - minute * 60;

//    if(minute < 10)
//    {
//        if(second < 10)
//        {
//            return QString("0%1:0%2").arg(minute).arg(second);
//        }
//        return QString("0%1:%2").arg(minute).arg(second);
//    }
//    return QString("%1:%2").arg(minute).arg(second);  //把int型转化为string类型后再设置为label的text

    return QString("%1%2:%3%4:%5%6").arg(hour/10).arg(hour%10).arg(minute/10).arg(minute%10).arg(second/10).arg(second%10);
}

void PlayerBar::getduration(qint64 playtime)
{
    m_pDurationLable->setText(settime(playtime));

    m_pPlaySlider->setMinimum(0);
    m_pPlaySlider->setMaximum(int(playtime));
    m_pPlaySlider->setSingleStep(1);
}

void PlayerBar::positionChanged(qint64 playtime)
{
    m_pProgressLable->setText(settime(playtime));
    m_pPlaySlider->setValue(int(playtime));
}


void PlayerBar::volumeSlider_valueChanged(int value)
{
    m_pPlayer->setVolume(value);
}

void PlayerBar::playSlider_valueChanged(int value)
{
    if(value > m_pPlayer->position() + 1000)
    {
        m_pPlayer->setPosition(value);
    }
}

void PlayerBar::modeButtonChange(QMediaPlaylist::PlaybackMode mode)
{
    for(int i = 0; i < 5; i++)
    {
        m_ButtonList.value(i)->setVisible(i == mode);
    }
}

void PlayerBar::playButtonChange(QMediaPlayer::State state)
{
    bool bPlayVisible;

    switch (state) {
    case m_pPlayer->PlayingState:
        bPlayVisible = false;
        break;
    case m_pPlayer->PausedState:
        bPlayVisible = true;
        break;
    case m_pPlayer->StoppedState:
        bPlayVisible = true;
        break;
    }

    m_pPlayButton->setVisible(bPlayVisible);
    m_pPauseButton->setVisible(!bPlayVisible);
}

void PlayerBar::nowPlaying(const QMediaContent &content)
{
    m_pTitleLable->setText(content.canonicalUrl().url().split("/").last());
}

void PlayerBar::showMediaList(int start, int end)
{
    QString fileName;
    QList<QString> strNameList;

    while (start <= end) {
        fileName = m_pPlayerList->media(start).canonicalUrl().url().split("/").last();
        strNameList << fileName;
        start++;
    }

    emit send_fileNameList(strNameList);
}

void PlayerBar::recv_filePath(QList<QMediaContent> filePathList)
{
     m_pPlayerList->addMedia(filePathList);
}

void PlayerBar::recv_itemDoubleClicked(int index)
{
    m_pPlayerList->setCurrentIndex(index);
    m_pPlayer->play();
}

PlayerBar::~PlayerBar()
{
    delete ui;
}
