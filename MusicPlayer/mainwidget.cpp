#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

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

    // 安装事件过滤器，标题栏中 eventFilter 将监听主界面事件
    this->installEventFilter(m_pTitleBar);

    // 设置主窗体
    this->resize(960, 640);
    this->setMinimumSize(600, 400);
    this->setWindowIcon(QIcon(":/image/image/icon.png"));
    this->setWindowTitle("rouot");

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

    connect(m_pPlayerBar,SIGNAL(send_fileNameList(QList<QString>)),this,SLOT(recv_fileNameList(QList<QString>)));
    connect(m_pPlayerBar,SIGNAL(send_operate_playListBar()),this,SLOT(recv_operate_playListBar()));
    connect(m_pPlayListBar,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_listWidget_itemDoubleClicked(QListWidgetItem*)));
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    m_pTitleBar->setGeometry(0, 0, width(), 30);
    m_pMovieLable->setGeometry(0, 30, width(), height()-30);
    m_pPlayerBar->setGeometry(0, height()-100, width(), 100);
    m_pPlayListBar->setGeometry(width()-200, 30, 200, height()-130);

    event->ignore();
}

bool MainWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        // 计算鼠标在窗体中的位置坐标
        int xPos = GET_X_LPARAM(param->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(param->lParam) - this->frameGeometry().y();

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
    filters<<QString("*.flac")<<QString("*.wav")<<QString("*.mp3")<<QString("*.ape");

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

        if(file_extend == "flac" || file_extend == "wav" || file_extend == "mp3" || file_extend == "ape")
        {
            filePathList << QUrl::fromLocalFile(file_path);
        }
    }

    emit send_filePath(filePathList);
}

MainWidget::~MainWidget()
{
    delete ui;
}
