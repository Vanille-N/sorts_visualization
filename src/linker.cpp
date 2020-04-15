#include "linker.h"
#include "window.h"
#include <stdlib.h>

Linker::Linker (Window * parent, item * array, int size, QGraphicsScene * scene, int delay) {
    m_mem = array ;
    m_size = size ;
    m_scene = scene ;
    m_delay = delay ;
    m_parent = parent ;
    m_width = std::max(512 / m_size, 1) ;
    m_array = new item * [m_size] ;
    for (int i = 0; i < m_size; i++) {
        m_array[i] = &m_mem[i] ;
    }
    for (int i = 0; i < m_size; i++) {
        m_array[i]->place = i+1 ;
        int height = (m_array[i]->val + 1) * m_width ;
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
            m_rects[i]->setToolTip(Tip(m_mem[i])) ;
        } else {
            QPen pen = QPen(ORANGE, 1, Qt::SolidLine) ;
            m_lines.push_back(m_scene->addLine(left, up, left, 540, pen)) ;
            m_lines[i]->setToolTip(Tip(m_mem[i])) ;
        }
//        printf("%d\n", i) ;
    }
}

void Linker::delay () {
    for (int i = 0; i <= m_delay/6; i++) {
        QApplication::processEvents() ;
    }
}

void Linker::render (int i, QColor C) {
    int height = (m_array[i]->val + 1) * m_width ;
    int left = 10 + i * m_width ;
    int up = 540 - height ;
    if (m_size <= 256) {
        QPen pen = QPen (Qt::black) ;
        QBrush brush = QBrush (C) ;
        m_scene->removeItem(m_rects[i]) ;
        m_rects[i] = m_scene->addRect(left, up, m_width, height, pen, brush) ;
        m_rects[i]->setToolTip(Tip(*m_array[i])) ;
    } else {
        QPen pen = QPen (C, 1, Qt::SolidLine) ;
        m_scene->removeItem(m_lines[i]) ;
        m_lines[i] = m_scene->addLine(left, up, left, 540, pen) ;
        m_lines[i]->setToolTip(Tip(*m_array[i])) ;
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

item Linker::get (int i) {
    render(i, RED) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->read++ ;
    m_parent->INCRREAD() ;
    render(i, ORANGE) ;
    return *m_array[i] ;
}

item * Linker::send (int i) {
    render(i, RED) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->read++ ;
    m_array[i]->swap++ ;
    m_parent->INCRREAD() ;
    render(i, ORANGE) ;
    return m_array[i] ;
}

bool Linker::cmp_ii (int i, int j) {
    render(i, BLUE) ;
    render(j, BLUE) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->cmp++ ;
    m_array[j]->cmp++ ;
    m_parent->INCRCMP() ;
    m_array[i]->read++ ;
    m_array[j]->read++ ;
    m_parent->INCRREAD() ;
    m_parent->INCRREAD() ;
    render(i, ORANGE) ;
    render(j, ORANGE) ;
    return (*m_array[i] < *m_array[j]) ;
}

bool Linker::cmp_ee (item * a, item * b) {
    a->read++ ;
    b->read++ ;
    a->cmp++ ;
    b->cmp++ ;
    m_parent->INCRCMP() ;
    return (*a < *b) ;
}

bool Linker::cmp_ie (int i, item * a) {
    render(i, BLUE) ;
    usleep(m_delay) ;
//    QApplication::processEvents() ;
//    m_array[i]->read++ ;
    delay() ;
    m_parent->INCRREAD() ;
    a->read++ ;
    m_array[i]->cmp++ ;
    a->cmp++ ;
    m_parent->INCRCMP() ;
    render(i, ORANGE) ;
    return (*m_array[i] < *a) ;
}

bool Linker::cmp_ei (item * a, int i) {
    render(i, BLUE) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->read++ ;
    m_parent->INCRREAD() ;
    a->read++ ;
    m_array[i]->cmp++ ;
    a->cmp++ ;
    m_parent->INCRCMP() ;
    render(i, ORANGE) ;
    return (*a < *m_array[i]) ;
}

void Linker::set (int i, item val) {
    *m_array[i] = val ;
    render(i, PURPLE) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->swap++ ;
    m_parent->INCRWRITE() ;
    render(i, ORANGE) ;
}

void Linker::receive (int i, item * val) {
    m_array[i] = val ;
    render(i, PURPLE) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->swap++ ;
    m_parent->INCRWRITE() ;
    render(i, ORANGE) ;
}

void Linker::flip (int i, int j) {
    item * tmp = m_array[i] ;
    m_array[i] = m_array[j] ;
    m_array[j] = tmp ;
    m_parent->INCRREAD() ;
    m_parent->INCRREAD() ;
    render(i, PURPLE) ;
    render(j, PURPLE) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    m_array[i]->swap++ ;
    m_array[j]->swap++ ;
    m_parent->INCRWRITE() ;
    m_parent->INCRWRITE() ;
    render(i, ORANGE) ;
    render(j, ORANGE) ;
}

void Linker::solved (int i) {
    m_array[i]->rank = i+1 ;
    render(i, GREEN) ;
    QApplication::processEvents() ;
}

void Linker::tmpsolve (int i) {
    render(i, APPLE) ;
    QApplication::processEvents() ;
}

//void Linker::incr (int i) {
//    m_array[i]++ ;
//    render(i, RED) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
//    render(i, ORANGE) ;
//    m_parent->INCRWRITE() ;
//    m_parent->INCRREAD() ;
//}

//void Linker::decr (int i) {
//    m_array[i]-- ;
//    render(i, RED) ;
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
//    render(i, ORANGE) ;
//    m_parent->INCRWRITE() ;
//    m_parent->INCRREAD() ;
//}

int Linker::getSize () { return m_size ; }

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

void Linker::flip_ifn_cmp (int i, int j) {
    render(i, BLUE) ;
    render(j, BLUE) ;
    m_array[i]->cmp++ ;
    m_array[j]->cmp++ ;
    m_parent->INCRREAD() ;
    m_parent->INCRREAD() ;
    m_parent->INCRCMP() ;
    if (*m_array[i] > *m_array[j]) {
        item * tmp = m_array[i] ;
        m_array[i] = m_array[j] ;
        m_array[j] = tmp ;
        m_array[i]->swap++ ;
        m_array[j]->swap++ ;
        m_parent->INCRWRITE() ;
        m_parent->INCRWRITE() ;
    }
//    usleep(m_delay) ;
//    QApplication::processEvents() ;
    delay() ;
    render(i, ORANGE) ;
    render(j, ORANGE) ;
}