﻿#ifndef COMMON_H
#define COMMON_H

#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QMovie>
#include <QIcon>
#include <QSlider>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QDir>
#include <QDirIterator>
#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QListWidget>
#include <QList>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QStringList>
#include <QFont>
#include <QFontDatabase>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#pragma comment(lib, "user32.lib")
#else
#endif

void loadQss(QString url, QWidget *widget);

#endif // COMMON_H
