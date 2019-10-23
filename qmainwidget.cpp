#include "qmainwidget.h"
#include "ui_qmainwidget.h"
#include "copythread.h"

QMainWidget::QMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMainWidget)
{
    ui->setupUi(this);
    ui->listWidget->setMainWidget(this,true);
    ui->listWidget_2->setMainWidget(this,false);
}

QMainWidget::~QMainWidget()
{
    delete ui;
}

void QMainWidget::addItem(QString filename) {
    qint64 fileSize;
    QFileInfo *fi = new QFileInfo(filename);
    if (fi->isDir()) return; // accept only files
    fileSize = fi->size();
    //    fileSize = getFileSize(filename);

    //Creating a new list widget item whose parent is the listwidget itself
    QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listWidget);

    //Adding the item to the listwidget
    ui->listWidget->addItem (listWidgetItem);

    //Creating an object of the designed widget which is to be added to the listwidget
    TheWidgetItem *theWidgetItem = new TheWidgetItem;
    theWidgetItem->setFileName(filename);
    theWidgetItem->setFileSize(fileSize);

    //Making sure that the listWidgetItem has the same size as the TheWidgetItem
    listWidgetItem->setSizeHint (theWidgetItem->sizeHint());

    //Finally adding the itemWidget to the list
    ui->listWidget->setItemWidget (listWidgetItem, theWidgetItem);

    // update this widget with total output size (this file's size * number of output widgets)
    theWidgetItem->setTotalBytes(fileSize*(ui->listWidget_2->count()));

    // loop over output widgets
    // recompute total file size by adding current file size to current total,
    // then update it in all totalBytes fields of the output folder widgets
    for(int i = 0; i < ui->listWidget_2->count(); ++i) {
        QListWidgetItem* item = ui->listWidget_2->item(i);
        TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget_2->itemWidget(item));

        theItem->totalBytes += fileSize;
    }
}

void QMainWidget::on_addBtn_clicked()
{
    QString filename =  QFileDialog::getOpenFileName(
              Q_NULLPTR,
              "Open File",
              QDir::currentPath(),
              "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");
    if (filename == NULL) return; // error opening file
    addItem(filename);
}

void QMainWidget::on_delBtn_clicked()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget->itemWidget(item));
    qint64 fileSize = theItem->fileSize;

    for(int i = 0; i < ui->listWidget_2->count(); ++i) {
        QListWidgetItem* item = ui->listWidget_2->item(i);
        TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget_2->itemWidget(item));

        theItem->totalBytes -= fileSize;
    }

    //Delete selected item from the listWidget
    delete ui->listWidget->currentItem();
}

void QMainWidget::addOutputItem(QString foldername) {
    QFileInfo *fi = new QFileInfo(foldername);
    if (!(fi->isDir())) return; // accept only folders

    //Creating a new list widget item whose parent is the listwidget itself
    QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listWidget_2);

    //Adding the item to the listwidget
    ui->listWidget_2->addItem (listWidgetItem);

    //Creating an object of the designed widget which is to be added to the listwidget
    TheWidgetItem *theWidgetItem = new TheWidgetItem;
    theWidgetItem->setFileName(foldername);

    //Making sure that the listWidgetItem has the same size as the TheWidgetItem
    listWidgetItem->setSizeHint (theWidgetItem->sizeHint());

    //Finally adding the itemWidget to the list
    ui->listWidget_2->setItemWidget (listWidgetItem, theWidgetItem);

    // loop over input file widgets
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget->itemWidget(item));

        theWidgetItem->totalBytes += theItem->fileSize; // perform sum over all input files and store as total size for output folder widget

        theItem->totalBytes += theItem->fileSize; // add this file size (total size = file size * output folders)
    }
}

void QMainWidget::on_addBtnOutput_clicked()
{
    QString foldername = QFileDialog::getExistingDirectory(
                Q_NULLPTR,
                tr("Open Directory"),
                QDir::currentPath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (foldername == NULL) return; // error opening folder
    addOutputItem(foldername);
}

void QMainWidget::on_delBtnOutput_clicked() {
    // loop over input file widgets
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget->itemWidget(item));

        theItem->totalBytes -= theItem->fileSize; // add this file size (total size = file size * output folders)
    }

    delete ui->listWidget_2->currentItem();
}

void QMainWidget::on_startCopyBtn_clicked() {
    /*
     * - get the two lists of input filenames (N) and output folders (M)
     * - create M threads (one per folder) after checking folders are all on different physical devices
     * - each thread receives the list of input files and the output folder,
     *   and updates the corresponding progress bars (percentage for an input file per thread is percentage/M
     *   (progress bar setValue is incremental - need atomicity for setValue)
     */

    // accumulate list of input widgets
    QList<TheWidgetItem*> inputs;
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget->itemWidget(item));
        inputs.append(theItem);
    }

    for(int i = 0; i < ui->listWidget_2->count(); ++i)
    {
        // 1 thread per output folder
        QListWidgetItem* item = ui->listWidget_2->item(i);
        TheWidgetItem* theItem = qobject_cast<TheWidgetItem*>(ui->listWidget_2->itemWidget(item));

        CopyThread* thread = new CopyThread(inputs,theItem);
        thread->start();
//        qDebug()<<"Thread started\n";
    }
}

void QMainWidget::on_stopCopyBtn_clicked() {
    // TODO stop all threads performing copy
}

//void QMainWidget::dropEvent(QDropEvent *ev)
//{
//   QList<QUrl> urls = ev->mimeData()->urls();
//   foreach(QUrl url, urls)
//   {
//       qDebug()<<url.toString();
//   }
//}

//void QMainWidget::dragEnterEvent(QDragEnterEvent *ev)
//{
//   ev->accept();
//}
