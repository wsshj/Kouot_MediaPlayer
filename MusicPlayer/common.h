#ifndef COMMON_H
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

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#pragma comment(lib, "user32.lib")
#else
#endif

#endif // COMMON_H
