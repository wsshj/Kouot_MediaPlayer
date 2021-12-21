#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 如果一个 Widget 已经 grabKeyboard，所有键盘事件将发送到该Widget而不是获得焦点的Widget
    this->grabKeyboard();

    initUI();
}

void MainWidget::initUI()
{
    // 设置主窗体无边框，注意函数名为 setWindowFlags ，不要少写了最后的 s 。
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_nBorderWidth = 5;
    m_pMovieLable = new QLabel(this);
    m_pTitleBar = new TitleBar(this);
    m_pPlayerBar = new PlayerBar(this);
    m_pPlayListBar = new QListWidget(this);

    m_pVideoWidget = new QVideoWidget(this);
    m_pPlayerBar->m_pPlayer->setVideoOutput(m_pVideoWidget);
//    m_pVideoWidget->setVisible(false);


    // 安装事件过滤器，标题栏中 eventFilter 将监听主界面事件
    this->installEventFilter(m_pTitleBar);

    // 设置主窗体
    this->resize(960, 640);
    this->setMinimumSize(600, 400);
    this->setWindowIcon(QIcon(":/image/image/Music Note.ico"));
    this->setWindowTitle("kouot");

    // 动图播放器
    m_pMovie = new QMovie(":/image/image/background.gif");
    m_pMovieLable->setScaledContents(true);
    m_pMovieLable->setMovie(m_pMovie);
    m_pMovie->start();

    // 窗口允许外部文件拖入
    this->setAcceptDrops(true);

    // 设置透明度
    m_pPlayListBar->setStyleSheet("border:none;background-color: rgba(255, 255, 255, 0.5);");
    m_pPlayListBar->setVisible(false);

    m_pTimer = new QTimer(this);

//    QGridLayout* layout = new QGridLayout(this);
//    layout->setContentsMargins(0,0,0,0);
//    layout->addWidget(m_pVideoWidget);
//    this->setLayout(layout);
//    m_pVideoWidget->lower();

    m_pVideoWidget->lower();
    m_pTitleBar->raise();
    m_pPlayerBar->raise();
    m_pPlayListBar->raise();

    connect(m_pPlayerBar,SIGNAL(send_fileNameList(QList<QString>)),this,SLOT(recv_fileNameList(QList<QString>)));
    connect(m_pPlayerBar,SIGNAL(send_operate_playListBar()),this,SLOT(recv_operate_playListBar()));
    connect(m_pPlayListBar,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_listWidget_itemDoubleClicked(QListWidgetItem*)));
    connect(m_pPlayerBar->m_pPlayer,SIGNAL(videoAvailableChanged(bool)),this,SLOT(VideoWidgetStatus(bool)));
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void MainWidget::fullScreen()
{
    if(isFullScreen())
    {
        showNormal();
        m_pMovieLable->setMouseTracking(false);
        m_pVideoWidget->setMouseTracking(false);
        setMouseTracking(false);

        setCursor(Qt::ArrowCursor);

        m_pTitleBar->setVisible(true);
        m_pPlayerBar->setVisible(true);
    }
    else
    {
        showFullScreen();
        m_pMovieLable->setMouseTracking(true);
        m_pVideoWidget->setMouseTracking(true);
        setMouseTracking(true);
//        m_pVideoWidget->setGeometry(0, 0, width(), height());

        m_pVideoWidget->setAutoFillBackground(true);
    }
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    m_pTitleBar->setGeometry(0, 0, width(), 30);
    m_pMovieLable->setGeometry(0, 0, width(), height());
    m_pPlayerBar->setGeometry(0, height()-80, width(), 80);
    m_pPlayListBar->setGeometry(width()-200, 30, 200, height()-110);
    m_pVideoWidget->setGeometry(0, 30, width()-200, height()-110);

    event->ignore();
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Space:
        if(m_pPlayerBar->m_pPlayer->state() == QMediaPlayer::PlayingState)
        {
            m_pPlayerBar->m_pPlayer->pause();
        }
        else if(m_pPlayerBar->m_pPlayer->state() == QMediaPlayer::PausedState)
        {
            m_pPlayerBar->m_pPlayer->play();
        }
        return;
    case Qt::Key_Left:
        m_pPlayerBar->m_pPlayer->setPosition(m_pPlayerBar->m_pPlayer->position() - 5000);
        return;
    case Qt::Key_Right:
        m_pPlayerBar->m_pPlayer->setPosition(m_pPlayerBar->m_pPlayer->position() + 5000);
        return;
    case Qt::Key_Up:
        m_pPlayerBar->m_pPlayer->setVolume(m_pPlayerBar->m_pPlayer->volume() + 2);
        return;
    case Qt::Key_Down:
        m_pPlayerBar->m_pPlayer->setVolume(m_pPlayerBar->m_pPlayer->volume() - 2);
        return;
    case Qt::Key_1:
        m_pPlayerBar->m_pPlayer->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        return;
    case Qt::Key_2:
        m_pPlayerBar->m_pPlayer->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        return;
    case Qt::Key_3:
        m_pPlayerBar->m_pPlayer->playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
        return;
    case Qt::Key_4:
        m_pPlayerBar->m_pPlayer->playlist()->setPlaybackMode(QMediaPlaylist::Loop);
        return;
    case Qt::Key_5:
        m_pPlayerBar->m_pPlayer->playlist()->setPlaybackMode(QMediaPlaylist::Random);
        return;
    case Qt::Key_F11:
        fullScreen();
        return;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isFullScreen())
    {
        qDebug() << event->pos();

        m_pTimer->start(2000);
        setCursor(Qt::ArrowCursor);
        m_pTitleBar->setVisible(true);
        m_pPlayerBar->setVisible(true);

        m_pVideoWidget->setGeometry(0, 30, width()-200, height()-110);

    }
}

