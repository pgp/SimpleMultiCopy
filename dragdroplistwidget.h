#ifndef DRAGDROPLISTWIDGET_H
#define DRAGDROPLISTWIDGET_H

#include <QListWidget>
#include <QObject>
#include <QWidget>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QtDebug>
#include "qmainwidget.h"

class DragDropListWidget : public QListWidget
{
    Q_OBJECT

public:
    QMainWidget* refMainWidget;
    bool whichList;
    explicit DragDropListWidget(QWidget *parent = 0);
    void setMainWidget(QMainWidget* refMainWidget_, bool whichList_);

protected:
    void dropEvent(QDropEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
};

#endif // DRAGDROPLISTWIDGET_H
