#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QDir>
#include <QFile>
#include <QElapsedTimer>
#include <fstream>
#include "thewidgetitem.h"

//void func(QList<int> list) // func will use a copy of list
//void func(QList<int>& list) // func will use the same instance of list
//void func(QList<int> const & list) // func will use the same instance of list and will not* modify it
//void func(QList<int>* list) // func will take a pointer to a QList<int> (that may be valid or not...)

class CopyThread : public QThread {
    Q_OBJECT

public:
    QList<TheWidgetItem*> inputFileWidgets; // contain both progress bar and label (progressBar->format())
    TheWidgetItem* outputFolderWidget; // one per thread

    CopyThread(QList<TheWidgetItem*> inputFileWidgets_, TheWidgetItem* outputFolderWidget_);

    void run();

    signals:
    void updateInputBar(quint32 delta, float speed);
    void updateOutputBar(quint32 delta, float speed);
};

#endif // COPYTHREAD_H
