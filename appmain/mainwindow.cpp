#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QResizeEvent>
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

QRect MainWindow::centreRect(QSize size)
{
    QRect rc;
    QRect windowRc = geometry();
    rc.setX((windowRc.width() - size.width()) >> 1);
    rc.setY((windowRc.height() - size.height()) >> 1);
    rc.setWidth(size.width());
    rc.setHeight(size.height());
    return rc;
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
                        QPixmap pixmap(fileName);
                        resize(pixmap.size());
                        ui->label_preview->setGeometry(centreRect(pixmap.size()));
                        ui->label_preview->setPixmap(QPixmap(fileName));
                    }
                }
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    this->m_movePoint = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    QSize mainWindowSize = geometry().size();
    QSize imageSize = ui->label_preview->geometry().size();

    if(mainWindowSize.width() >= imageSize.width() && mainWindowSize.height() >= imageSize.height() && imageSize != QSize(0,0)) {
        return;
    }
    QPoint offsetPoint = event->pos() - m_movePoint;
    m_movePoint = event->pos();
    offsetPoint += QPoint(ui->label_preview->geometry().x(), ui->label_preview->geometry().y());
    ui->label_preview->move(offsetPoint);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QRect rc;
    QSize pixmapSize = ui->label_preview->size();
    rc.setX((event->size().width() - pixmapSize.width()) >> 1);
    rc.setY((event->size().height() - pixmapSize.height()) >> 1);
    rc.setWidth(pixmapSize.width());
    rc.setHeight(pixmapSize.height());
    ui->label_preview->setGeometry(rc);
}

