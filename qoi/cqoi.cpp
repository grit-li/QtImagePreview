#include "cqoi.h"
#include <QDebug>

CQoi::CQoi(const QString& fileName)
{
    m_pixels = qoi_read(fileName.toStdString().c_str(), &m_qoiDesc, 0);
}

CQoi::~CQoi(void)
{
    if(m_pixels) {
        free(m_pixels);
        m_pixels = NULL;
    }
}

static inline QImage updateImage(const qoi_desc* desc, void* src)
{
    QImage::Format format = QImage::Format_RGBA8888;
    if(desc->channels == 3) {
        format = QImage::Format_RGB888;
    }
    unsigned char* p = static_cast<unsigned char *>(src);
    QImage image(desc->width, desc->height, format);
    unsigned int index = 0;
    for(unsigned int h = 0; h < desc->height; h++) {
        for(unsigned int w = 0; w < desc->width; w++) {
            index = h * desc->width + w;
            index *= desc->channels;
            if(desc->channels == 3) {
                image.setPixel(w, h, qRgb(p[index + 0], p[index + 1], p[index + 2]));
            } else if(desc->channels == 4) {
                image.setPixel(w, h, qRgba(p[index + 0], p[index + 1], p[index + 2], p[index + 3]));
            }
        }
    }
    return image;
}


QPixmap CQoi::toPixmap(void) const
{
    return QPixmap::fromImage(toImage());
}

QImage CQoi::toImage(void) const
{
    return m_pixels ? updateImage(&m_qoiDesc, m_pixels) : QImage();
}

