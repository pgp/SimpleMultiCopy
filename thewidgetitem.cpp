#include "thewidgetitem.h"
#include "ui_thewidgetitem.h"

TheWidgetItem::TheWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TheWidgetItem)
{
    ui->setupUi(this);
}

TheWidgetItem::~TheWidgetItem()
{
    delete ui;
}

void TheWidgetItem::setFileName(QString fileName_)
{
    filename = fileName_;
    ui->progressBar->setTextVisible(true);
    ui->progressBar->setFormat(filename);
}

void TheWidgetItem::setTotalBytes(quint64 totalBytes_) {
    totalBytes = totalBytes_;
}

void TheWidgetItem::setFileSize(quint64 fileSize_) {
    fileSize = fileSize_;
}

void TheWidgetItem::on_pressThisBtn_clicked()
{
//    // DEBUG
//    progress = 0;
//    int i;
//    for (i=0;i<=100;i++) {
//        progress = ui->progressBar->value();
//        ui->progressBar->setValue(progress++);
//        std::this_thread::sleep_for(std::chrono::milliseconds(30));
//    }
}

void TheWidgetItem::updateByteProgress(quint32 delta,float speed) {
    byteProgress += delta;
    ui->progressBar->setValue((byteProgress*100.0)/totalBytes); // percentage truncated as integer
    ui->speedLabel->setText(QString::number(speed)+" Mb/sec");
}
