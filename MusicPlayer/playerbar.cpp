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
}

void PlayerBar::initUI()
{
    m_pTimeWidget = new QWidget(this);

    m_pTitleLable = new QLabel(this);
    m_pIconLable = new QLabel(this);
    m_pProgressLable = new QLabel(this);
    m_pDivisionLable = new QLabel(this);
    m_pDurationLable = new QLabel(this);

    m_pPlayButton = new QPushButton(this);
    m_pStopButton = new QPushButton(this);
    m_pPreviousButton = new QPushButton(this);
    m_pNextButton = new QPushButton(this);
    m_pModeButton = new QPushButton(this);
    m_pVolumeButton = new QPushButton(this);
    m_pPlayListButton = new QPushButton(this);

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

    // 设置 objectName 供外部调用
    m_pPlayButton->setObjectName("playButton");
    m_pStopButton->setObjectName("stopButton");
    m_pPreviousButton->setObjectName("previousButton");
    m_pNextButton->setObjectName("nextButton");
    m_pModeButton->setObjectName("modeButton");
    m_pVolumeButton->setObjectName("volumeButton");
    m_pPlayListButton->setObjectName("playlistButton");

    // 设置工具提示
    m_pPlayButton->setToolTip("播放/暂停");
    m_pStopButton->setToolTip("停止");
    m_pPreviousButton->setToolTip("前一首");
    m_pNextButton->setToolTip("后一首");
    m_pModeButton->setToolTip("播放模式：单曲播放");
    m_pVolumeButton->setToolTip("音量");
    m_pPlayListButton->setToolTip("播放列表");

    // 设置按键图标
    m_pPlayButton->setIcon(QIcon(":/image/image/play.svg"));
    m_pStopButton->setIcon(QIcon(":/image/image/stop.svg"));
    m_pPreviousButton->setIcon(QIcon(":/image/image/back.svg"));
    m_pNextButton->setIcon(QIcon(":/image/image/forward.svg"));
    m_pModeButton->setIcon(QIcon(":/image/image/redo.svg"));
    m_pVolumeButton->setIcon(QIcon(":/image/image/volume-loud.svg"));
    m_pPlayListButton->setIcon(QIcon(":/image/image/list.svg"));

    // 设置按键图标尺寸
    m_pPlayButton->setIconSize(QSize(50, 50));
    m_pStopButton->setIconSize(QSize(50, 50));
    m_pPreviousButton->setIconSize(QSize(50, 50));
    m_pNextButton->setIconSize(QSize(50, 50));
    m_pModeButton->setIconSize(QSize(50, 50));
    m_pVolumeButton->setIconSize(QSize(50, 50));
    m_pPlayListButton->setIconSize(QSize(50, 50));

    // 去除按键边框
    m_pPlayButton->setStyleSheet("border:none;");
    m_pStopButton->setStyleSheet("border:none;");
    m_pPreviousButton->setStyleSheet("border:none;");
    m_pNextButton->setStyleSheet("border:none;");
    m_pModeButton->setStyleSheet("border:none;");
    m_pVolumeButton->setStyleSheet("border:none;");
    m_pPlayListButton->setStyleSheet("border:none;");

    // 设置拖动条样式
    m_pPlaySlider->setStyleSheet("QSlider::groove:horizontal{height: 4px; border-radius: 2px;}"
                                 "QSlider::sub-page:horizontal{height: 4px; border-radius: 2px; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.181818 rgba(32, 234, 230, 255), stop:0.471591 rgba(62, 168, 255, 255), stop:0.761364 rgba(35, 41, 214, 255));}"
                                 "QSlider::add-page:horizontal{height: 4px; border-radius: 2px; background-color: rgb(200, 200, 200);}"
                                 "QSlider::handle:horizontal{width: 8px; margin-top: -2px; margin-bottom: -2px; border-radius: 4px; background-color: rgb(35, 41, 214);}"
                                 "QSlider::handle:horizontal:hover {width: 10px; margin-top: -3px; margin-bottom: -3px; border-radius: 5px; background-color: rgb(66, 132, 198);}"
                                 );

    m_pVolumeSlider->setStyleSheet("QSlider::groove:vertical{width: 2px; border-radius: 1px;}"
                                 "QSlider::sub-page:vertical{width: 2px; border-radius: 1px; background-color: rgba(200, 200, 200, 255);}"
                                 "QSlider::add-page:vertical{width: 2px; border-radius: 1px; background-color: rgba(35, 41, 214, 255);}"
                                 "QSlider::handle:vertical{height: 2px; border-radius: 1px; background-color: rgb(35, 41, 214);}"
                                 "QSlider::handle:vertical:hover {height: 8px; margin-left: -3px; margin-right: -3px; border-radius: 4px; background-color: rgb(66, 132, 198);}"
                                 );

    // 创建画布填充背景色
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(255,255,255,170));
    setAutoFillBackground(true);
    setPalette(pal);
}

