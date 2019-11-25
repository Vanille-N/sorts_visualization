#include "distributor.h"

Distributor::Distributor (int size, int * array) {
    m_size = size ;
    m_array = array ;
}

void Distributor::flip (int i, int j) {
    int tmp = m_array[i] ;
    m_array[i] = m_array[j] ;
    m_array[j] = tmp ;
}

// DISTRIBUTIONS

void Distributor::Dregular () {
    if (m_size <= 512) {
        for (int i = 0; i < m_size; i++) {
            m_array[i] = i ;
        }
    } else {
        for (int i = 0; i < m_size; i++) {
            m_array[i] = i/2 ;
        }
    }
}

void Distributor::Duniform () {
    for (int i = 0; i < m_size; i++) {
        m_array[i] = std::experimental::randint(0, std::min(m_size, 512)-1) ;
    }
    std::sort(m_array, m_array + m_size) ;
}

#ifndef LIMITED
void Distributor::Dcentered () {
    std::default_random_engine generator ;
    std::normal_distribution<double> distribution(std::min(m_size, 512)/2.0, std::min(m_size, 512)/5.0) ;

    for (int i = 0; i < m_size;) {
        double number = distribution(generator) ;
        if ((number >= 0.0)&&(number < std::min(m_size, 512))) { m_array[i++] = int(number) ; }
    }
    std::sort(m_array, m_array + m_size) ;
}

void Distributor::Dsplit () {
    Dcentered() ;
    for (int i = 0; i < m_size; i++) {
        if (m_array[i] >= std::min(m_size, 512)/2) {
            m_array[i] = m_array[i] - std::min(m_size, 512)/2 ;
        } else {
            m_array[i] = m_array[i] + std::min(m_size, 512)/2 ;
        }
    }
    std::sort(m_array, m_array + m_size) ;
}

void Distributor::Dhigh () {
    std::default_random_engine generator ;
    std::normal_distribution<double> distribution(std::min(m_size, 512)/1.0, std::min(m_size, 512)/3.0) ;

    for (int i = 0; i < m_size;) {
        double number = distribution(generator) ;
        if ((number >= 0.0)&&(number < std::min(m_size, 512))) { m_array[i++] = int(number) ; }
    }
    std::sort(m_array, m_array + m_size) ;
}

void Distributor::Dlow () {
    std::default_random_engine generator ;
    std::normal_distribution<double> distribution(0.0, std::min(m_size, 512)/3.0) ;

    for (int i = 0; i < m_size;) {
        double number = distribution(generator) ;
        if ((number >= 0.0)&&(number < std::min(m_size, 512))) { m_array[i++] = int(number) ; }
    }
    std::sort(m_array, m_array + m_size) ;
}
#endif

// SCRAMBLES

void Distributor::Srandom () {
    for (int i = 0; i < m_size; i++) {
        flip(i, std::experimental::randint(i, m_size-1)) ;
    }
}

void Distributor::Ssorted () {
    int lg = 0 ;
    for (int i = 1; i < m_size; i<<=1) { lg++ ; }
    for (int i = 0; i < m_size; i++) {
        flip(i, std::experimental::randint(std::max(0, i-lg), std::min(m_size-1, i+lg))) ;
    }
}

#ifndef LIMITED
void Distributor::Sreversed () {
    for (int i = 0; i < m_size/2; i++) {
        flip(i, m_size-1-i) ;
    }
    Ssorted() ;
}

void Distributor::Sshuffle () {
    Ssorted() ;
    for (int i = 0; i < m_size/2; i+=2) {
        flip(i, m_size-1-i) ;
    }
}
#endif
