#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QLCDNumber>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QVector>
#include "optionmgr.h"
#include "distributor.h"
#include "linker.h"
#include "sorter.h"
#include "xslider.h"
#include "item.h"

class Window : public QWidget {
    Q_OBJECT
public:
    Window (QWidget * parent, QGraphicsScene *, QGraphicsView *) ;
    void INCRCMP () ;
    void INCRREAD () ;
    void INCRWRITE () ;
public slots:
    void run () ;
    void done () ;
    void Resize (int) ;
    void kill () ;
    void abort () ;
    void play () ;
    void pause () ;
private:
    QWidget * m_parent ;
    QGraphicsScene * m_scene ;
    QGraphicsView * m_view ;
    QComboBox * m_chooseAlgo ;
    QComboBox * m_chooseOption ;
    OptionMgr * m_optmgr ;
    XSlider * m_chooseDelay ;
    QComboBox * m_chooseDistrib ;
    QComboBox * m_chooseScramble ;
    QComboBox * m_chooseSize ;
    QLCDNumber * m_nbCmp ;
    QLCDNumber * m_nbRead ;
    QLCDNumber * m_nbWrite ;
    QPushButton * m_start ;
    QPushButton * m_exit ;
    QPushButton * m_abort ;
    QPushButton * m_playpause ;
    QVBoxLayout * m_chooseMenu ;
    Linker * m_link ;
    Sorter * m_sort ;
    item * m_array ;
    bool playing ;
};

#endif // WINDOW_H
