#ifndef LINKER_H
#define LINKER_H

#include <QApplication>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QColor>
#include <vector>
#include <unistd.h>
#include "restrict.h"

#define WHITE QColor(255, 255, 255)
#define GREEN QColor(0, 200, 30)
#define BLUE QColor(0, 50, 150)
#define RED QColor(250, 50, 0)
#define ORANGE QColor(255, 150, 0)
#define APPLE QColor(160, 230, 0)

class Window ;

class Linker {
public:
    Linker (Window *, int *, int, QGraphicsScene *, int) ;
    ~Linker () ;
    int read (int) ;
    void write (int) ;
    void done (int) ;
    int get (int) ;
    bool cmp_ii (int, int) ;
    bool cmp_ei (int, int) ;
    bool cmp_ie (int, int) ;
    bool cmp_ee (int, int) ;
    void set (int, int) ;
    void flip (int, int) ;
    void solved (int) ;
    void tmpsolve (int) ;
    int getSize () ;
    void decr (int) ;
    void incr (int) ;
#ifndef LIMITED
    void setBinHeapLv (int) ;
    void setTerHeapLv (int) ;
#endif
    void flip_ifn_cmp (int, int) ;
private:
    void render (int, QColor) ;
#ifndef LIMITED
    void setHeapLv (int, int) ;
#endif

    QGraphicsScene * m_scene ;
    std::vector<QGraphicsRectItem *> m_rects ;
    std::vector<QGraphicsLineItem *> m_lines ;
    int * m_array ;
    int m_size ;
    Window * m_parent ;
    int m_writingIdx ;
    int m_readingIdx ;
    int m_width ;
    int m_delay ;
};

#endif // LINKER_H
