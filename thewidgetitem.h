#ifndef THEWIDGETITEM_H
#define THEWIDGETITEM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QProgressBar>
#include <QLabel>

namespace Ui {
class TheWidgetItem;
}

class TheWidgetItem : public QWidget
{
    Q_OBJECT

public:
    QString filename;
    int progress = 0;
    quint64 byteProgress = 0;
    quint64 totalBytes = 0; // for input file widgets, updated when an output folder is added/removed (+= or -= file size)
    // for output folder widgets, updated when an input file is added/removed (sum of file sizes)

    qint64 fileSize; // only used for input files, needed to update total size upon item removal

    explicit TheWidgetItem(QWidget *parent = 0);
    ~TheWidgetItem();
    void setFileName(QString fileName_);
    void setTotalBytes(quint64 totalBytes_);
    void setFileSize(quint64 fileSize_);
private slots:
    void on_pressThisBtn_clicked();
//    void on_horizontalSlider_valueChanged(int value);
//    void updateProgress(int delta);
    void updateByteProgress(quint32 delta, float speed);

private:
    Ui::TheWidgetItem *ui;
};

#endif // THEWIDGETITEM_H
