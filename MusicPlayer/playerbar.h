#ifndef PLAYERBAR_H
#define PLAYERBAR_H

#include <QWidget>

#include "common.h"
#include "playbutton.h"

namespace Ui {
class PlayerBar;
}

class PlayerBar : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerBar(QWidget *parent = nullptr);
    ~PlayerBar();

private:
    Ui::PlayerBar *ui;

private:
    QFrame *m_pBackgroundFrame;

    QWidget *m_pTimeWidget;

    QLabel *m_pTitleLable;
    QLabel *m_pIconLable;
    QLabel *m_pProgressLable;
    QLabel *m_pDivisionLable;
    QLabel *m_pDurationLable;

    QPushButton *m_pPlayButton;
    QPushButton *m_pPauseButton;
    QPushButton *m_pStopButton;
    QPushButton *m_pPreviousButton;
    QPushButton *m_pNextButton;

    QPushButton *m_pCurrentItemOnceButton;
    QPushButton *m_pCurrentItemInLoopButton;
    QPushButton *m_pSequentialButton;
    QPushButton *m_pLoopButton;
    QPushButton *m_pRandomButton;
    QList<QPushButton *> m_ButtonList;

    QPushButton *m_pVolumeLoudButton;
    QPushButton *m_pVolumeQuietButton;
    QPushButton *m_pVolumeOffButton;

    QPushButton *m_pPlayListButton;

    QSlider *m_pPlaySlider;
    QSlider *m_pVolumeSlider;

    bool m_bMute = false;
    int m_nVolume = 0;

    // 界面初始化
    void initUI();
    // 播放初始化
    void initMedia();
    // 信号槽初始化
    void initConnect(QWidget *parent);
    // 配置初始化
    void initConfig();

protected:
    // 监听窗体缩放事件
    void resizeEvent(QResizeEvent *event);

public:
    QMediaPlayer *m_pPlayer;
    QMediaPlaylist *m_pPlayerList;
    QList<QMediaContent> m_MediaList;

    QString settime(qint64 playtime);

signals:
    void send_operate_playListBar();

    void send_fileNameList(QList<QString> strNameList);

private slots:
    void playButton_clicked();

    void pauseButton_clicked();

    void stopButton_clicked();

    void previousButton_clicked();

    void nextButton_clicked();

    void volumeOnButton_clicked();
    void volumeOffButton_clicked();

    void playlistButton_clicked();

    void currentItemOnceButton_clicked();
    void currentItemInLoopButton_clicked();
    void sequentialButton_clicked();
    void loopButton_clicked();
    void randomButton_clicked();

    void getduration(qint64 playtime);

    void positionChanged(qint64 playtime);

    void playSlider_valueChanged(int value);

    void volumeSlider_valueChanged(int value);

    void playButtonChange(QMediaPlayer::State state);

    void nowPlaying(const QMediaContent &content);

    void showMediaList(int start, int end);

    void modeButtonChange(QMediaPlaylist::PlaybackMode mode);

    void recv_filePath(QList<QMediaContent> filePathList);

    void recv_itemDoubleClicked(int index);

    void volumeButtonChange(int value);
};

#endif // PLAYERBAR_H
