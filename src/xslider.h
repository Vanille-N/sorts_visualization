#ifndef XSLIDER_H
#define XSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QVector>
#include <QLabel>
#include <QHBoxLayout>
#include <QString>

class XSlider : public QWidget {
    Q_OBJECT
public:
    XSlider (QVector<int>, QVector<int>) ;
    int getval () ;
public slots:
    void updateDisp () ;
private:
    QVector<int> m_val ;
    QVector<int> m_mul ;
    QSlider * m_slideVal ;
    QSlider * m_slideMul ;
    QLabel * m_disp ;
};

#endif // XSLIDER_H
