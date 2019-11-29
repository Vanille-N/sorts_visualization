#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include <experimental/random>
#include <algorithm>
#include <random>
#include "restrict.h"
#include "item.h"

class Distributor {
public:
    Distributor(int size, item * array) ;
    void flip (int, int) ;
    void Dregular () ;
    void Duniform () ;
#ifndef LIMITED
    void Dcentered () ;
    void Dsplit () ;
    void Dhigh () ;
    void Dlow () ;
#endif
    void Srandom () ;
    void Ssorted () ;
#ifndef LIMITED
    void Sreversed () ;
    void Sshuffle () ;
#endif
private:
    int m_size ;
    item * m_array ;
};

#endif // DISTRIBUTOR_H
