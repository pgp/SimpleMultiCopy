#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include "thewidgetitem.h"
#include <QVector>
#include <QDropEvent>
#include <QUrl>
#include <QtDebug>
#include <QMimeData>
#include <QFileDialog>

namespace Ui {
class QMainWidget;
}

class QMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QMainWidget(QWidget *parent = 0);
    ~QMainWidget();

    QVector <TheWidgetItem*> itemVec;
    void addItem(QString filename);
    void addOutputItem(QString foldername);

private slots:
    void on_addBtn_clicked();
    void on_delBtn_clicked();
    void on_addBtnOutput_clicked();
    void on_delBtnOutput_clicked();

    void on_startCopyBtn_clicked();
    void on_stopCopyBtn_clicked();

private:
    Ui::QMainWidget *ui;
};

#endif // QMAINWIDGET_H
