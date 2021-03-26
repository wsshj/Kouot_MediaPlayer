#ifndef PLAYBUTTON_H
#define PLAYBUTTON_H

#include <QPushButton>

class PlayButton : public QPushButton
{
private:
    // 按键图标宽度
    int m_nWidth = 50;
    // 按键图标高度
    int m_nHeight = 50;
    // 按键样式
    QString m_strStyleSheet = "border:none;";

public:
    PlayButton(const QString &objectName, const QString &tip, QIcon icon, QWidget *parent = nullptr, bool visible = true);
    ~PlayButton();
};

#endif // PLAYBUTTON_H
