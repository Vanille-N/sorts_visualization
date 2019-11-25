#include "linker.h"
#include "window.h"
#include <stdlib.h>

Linker::Linker (Window * parent, int * array, int size, QGraphicsScene * scene, int delay) {
    m_array = array ;
    m_size = size ;
    m_scene = scene ;
    m_delay = delay ;
    m_parent = parent ;
    m_width = std::max(512 / m_size, 1) ;
    for (int i = 0; i < m_size; i++) {
        int height = (m_array[i] + 1) * m_width ;
        float left = 10 + i * m_width ;
        int intleft = std::floor(left) ;
        if (left - intleft < 0.1) {
            left = intleft ;
        } else if (left - intleft < 0.6) {
            left = intleft + 0.5 ;
        } else {
            left = intleft + 1 ;
        }
        int up = 540 - height ;
        // m_rects.push_back(m_scene->addRect(left, up, (m_size<512)?m_width:.5, height, pen, brush)) ;
        if (m_size <= 256) {
            QPen pen = QPen (Qt::black) ;
            QBrush brush = QBrush (ORANGE) ;
            m_rects.push_back(m_scene->addRect(left, up, m_width, height , pen, brush)) ;
        } else {
            QPen pen = QPen(ORANGE, 1, Qt::SolidLine) ;
            m_lines.push_back(m_scene->addLine(left, up, left, 540, pen)) ;
        }
//        printf("%d\n", i) ;
    }
}

void Linker::render (int idx, QColor C) {
    int height = (m_array[idx] + 1) * m_width ;
    int left = 10 + idx * m_width ;
    int up = 540 - height ;
    if (m_size <= 256) {
        QPen pen = QPen (Qt::black) ;
        QBrush brush = QBrush (C) ;
        m_scene->removeItem(m_rects[idx]) ;
        m_rects[idx] = m_scene->addRect(left, up, m_width, height, pen, brush) ;
    } else {
        QPen pen = QPen (C, 1, Qt::SolidLine) ;
        m_scene->removeItem(m_lines[idx]) ;
        m_lines[idx] = m_scene->addLine(left, up, left, 540, pen) ;
    }

}

Linker::~Linker () {
    if (m_size <= 256) {
        for (int i = 0; i < m_size; i++) {
            m_scene->removeItem(m_rects[i]) ;
        }
    } else {
        for (int i = 0; i < m_size; i++) {
            m_scene->removeItem(m_lines[i]) ;
        }
    }
}

int Linker::get (int idx) {
    render(idx, RED) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(idx, ORANGE) ;
    m_parent->INCRREAD() ;
    return m_array[idx] ;
}

bool Linker::cmp_ii (int i, int j) {
    render(i, BLUE) ;
    render(j, BLUE) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    render(j, ORANGE) ;
    m_parent->INCRCMP() ;
    m_parent->INCRREAD() ;
    m_parent->INCRREAD() ;
    return (m_array[i] < m_array[j]) ;
}

bool Linker::cmp_ee (int i, int j) {
    m_parent->INCRCMP() ;
    return (i < j) ;
}

bool Linker::cmp_ie (int i, int j) {
    render(i, BLUE) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    m_parent->INCRCMP() ;
    m_parent->INCRREAD() ;
    return (m_array[i] < j) ;
}

bool Linker::cmp_ei (int i, int j) {
    render(j, BLUE) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(j, ORANGE) ;
    m_parent->INCRCMP() ;
    m_parent->INCRREAD() ;
    return (i < m_array[j]) ;
}

void Linker::set (int i, int val) {
    m_array[i] = val ;
    render(i, BLUE) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    m_parent->INCRWRITE() ;
}

void Linker::flip (int i, int j) {
    int tmp = m_array[i] ;
    m_array[i] = m_array[j] ;
    m_array[j] = tmp ;
    render(i, BLUE) ;
    render(j, BLUE) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    render(j, ORANGE) ;
    m_parent->INCRREAD() ;
    m_parent->INCRREAD() ;
    m_parent->INCRWRITE() ;
    m_parent->INCRWRITE() ;

}

void Linker::solved (int i) {
    render(i, GREEN) ;
    QApplication::processEvents() ;
}

void Linker::tmpsolve (int i) {
    render(i, APPLE) ;
    QApplication::processEvents() ;
}

void Linker::incr (int i) {
    m_array[i]++ ;
    render(i, RED) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    m_parent->INCRWRITE() ;
    m_parent->INCRREAD() ;
}

void Linker::decr (int i) {
    m_array[i]-- ;
    render(i, RED) ;
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    m_parent->INCRWRITE() ;
    m_parent->INCRREAD() ;
}
int Linker::getSize () { return m_size ; }

#ifndef LIMITED
void Linker::setBinHeapLv(int idx) {
    setHeapLv(idx, 2) ;
}

void Linker::setTerHeapLv(int idx) {
    setHeapLv(idx, 3) ;
}

void Linker::setHeapLv (int idx, int base) {
    int lv = std::floor(std::log(idx+1) / std::log(base) + 0.001) ;
    QColor c ;
    c.setHsv(lv*255/8, 100, 255) ;
    render(idx, c) ;
    QApplication::processEvents() ;
}
#endif

void Linker::flip_ifn_cmp (int i, int j) {
    render(i, BLUE) ;
    render(j, BLUE) ;
    if (m_array[i] > m_array[j]) {
        int tmp = m_array[i] ;
        m_array[i] = m_array[j] ;
        m_array[j] = tmp ;
    }
    usleep(m_delay) ;
    QApplication::processEvents() ;
    render(i, ORANGE) ;
    render(j, ORANGE) ;
    m_parent->INCRREAD() ;
    m_parent->INCRREAD() ;
    m_parent->INCRWRITE() ;
    m_parent->INCRWRITE() ;
    m_parent->INCRCMP() ;
}