void PlayerBar::initMedia()
{
    m_pPlayer = new QMediaPlayer;
    m_pPlayerList = new QMediaPlaylist;

    m_pPlayer->setPlaylist(m_pPlayerList);

    m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->CurrentItemOnce);

    m_pVolumeSlider->setMinimum(0);
    m_pVolumeSlider->setMaximum(100);
    m_pVolumeSlider->setSingleStep(1);
    m_pVolumeSlider->setValue(100);
}

void PlayerBar::initConnect(QWidget *parent)
{
    connect(m_pPlayButton, SIGNAL(clicked()), this, SLOT(on_playButton_clicked()));
    connect(m_pStopButton, SIGNAL(clicked()), this, SLOT(on_stopButton_clicked()));
    connect(m_pPreviousButton, SIGNAL(clicked()), this, SLOT(on_previousButton_clicked()));
    connect(m_pNextButton, SIGNAL(clicked()), this, SLOT(on_nextButton_clicked()));
    connect(m_pModeButton, SIGNAL(clicked()), this, SLOT(on_modeButton_clicked()));
    connect(m_pVolumeButton, SIGNAL(clicked()), this, SLOT(on_volumeButton_clicked()));
    connect(m_pPlayListButton, SIGNAL(clicked()), this, SLOT(on_playlistButton_clicked()));

    connect(m_pPlaySlider, SIGNAL(valueChanged(int)), this, SLOT(on_playSlider_valueChanged(int)));
    connect(m_pVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_volumeSlider_valueChanged(int)));

    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(m_pPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(getduration(qint64)));
    connect(m_pPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playButtonChange(QMediaPlayer::State)));
    connect(m_pPlayer, SIGNAL(currentMediaChanged(const QMediaContent)), this, SLOT(nowPlaying(const QMediaContent)));
    connect(m_pPlayer, SIGNAL(volumeChanged(int)), this, SLOT(volumeButtonChange(int)));

    connect(m_pPlayerList, SIGNAL(mediaInserted(int, int)), this, SLOT(showMediaList(int, int)));
    connect(m_pPlayerList, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)), this, SLOT(modeButtonChange(QMediaPlaylist::PlaybackMode)));

    connect(parent, SIGNAL(send_filePath(QList<QMediaContent>)), this, SLOT(recv_filePath(QList<QMediaContent>)));
    connect(parent, SIGNAL(send_itemDoubleClicked(int)), this, SLOT(recv_itemDoubleClicked(int)));
}

void PlayerBar::resizeEvent(QResizeEvent *event)
{
    m_pPreviousButton->setGeometry(30, 15, 50, 50);
    m_pPlayButton->setGeometry(90, 15, 50, 50);
    m_pNextButton->setGeometry(150, 15, 50, 50);

    m_pStopButton->setGeometry(210, 15, 50, 50);

    m_pTitleLable->setGeometry(270,15,150,20);
    m_pPlaySlider->setGeometry(270,32,width()-270-230,20);
    m_pTimeWidget->setGeometry(width()-230-100,15,100,20);

    m_pModeButton->setGeometry(width()-220, 15, 50, 50);
    m_pVolumeSlider->setGeometry(width()-160, 15, 10, 50);
    m_pVolumeButton->setGeometry(width()-140, 15, 50, 50);
    m_pPlayListButton->setGeometry(width()-80, 15, 50, 50);

    event->ignore();
}

void PlayerBar::on_playButton_clicked()
{
    if(m_pPlayer->state() == m_pPlayer->PlayingState)
    {
        m_pPlayer->pause();
    }
    else
    {
        m_pPlayer->play();
    }
}

void PlayerBar::on_stopButton_clicked()
{
    m_pPlayer->stop();
}

void PlayerBar::on_previousButton_clicked()
{
    m_pPlayer->playlist()->previous();
}

