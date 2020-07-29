#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "common.h"
#include "titlebar.h"
#include "playerbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

private:
    // 标题栏
    TitleBar *m_pTitleBar;

    // 播放器栏
    PlayerBar *m_pPlayerBar;

    // 播放列表栏
    QListWidget *m_pPlayListBar;

    // 模拟窗体边框的宽度
    int m_nBorderWidth;

    // 背景动图播放器
    QMovie *m_pMovie;

    // 放动图播放器的标签
    QLabel *m_pMovieLable;

    // 界面初始化
    void initUI();

protected:
    // 监听窗体缩放事件
    void resizeEvent(QResizeEvent *event);

    // 用来获取windows的事件处理
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    QList<QMediaContent> traverseFolder(QString path, QList<QMediaContent> filePathList);

signals:
    // 发送文件路径
    void send_filePath(QList<QMediaContent> filePathList);

    // 发送在列表中选中的文件的index
    void send_itemDoubleClicked(int index);

private slots:
    //  操作播放列表显示隐藏
    void recv_operate_playListBar();

    // 发送播放列表中文件名称，在播放列表中显示
    void recv_fileNameList(QList<QString> strNameList);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void dragEnterEvent(QDragEnterEvent *event);

    void dropEvent(QDropEvent *event);
};
#endif // MAINWIDGET_H
