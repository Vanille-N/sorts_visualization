#pragma once

#include <QString>

struct item {
    int val ;
    int rank ;
    int place ;
    int swap ;
    int cmp ;
    int read ;
};

inline item Item (int v) {
    return item{v, -1, -1, 0, 0, 0} ;
}

inline bool operator< (const item & a, const item & b) {
    return a.val < b.val ;
}

inline bool operator> (const item & a, const item & b) {
    return a.val > b.val ;
}

inline bool operator== (const item & a, const item & b) {
    return a.val == b.val ;
}

inline QString Tip (item k) {
    QString s = QString("Item n°%1\nValue: %2\nRank: %3\nRead: %4\nCompared: %5\nMoved: %6") ;
    return s.arg(QString::number(k.place),
                 QString::number(k.val),
                 (k.rank == -1) ? "???" : QString::number(k.rank),
                 QString::number(k.read),
                 QString::number(k.cmp),
                 QString::number(k.swap)) ;
}

