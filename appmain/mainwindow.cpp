#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cqoi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_movePoint(0, 0)
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

QSize MainWindow::scaledImage(QSize size, int scaled)
{
    size.setWidth((size.width() * scaled) / 100);
    size.setHeight((size.height() * scaled) / 100);
    return size;
}

static inline bool isTrueSuffix(QString suffix)
{
    bool ret = false;
    if(suffix == "png" || suffix == "bmp" || suffix == "jpg" || suffix == "jpeg" || suffix == "qoi") {
        ret = true;
    }
    return ret;
}
static inline QPixmap createPixmap(const QString& fileName, const QString& suffix)
{
    if(suffix == "qoi") {
        return CQoi::toPixmap(fileName);
    } else {
        return QPixmap(fileName);
    }
}
void MainWindow::showImage(QString fileName)
{
    QFileInfo info(fileName);
    if(info.isFile()) {
        if(isTrueSuffix(info.suffix())) {
            QPixmap pixmap = createPixmap(fileName, info.suffix());
            if(pixmap.isNull() == false) {
                resize(pixmap.size());
                ui->label_preview->setGeometry(centreRect(pixmap.size()));
                ui->label_preview->setPixmap(pixmap);
            }
        }
    }
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
                showImage(file.toLocalFile());
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

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if(!ui->label_preview->pixmap() || ui->label_preview->pixmap()->isNull() == true) {
        return;
    }
    if(event->delta() > 0) {
        m_scaledRange += 5;
    } else {
        m_scaledRange -= 5;
    }
    QSize size = scaledImage(ui->label_preview->pixmap()->size(), m_scaledRange);
    QSize mainWindowSize = geometry().size();
    ui->label_preview->resize(size);

    if(mainWindowSize.width() >= size.width() && mainWindowSize.height() >= size.height() && size != QSize(0,0)) {
        ui->label_preview->setGeometry(centreRect(size));
    }
}

