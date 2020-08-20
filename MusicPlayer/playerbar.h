#ifndef PLAYERBAR_H
#define PLAYERBAR_H

#include <QWidget>

#include "common.h"

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
    QPushButton *m_pModeButton;
    QPushButton *m_pVolumeButton;
    QPushButton *m_pPlayListButton;

    QSlider *m_pPlaySlider;
    QSlider *m_pVolumeSlider;

    bool m_bMute = false;
    int m_nVolume = 0;

    // 界面初始化
    void initUI();
    // 信号槽初始化
    void initConnect(QWidget *parent);
    // 界面初始化
    void initMedia();

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
    void on_playButton_clicked();

    void on_stopButton_clicked();

    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_modeButton_clicked();

    void on_volumeButton_clicked();

    void on_playlistButton_clicked();

    void getduration(qint64 playtime);

    void positionChanged(qint64 playtime);

    void on_playSlider_valueChanged(int value);

    void on_volumeSlider_valueChanged(int value);

    void playButtonChange(QMediaPlayer::State state);

    void nowPlaying(const QMediaContent &content);

    void showMediaList(int start, int end);

    void modeButtonChange(QMediaPlaylist::PlaybackMode mode);

    void recv_filePath(QList<QMediaContent> filePathList);

    void recv_itemDoubleClicked(int index);

    void volumeButtonChange(int value);
};

#endif // PLAYERBAR_H
