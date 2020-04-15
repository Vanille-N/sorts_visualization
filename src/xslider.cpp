#include "xslider.h"

XSlider::XSlider (QVector<int> val, QVector<int> mul) {
    setFixedHeight(70) ;
    for (int i = 0; i < val.size(); i++) {
        m_val.push_back(val[i]) ;
    }
    for (int i = 0; i < mul.size(); i++) {
        m_mul.push_back(mul[i]) ;
    }
    m_slideVal = new QSlider (Qt::Horizontal) ;
    m_slideVal->setMaximum(val.size()-1) ;
    m_slideMul = new QSlider (Qt::Horizontal) ;
    m_slideMul->setMaximum(mul.size()-1) ;
    m_disp = new QLabel("0 ms") ;
    connect(m_slideVal, SIGNAL(valueChanged(int)), this, SLOT(updateDisp())) ;
    connect(m_slideMul, SIGNAL(valueChanged(int)), this, SLOT(updateDisp())) ;
    auto vbox = new QVBoxLayout () ;
    vbox->addWidget(m_slideVal) ;
    vbox->addWidget(m_slideMul) ;
    vbox->addWidget(m_disp) ;
    setLayout(vbox) ;
    m_slideVal->setFixedWidth(100) ;
    m_slideMul->setFixedWidth(100) ;

}


int XSlider::getval () {
    return m_val[m_slideVal->value()]*m_mul[m_slideMul->value()] ;
}

void XSlider::updateDisp () {
    m_disp->setText(QString::number(getval()) + " ms") ;
}

