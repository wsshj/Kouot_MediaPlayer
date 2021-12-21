#include "common.h"

void loadQss(QString url, QWidget *widget)
{
    QFile qssFile(url);
    qssFile.open(QFile::ReadOnly);

    if(qssFile.isOpen())
    {
        widget->setStyleSheet(QLatin1String(qssFile.readAll()));
        qssFile.close();
    }
}
