#include "dragdroplistwidget.h"

DragDropListWidget::DragDropListWidget(QWidget* parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

// whichList: true for input, false for output
void DragDropListWidget::setMainWidget(QMainWidget* refMainWidget_, bool whichList_) {
    refMainWidget = refMainWidget_;
    whichList = whichList_;
}

void DragDropListWidget::dropEvent(QDropEvent *ev) {
    QList<QUrl> urls = ev->mimeData()->urls();
    if (whichList) {
        foreach(QUrl url, urls)
        {
            refMainWidget->addItem(url.toLocalFile());
            //        qDebug()<<url.toString();
        }
    }
    else {
        foreach(QUrl url, urls)
        {
            refMainWidget->addOutputItem(url.toLocalFile());
        }
    }
}

void DragDropListWidget::dragEnterEvent(QDragEnterEvent *ev) {
//    ev->accept();
    ev->acceptProposedAction();
}
