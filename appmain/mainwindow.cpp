#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow(void)
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    if(event->mimeData()->hasUrls()) {
        const auto&& fileList = event->mimeData()->urls();
        for(const auto& file : fileList) {
            if(file.isLocalFile()) {
                QString fileName = file.toLocalFile();
                QFileInfo info(fileName);
                if(info.isFile()) {
                    qDebug() << fileName;
                    if(info.suffix() == "png") {
                        ui->label_preview->setPixmap(QPixmap(fileName));
                    }
                }
            }
        }
    }
}

