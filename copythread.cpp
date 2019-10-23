#include "copythread.h"

CopyThread::CopyThread(QList<TheWidgetItem*> inputFileWidgets_, TheWidgetItem* outputFolderWidget_)
{
    inputFileWidgets = inputFileWidgets_;
    outputFolderWidget = outputFolderWidget_;
}

// See here for more copy methods:
// http://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way

void CopyThread::run() {
    // connect to output slot
    QObject::connect(this, SIGNAL(updateOutputBar(quint32,float)),
                         outputFolderWidget, SLOT(updateByteProgress(quint32,float)));

    // for each input file:
    //   connect to input and output widgets
    //   copy file, every X Mb emit signal for update bars
    //   disconnect widgets

    QString outputFoldername = outputFolderWidget->filename;

    for (TheWidgetItem* inputFileWidget : inputFileWidgets) {
        // connect to input slots
        QObject::connect(this, SIGNAL(updateInputBar(quint32,float)),
                             inputFileWidget, SLOT(updateByteProgress(quint32,float)));

//        QString inputFilename = inputFileWidget->filename;
        // copy inputFilename to outputFoldername
        // emit signals during copy

        // here the actual file copy is performed
        /////////////////////**********************////////////////////////

        // get filename only (to be concatenated to output folder)
        QFileInfo inFile(inputFileWidget->filename);
        QString inputNameOnly(inFile.fileName());

        QString outPath = QDir(outputFoldername).filePath(inputNameOnly);


        // only one progress step per file/folder pair
//        std::ifstream srce( inputFileWidget->filename.toLatin1().data(), std::ios::binary);
//        std::ofstream dest( outPath.toLatin1().data(), std::ios::binary);
//        dest << srce.rdbuf();

//        emit updateInputBar(inputFileWidget->fileSize);
//        emit updateOutputBar(inputFileWidget->fileSize);

        quint32 BUF_LEN = 32*1048576;
        qint32 readBytes;
        char *buffer = new char[BUF_LEN];
        std::ifstream srce(inputFileWidget->filename.toUtf8().constData(), std::ios::binary);
        std::ofstream dest(outPath.toUtf8().constData(), std::ios::binary);

        QElapsedTimer timer; // update only progress bars for output (easier)
        timer.start();

        for(;;)
        {
            srce.read(buffer, BUF_LEN);
            readBytes = srce.gcount();
            if (readBytes <= 0) break;
            dest.write(buffer, readBytes);
            qint64 nanoSec = timer.nsecsElapsed();
            timer.restart();

            emit updateInputBar(readBytes,0.0);
            emit updateOutputBar(readBytes,readBytes*1000.0/nanoSec); // (bytes/1000000 aka Mb) / (nanoSec/1e9 aka seconds)
        }

        srce.close();
        dest.close();

        delete buffer;

        /////////////////////**********************////////////////////////

        QObject::disconnect(this, SIGNAL(updateInputBar(quint32,float)),
                            inputFileWidget, SLOT(updateByteProgress(quint32,float)));
    }
}
