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

public:
    PlayButton(const QString &objectName, const QString &tip, QWidget *parent = nullptr, bool visible = true);
    ~PlayButton();
};

#endif // PLAYBUTTON_H