void PlayerBar::on_nextButton_clicked()
{
    m_pPlayer->playlist()->next();
}

void PlayerBar::on_modeButton_clicked()
{
    int mod = m_pPlayer->playlist()->playbackMode() ;
    if(mod == 4)
    {
        mod = -1;
    }
    mod++;

    switch (mod) {
    case 0: //单曲，播完停止
        m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->CurrentItemOnce);
        break;
    case 1: //单曲，循环
        m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->CurrentItemInLoop);
        break;
    case 2: //列表，播完停止
        m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->Sequential);
        break;
    case 3: //列表，循环
        m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->Loop);
        break;
    case 4: //随机
        m_pPlayer->playlist()->setPlaybackMode(m_pPlayerList->Random);
        break;
    default:
        break;
    }
}

void PlayerBar::on_volumeButton_clicked()
{
    if(m_bMute)
    {
        m_pVolumeButton->setIcon(QIcon(":/image/image/volume-off.svg"));
        m_pPlayer->setVolume(m_nVolume);
        m_pVolumeSlider->setValue(m_nVolume);
        m_bMute = false;
    }
    else
    {
        m_nVolume = m_pPlayer->volume();
        m_pPlayer->setVolume(0);
        m_pVolumeButton->setIcon(QIcon(":/image/image/mute.svg"));
        m_pVolumeSlider->setValue(0);
        m_bMute = true;
    }
}

void PlayerBar::volumeButtonChange(int value)
{
    if(value == 0)
    {
        m_pVolumeButton->setIcon(QIcon(":/image/image/volume-off.svg"));
    }
    else if(value > 50)
    {
        m_pVolumeButton->setIcon(QIcon(":/image/image/volume-loud.svg"));
    }
    else
    {
        m_pVolumeButton->setIcon(QIcon(":/image/image/volume-quiet.svg"));
    }
}

void PlayerBar::on_playlistButton_clicked()
{
    emit send_operate_playListBar();
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


void PlayerBar::on_volumeSlider_valueChanged(int value)
{
    m_pPlayer->setVolume(value);
}

void PlayerBar::on_playSlider_valueChanged(int value)
{
    if(value > m_pPlayer->position() + 1000)
    {
        m_pPlayer->setPosition(value);
    }
}

void PlayerBar::modeButtonChange(QMediaPlaylist::PlaybackMode mode)
{
    switch (mode) {
    case QMediaPlaylist::CurrentItemOnce: //单曲，播完停止
        m_pModeButton->setIcon(QIcon(":/image/image/redo.svg"));
        m_pModeButton->setToolTip("播放模式：单曲播放");
        break;
    case QMediaPlaylist::CurrentItemInLoop: //单曲，循环
        m_pModeButton->setIcon(QIcon(":/image/image/refresh.svg"));
        m_pModeButton->setToolTip("播放模式：单曲循环");
        break;
    case QMediaPlaylist::Sequential: //列表，播完停止
        m_pModeButton->setIcon(QIcon(":/image/image/redo.svg"));
        m_pModeButton->setToolTip("播放模式：顺序播放");
        break;
    case QMediaPlaylist::Loop: //列表，循环
        m_pModeButton->setIcon(QIcon(":/image/image/rotate.svg"));
        m_pModeButton->setToolTip("播放模式：循环播放");
        break;
    case QMediaPlaylist::Random: //随机
        m_pModeButton->setIcon(QIcon(":/image/image/shuffle.svg"));
        m_pModeButton->setToolTip("播放模式：随机播放");
        break;
    }
}

void PlayerBar::playButtonChange(QMediaPlayer::State state)
{
    switch (state) {
    case m_pPlayer->PlayingState:
        m_pPlayButton->setIcon(QIcon(":/image/image/pause.svg"));
        break;
    case m_pPlayer->PausedState:
        m_pPlayButton->setIcon(QIcon(":/image/image/play.svg"));
        break;
    case m_pPlayer->StoppedState:
        m_pPlayButton->setIcon(QIcon(":/image/image/play.svg"));
        break;
    }
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

void PlayerBar::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        on_playButton_clicked();
        return;
    case Qt::Key_Left:
        m_pPlayer->setPosition(m_pPlayer->position() - 5000);
        return;
    case Qt::Key_Right:
        m_pPlayer->setPosition(m_pPlayer->position() + 5000);
        return;
    }
}

PlayerBar::~PlayerBar()
{
    delete ui;
}
