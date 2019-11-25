#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>

class Drawer {
public:
    Drawer (QGraphicsScene) ;
    void draw (int, int, int, int) ;
private:
    QGraphicsScene * m_scene ;
};

#endif // DRAWER_H
