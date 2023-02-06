#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(void);

private:
    QRect centreRect(QSize);
    QSize scaledImage(QSize, int);
    void showImage(QString);
private:
    void dragEnterEvent(QDragEnterEvent *) override;
    void dropEvent(QDropEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void wheelEvent(QWheelEvent *) override;

private:
    Ui::MainWindow *ui;
    QPoint m_movePoint;

private:
    class ScaledRange
    {
    private:
        const int maxScaled = 1500;
        const int minScaled = 10;
        int currentScaled = 100;

    public:
        ScaledRange& operator =(int scaled)
        {
            if(scaled > maxScaled) {
                scaled = maxScaled;
            }
            if(scaled < minScaled) {
                scaled = minScaled;
            }
            currentScaled = scaled;
            return *this;
        }
        ScaledRange& operator +=(int scaled)
        {
            currentScaled += scaled;
            *this = currentScaled;
            return *this;
        }
        ScaledRange& operator -=(int scaled)
        {
            currentScaled -= scaled;
            *this = currentScaled;
            return *this;
        }
        operator int() const
        {
            return currentScaled;
        }
    };

    ScaledRange m_scaledRange;
};
#endif // MAINWINDOW_H
