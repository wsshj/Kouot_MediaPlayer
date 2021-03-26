#include "playbutton.h"

PlayButton::PlayButton(const QString &objectName, const QString &tip, QIcon icon, QWidget *parent, bool visible):QPushButton(parent)
{
    // 设置 objectName 供外部调用
    this->setObjectName(objectName);
    // 设置工具提示
    this->setToolTip(tip);
    // 设置按键图标
    this->setIcon(icon);
    // 设置按键图标尺寸
    this->setIconSize(QSize(m_nWidth, m_nHeight));
    // 设置样式
    this->setStyleSheet(m_strStyleSheet);
    // 设置显示/隐藏
    this->setVisible(visible);
}

PlayButton::~PlayButton()
{

}
