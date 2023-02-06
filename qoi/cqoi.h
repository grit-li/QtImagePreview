#ifndef CQOI_H
#define CQOI_H

#include <QtCore/qglobal.h>
#include <QPixmap>
#include <QImage>
#include <QString>

#include "qoi.h"

#if defined(QOI_LIBRARY)
#  define QOI_EXPORT Q_DECL_EXPORT
#else
#  define QOI_EXPORT Q_DECL_IMPORT
#endif

class QOI_EXPORT CQoi
{
public:
    CQoi(const QString&);
    virtual ~CQoi(void);

public:
    QPixmap toPixmap(void) const;
    QImage toImage(void) const;

public:
    static inline QPixmap toPixmap(const QString&);

private:
    qoi_desc m_qoiDesc;
    void* m_pixels;
};

inline QPixmap CQoi::toPixmap(const QString& fileName)
{
    return CQoi(fileName).toPixmap();
}

#endif // CQOI_H
