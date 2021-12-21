#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // 设置属性
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QApplication a(argc, argv);

    // 添加外部字体
    QFontDatabase::addApplicationFont(":/font/font/MoscowMetroColor.otf"); // 从source资源文件

    MainWidget w;
    w.show();
    return a.exec();
}