bool MainWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    if(isFullScreen())
    {
        return false;
    }

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
//        // 计算鼠标在屏幕中的位置坐标,受分辨率影响
//        int xPos = GET_X_LPARAM(param->lParam) - this->frameGeometry().x();
//        int yPos = GET_Y_LPARAM(param->lParam) - this->frameGeometry().y();

        // 计算鼠标在窗体中的位置坐标
        int xPos = mapFromGlobal(QCursor().pos()).x();
        int yPos = mapFromGlobal(QCursor().pos()).y();

        // 鼠标区域位于窗体边框，进行缩放。可以模拟窗体边框缩放效果
        if(xPos < m_nBorderWidth && yPos<m_nBorderWidth)                    //左上角
            *result = HTTOPLEFT;
        else if(xPos>=width()-m_nBorderWidth&&yPos<m_nBorderWidth)          //右上角
            *result = HTTOPRIGHT;
        else if(xPos<m_nBorderWidth&&yPos>=height()-m_nBorderWidth)         //左下角
            *result = HTBOTTOMLEFT;
        else if(xPos>=width()-m_nBorderWidth&&yPos>=height()-m_nBorderWidth)//右下角
            *result = HTBOTTOMRIGHT;
        else if(xPos < m_nBorderWidth)                                     //左边
            *result = HTLEFT;
        else if(xPos>=width()-m_nBorderWidth)                              //右边
            *result = HTRIGHT;
        else if(yPos<m_nBorderWidth)                                       //上边
            *result = HTTOP;
        else if(yPos>=height()-m_nBorderWidth)                             //下边
            *result = HTBOTTOM;
        else              //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void MainWidget::recv_operate_playListBar()
{
    if(m_pPlayListBar->isVisible())
    {
        m_pPlayListBar->setVisible(false);
    }
    else
    {
        m_pPlayListBar->setVisible(true);
    }
}

void MainWidget::recv_fileNameList(QList<QString> strNameList)
{
    m_pPlayListBar->addItems(strNameList);
}

QList<QMediaContent> MainWidget::traverseFolder(QString path, QList<QMediaContent> filePathList)
{
    //判断路径是否存在
    QDir dir(path);
    if(!dir.exists())
    {
        return filePathList;
    }

    //获取所选文件类型过滤器
    QStringList filters;
    filters<<QString("*.flac")<<QString("*.wav")<<QString("*.mp3")<<QString("*.ape")<<QString("*.rmvb")<<QString("*.mp4");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path,
        filters,
        QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);

    QStringList string_list;

    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        filePathList << QUrl::fromLocalFile(absolute_file_path);
    }

    return filePathList;
}

void MainWidget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit send_itemDoubleClicked(m_pPlayListBar->row(item));
}

//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void MainWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

//当用户放下这个文件后，就会触发dropEvent事件
void MainWidget::dropEvent(QDropEvent *event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();
    QList<QMediaContent> filePathList;

    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls)
    {
        QString file_path = url.toLocalFile();

        QFileInfo fileInfo(file_path);

        if(fileInfo.isDir())
        {
            filePathList << traverseFolder(file_path, filePathList);
            continue;
        }

        QString file_extend = fileInfo.suffix();

        if(file_extend == "flac" || file_extend == "wav" || file_extend == "mp3" || file_extend == "ape" || file_extend == "rmvb" || file_extend == "mp4")
        {
            filePathList << QUrl::fromLocalFile(file_path);
        }
    }

    emit send_filePath(filePathList);
}

void MainWidget::VideoWidgetStatus(bool videoAvailable)
{
    m_pVideoWidget->setVisible(videoAvailable);
}

void MainWidget::onTimeout()
{
    if (mapFromGlobal(QCursor().pos()).y() < 30 || mapFromGlobal(QCursor().pos()).y() > height() - 80)
    {
        qDebug() << "dada";
        m_pTimer->stop();
        return;
    }

    if(isFullScreen())
    {
        setCursor(Qt::BlankCursor); //隐藏鼠标

        m_pTitleBar->setVisible(false);
        m_pPlayerBar->setVisible(false);

        m_pVideoWidget->setGeometry(0, 0, width(), height());
    }

}

MainWidget::~MainWidget()
{
    delete ui;
}
