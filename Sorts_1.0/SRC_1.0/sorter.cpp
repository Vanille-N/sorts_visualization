#include "sorter.h"

Sorter::Sorter(Linker * link) {
    m_link = link ;
    m_size = m_link->getSize() ;
}

// /////////// //
// BUBBLE-TYPE //
// /////////// //

void Sorter::Bubble_upwards () {
    for (int i = 0; i < m_size; i++) {
        bool sorted = true ;
        for (int j = m_size-1; j > i; j--) {
            if (m_link->cmp_ii(j, j-1)) {
                sorted = false ;
                m_link->flip(j, j-1) ;
            }
        }
        m_link->solved(i) ;
        if (sorted) {
            for (int j = i+1; j < m_size; j++) {
                m_link->solved(j) ;
            }
            break ;
        }
    }
}

void Sorter::Bubble_downwards() {
    for (int i = m_size; i > 0; i--) {
        bool sorted = true ;
        for (int j = 1; j < i; j++) {
            if (m_link->cmp_ii(j, j-1)) {
                sorted = false ;
                m_link->flip(j, j-1) ;
            }
        }
        m_link->solved(i-1) ;
        if (sorted) {
            for (int j = 0; j < i; j++) {
                m_link->solved(j) ;
            }
            break ;
        }
    }
}

void Sorter::Bubble_shaker () {
    int lt = 0, rt = m_size ;
    int upwards = true ;
    while (rt > lt) {
        if (upwards) {
            int k = lt + 1 ;
            for (int i = lt+1; i < rt; i++) {
                if (m_link->cmp_ii(i, i-1)) {
                    m_link->flip(i, i-1) ;
                    k = i ;
                }
            }
            do m_link->solved(--rt) ; while (rt > k) ;
            upwards = false ;
        } else {
            int k = rt - 1 ;
            for (int i = rt-1; i > lt; i--) {
                if (m_link->cmp_ii(i, i-1)) {
                    m_link->flip(i, i-1) ;
                    k = i ;
                }
            }
            do m_link->solved(lt++) ; while (lt < k) ;
            upwards = true ;
        }
    }
}

void Sorter::Comb () {
    float step = float(m_size / 1.3) ;
    for (;;) {
        int s = int(std::ceil(step)) ;
        bool sorted = (s == 1) ;
        for (int j = s; j < m_size; j++) {
            if (m_link->cmp_ii(j, j-s)) {
                sorted = false ;
                m_link->flip(j, j-s) ;
            }
        }
        if (sorted) {
            for (int j = 0; j < m_size; j++) {
                m_link->solved(j) ;
            }
            break ;
        }
        step /= float(1.4) ;
    }

}

#ifndef LIMITED
// ////////// //
// QUICK-TYPE //
// ////////// //

void Sorter::Quick_naive () {
    Quicksort(0, m_size, 0) ;
}

void Sorter::Quick_middle () {
    Quicksort(0, m_size, 1) ;
}

void Sorter::Quick_optimized () {
    Quicksort(0, m_size, 2) ;
}

void Sorter::Quicksort (int a, int b, int part) {
    if (b - a == 1) {
        m_link->solved(a) ;
        return ;
    } else if (b - a < 1) {
        return ;
    }
    int pv = 0 ;
    switch (part) {
        case 0: pv = Naive_partition(a, b) ; break ;
        case 1: pv = Middle_partition(a, b) ; break ;
        case 2: pv = Fast_partition(a, b) ; break ;
    }
    m_link->solved(pv) ;
    Quicksort(a, pv, part) ;
    Quicksort(pv+1, b, part) ;
}

int Sorter::Naive_partition (int a, int b) {
    int pvVal = m_link->get(a) ;
    int pvIdx = a ;
    int k = a + 1 ;
    while (k < b) {
        if (m_link->cmp_ie(k, pvVal)) {
            m_link->flip(pvIdx++, k) ;
            m_link->flip(pvIdx, k++) ;
        } else {
            k++ ;
        }
    }
    return pvIdx ;
}

int Sorter::Middle_partition (int a, int b) {
    int pvIdx = a ;
    m_link->flip(pvIdx, (b+a)/2) ;
    int pvVal = m_link->get(a) ;
    int k = a + 1 ;
    while (k < b) {
        if (m_link->cmp_ie(k, pvVal)) {
            m_link->flip(pvIdx++, k) ;
            m_link->flip(pvIdx, k++) ;
        } else {
            k++ ;
        }
    }
    return pvIdx ;
}

int Sorter::Fast_partition (int a, int b) {
    // Edge cases
    if (b - a <= 1) {
        return a ;
    } else if (b - a == 2) {
        if (m_link->cmp_ii(a+1, a)) { m_link->flip(a, a+1) ; }
        m_link->solved(a+1) ;
        return a ;
    } else if (b - a < 6) {
        return Middle_partition(a, b) ;
    }
    int p1 = (a + b) / 2 ;
    int p2 = (a + p1) / 2 ;
    int p3 = (b + p1) / 2 ;
    int pVal1 = m_link->get(p1) ;
    int pVal2 = m_link->get(p2) ;
    int pVal3 = m_link->get(p3) ;
    int p ;
    // Find median
    if (m_link->cmp_ii(p1, p3)) {
        if (m_link->cmp_ee(pVal1, pVal2)) { if (m_link->cmp_ee(pVal2, pVal3)) { p = p2 ; } else { p = p3 ; } } else { p = p1 ; }
    } else {
        if (m_link->cmp_ee(pVal1, pVal2)) { p = p1 ; } else { if (m_link->cmp_ee(pVal2, pVal3)) { p = p2 ; } else { p = p3 ; } }
    }
    m_link->flip(p, a) ;
    int pv = a ;
    int pvVal = m_link->get(a) ;
    int i = a, j = b ;
    for (;;) {
        do {
            j-- ;
        } while ((i < j) && !m_link->cmp_ie(j, pvVal)) ;
        do {
            i++ ;
        } while ((i < j) && m_link->cmp_ie(i, pvVal)) ;
        if (i < j) {
            m_link->flip(i, j) ;
        } else {
            m_link->flip(pv, j) ;
            return j ;
        }
    }
}

// ////////// //
// MERGE-TYPE //
// ////////// //

void Sorter::Mergesort_standard () {
    int tab [m_size] ;
    Mergesort(0, m_size, tab) ;
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::Mergesort_pack () {
    int tab [m_size] ;
    for (int d = 1; d < m_size; d <<= 1) {
        for (int i = 0; i+d < m_size; i += 2*d) {
            merge(i, i+d, i+2*d, tab) ;
        }
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::Mergesort_weave () {
    int tab [m_size] ;
    Weavesort(0, m_size, tab) ;
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::Weavesort (int a, int b, int tab []) {
    if (b - a < 2) return ;
    int m = (a + b) / 2 ;
    Weavesort(a, m, tab) ;
    Weavesort(m, b, tab) ;
    weave(a, m, b, tab) ;
    LocalInsertSort(a, b) ;
}

void Sorter::Mergesort (int a, int b, int tab []) {
    if (b - a < 2) return ;
    int m = (a + b) / 2 ;
    Mergesort(a, m, tab) ;
    Mergesort(m, b, tab) ;
    merge(a, m, b, tab) ;
}

void Sorter::merge (int a, int b, int c, int tab []) {
    int i = a, j = b, k = 0 ;
    while ((i < b) && (j < c) && (j < m_size)) {
        if (m_link->cmp_ii(i, j)) {
            tab[a + k++] = m_link->get(i++) ;
        } else {
            tab[a + k++] = m_link->get(j++) ;
        }
    }
    while ((i < b) && (i < m_size)) { tab[a + k++] = m_link->get(i++) ; }
    for (int h = a; h < j; h++) {
        m_link->set(h, tab[h]) ;
        m_link->tmpsolve(h) ;
    }
    for (int h = j; (h < c) && (h < m_size); h++) m_link->tmpsolve(h) ;
}

void Sorter::weave (int a, int b, int c, int tab []) {
    int i = a, j = b, k = 0 ;
    while ((i < b) && (j < c) && (j < m_size)) {
        tab[a + k++] = m_link->get(i++) ;
        tab[a + k++] = m_link->get(j++) ;
    }
    while ((i < b) && (i < m_size)) { tab[a + k++] = m_link->get(i++) ; }
    for (int h = a; h < j; h++) {
        m_link->set(h, tab[h]) ;
        m_link->tmpsolve(h) ;
    }
}


// ////////// //
// RADIX-TYPE //
// ////////// //

void Sorter::RadixLSD2 () {
    RadixLSD(1) ;
}

void Sorter::RadixLSD4 () {
    RadixLSD(2) ;
}

void Sorter::RadixLSD8 () {
    RadixLSD(3) ;
}

void Sorter::RadixLSD (int baselg) {
    int M = (1 << baselg) ;
    int maxidx = 0 ;
    for (int i = 0; i < m_size; i++) {
        if (m_link->cmp_ii(maxidx, i)) {
            maxidx = i ;
        }
    }
    int maxval = m_link->get(maxidx) ;
    int access [M] ;
    int cpy [m_size] ;
    for (int filter = M-1, d = 0; (filter <= maxval) || (filter & maxval); filter <<= baselg, d += baselg) {
        for (int m = 0; m < M; m++) { access[m] = 0 ; }
        for (int n = 0; n < m_size; n++) { cpy[n] = m_link->get(n) ; }
        for (int n = 0; n < m_size; n++) { access[(cpy[n]&filter)>>d]++ ; }
        for (int m = 1; m < M; m++) { access[m] += access[m-1] ; }
        for (int n = m_size-1; n >= 0; n--) {
            int k = --access[((cpy[n])&filter)>>d] ;
            m_link->set(k, cpy[n]) ;
            m_link->tmpsolve(k) ;
        }
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::RadixMSD2 () {
    RadixMSDLauncher(1) ;
}

void Sorter::RadixMSD4 () {
    RadixMSDLauncher(2) ;
}

void Sorter::RadixMSD8 () {
    RadixMSDLauncher(3) ;
}

void Sorter::RadixMSDLauncher (int baselg) {
    int M = (1 << baselg) ;
    int maxidx = 0 ;
    for (int i = 0; i < m_size; i++) {
        if (m_link->cmp_ii(maxidx, i)) {
            maxidx = i ;
        }
    }
    int maxval = m_link->get(maxidx) ;
    int filter = M - 1 ;
    int shift = 0 ;
    int level ;
    for (level = 0; (filter <= maxval) || (filter & maxval) ; level++) {
        filter <<= baselg ;
        shift += baselg ;
    }
    RadixMSD(0, m_size, baselg, M, filter, shift, level) ;
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::RadixMSD (int start, int end, int baselg, int M, int filter, int shift, int level) {
    int access [M] ;
    int cpy [end-start] ;
    for (int m = 0; m < M; m++) { access[m] = 0 ; }
    for (int n = start; n < end; n++) { cpy[n-start] = m_link->get(n) ; }
    for (int n = start; n < end; n++) { access[(cpy[n-start]&filter)>>shift]++ ; }
    for (int m = 1; m < M; m++) { access[m] += access[m-1] ; }
    for (int n = end-1; n >= start; n--) {
        int k = --access[((cpy[n-start])&filter)>>shift] + start ;
        m_link->set(k, cpy[n-start]) ;
        m_link->tmpsolve(k) ;
    }
    if (level) {
        for (int m = 0; m < M; m++) {
            RadixMSD(start + access[m], (m==M-1)?end:start+access[m+1], baselg, M, filter>>baselg, shift-baselg, level-1) ;
        }
    }
}

void Sorter::Counting () {
    int access [m_size] ;
    for (int i = 0; i < m_size; i++) access[i] = 0 ;
    int cpy [m_size] ;
    for (int n = 0; n < m_size; n++) {
        cpy[n] = m_link->get(n) ;
        access[cpy[n]]++ ;
    }
    for (int m = 1; m < m_size; m++) { access[m] += access[m-1] ; }
    for (int n = m_size-1; n >= 0; n--) {
        int k = --access[cpy[n]] ;
        m_link->set(k, cpy[n]) ;
        m_link->solved(k) ;
    }
}

// /////////// //
// BUCKET-TYPE //
// /////////// //

void Sorter::LocalInsertSort (int beg, int end) {
    for (int i = beg; i < end; i++) {
        int j = i ;
        while ((j > beg) && (m_link->cmp_ii(j, j-1))) {
            m_link->flip(j, j-1) ;
            m_link->tmpsolve(j) ;
            j-- ;
        }
        m_link->tmpsolve(j) ;
        if (j > beg) m_link->tmpsolve(j-1) ;
    }
}

int Sorter::Dichotomy (int val, int beg, int end) {
    if (end == beg+1) {
        return beg ;
    } else {
        int mid = (beg + end) / 2 ;
        if (m_link->cmp_ei(val, mid)) {
            m_link->tmpsolve(mid) ;
            return Dichotomy(val, beg, mid) ;
        } else {
            m_link->tmpsolve(mid) ;
            return Dichotomy(val, mid, end) ;
        }
    }
}

void Sorter::Sampler () {
    int sampleSize = std::ceil(std::sqrt(m_size)) ;
    if (m_size < 16) {
        LocalInsertSort(0, m_size) ;
        return ;
    }
    for (int i = 0; i < sampleSize; i++) {
        m_link->flip(i, std::min(i*sampleSize, m_size-1)) ;
    }
    LocalInsertSort(0, sampleSize) ;
    int access [sampleSize+1] ;
    for (int i = 0; i <= sampleSize; i++) access[i] = 0 ;
    // Count members of each interval
    for (int i = sampleSize; i < m_size; i++) { access[Dichotomy(m_link->get(i), 0, sampleSize+1)]++ ; }
    // Turn into cumulative
    for (int j = 1; j <= sampleSize; j++) { access[j] += access[j-1] ; }
    // Copy in order then rewrite
    int cpy [m_size - sampleSize] ;
    for (int i = m_size-1; i >= sampleSize; i--) { cpy[--access[Dichotomy(m_link->get(i), 0, sampleSize+1)]] = m_link->get(i) ; }
    for (int i = sampleSize; i < m_size; i++) { m_link->set(i, cpy[i - sampleSize]) ; }
    // Sub-sort
    for (int i = 0; i < sampleSize; i++) { LocalInsertSort(sampleSize+access[i], sampleSize+access[i+1]) ; }
    LocalInsertSort(sampleSize+access[sampleSize], m_size) ;
    merge(0, sampleSize, m_size, cpy) ;
    for (int i = 0; i < m_size; i++) { m_link->solved(i) ; }
}

void Sorter::Bucket () {
    int bucketcount = std::ceil(std::sqrt(m_size)) ;
    if (m_size < 16) {
        LocalInsertSort(0, m_size) ;
        return ;
    }
    int access [bucketcount+1] ;
    for (int i = 0; i <= bucketcount; i++) access[i] = 0 ;
    // Count members of each interval
    for (int i = 0; i < m_size; i++) { access[m_link->get(i) / bucketcount]++ ; }
    // Turn into cumulative
    for (int j = 1; j <= bucketcount; j++) { access[j] += access[j-1] ; }
    // Copy in order then rewrite
    int cpy [m_size] ;
    for (int i = m_size-1; i >= 0; i--) { cpy[--access[m_link->get(i) / bucketcount]] = m_link->get(i) ; }
    for (int i = 0; i < m_size; i++) { m_link->set(i, cpy[i]) ; }
    // Sub-sort
    for (int i = 0; i < bucketcount; i++) { LocalInsertSort(access[i], access[i+1]) ; }
    for (int i = 0; i < m_size; i++) { m_link->solved(i) ; }
}

// /////////// //
// INSERT-TYPE //
// /////////// //

void Sorter::Insertion () {
    for (int i = 1; i < m_size; i++) {
        int j = i ;
        int val = m_link->get(j) ;
        while ((j > 0) && (m_link->cmp_ei(val, j-1))) {
            m_link->flip(j, j-1) ;
            m_link->tmpsolve(j) ;
            j-- ;
        }
        m_link->tmpsolve(j) ;
        if (j > 0) m_link->tmpsolve(j-1) ;
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::Shell () {
    int gaps [8] = {701, 301, 132, 57, 23, 10, 4, 1} ;
    for (int g = 0; g < 8; g++) {
        int gap = gaps[g] ;
        for (int i = gap; i < m_size; i++) {
            int val = m_link->get(i) ;
            for (int j = i; j >= gap && m_link->cmp_ei(val, j-gap); j -= gap) {
                m_link->flip(j, j-gap) ;
            }
        }
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

// ///////// //
// HEAP-TYPE //
// ///////// //

void Sorter::Reheapify_max (int i, int end) {
    int lt = 2*i + 1;
    if (lt >= end) {
        return ;
    }
    int maxson = lt ;
    if ((lt < end-1) && m_link->cmp_ii(lt, lt+1)) {
        maxson++ ;
    }
    m_link->setBinHeapLv(lt) ;
    if (lt < end-1) m_link->setBinHeapLv(lt+1) ;
    if (m_link->cmp_ii(i, maxson)) {
        m_link->flip(i, maxson) ;
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(maxson) ;
        Reheapify_max(maxson, end) ;
    } else {
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(maxson) ;
    }
}

void Sorter::Reheapify_min (int i, int end) {
    int lt = 2*i + 1 ;
    if (lt >= end) {
        return ;
    }
    int maxson = lt ;
    if ((lt < end-1) && m_link->cmp_ii(lt+1, lt)) {
        maxson++ ;
    }
    m_link->setBinHeapLv(lt) ;
    if (lt < end-1) m_link->setBinHeapLv(lt+1) ;
    if (m_link->cmp_ii(maxson, i)) {
        m_link->flip(i, maxson) ;
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(maxson) ;
        Reheapify_min(maxson, end) ;
    } else {
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(maxson) ;
    }
}

void Sorter::Reheapify_tern (int i, int end) {
    int lt = 3*i + 1 ;
    if (lt >= end) {
        return ;
    }
    int maxson = lt ;
    if ((lt < end-1) && m_link->cmp_ii(lt, lt+1)) {
        maxson = lt+1 ;
    }
    if ((lt < end-2) && m_link->cmp_ii(maxson, lt+2)) {
        maxson = lt+2 ;
    }
    m_link->setTerHeapLv(lt) ;
    if (lt < end-1) m_link->setTerHeapLv(lt+1) ;
    if (lt < end-2) m_link->setTerHeapLv(lt+2) ;
    if (m_link->cmp_ii(i, maxson)) {
        m_link->flip(i, maxson) ;
        m_link->setTerHeapLv(i) ;
        m_link->setTerHeapLv(maxson) ;
        Reheapify_tern(maxson, end) ;
    }else {
        m_link->setTerHeapLv(i) ;
        m_link->setTerHeapLv(maxson) ;
    }
}

void Sorter::Heapsort_max () {
    for (int i = m_size / 2 + 1; i >= 0; i--) {
        Reheapify_max(i, m_size) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        m_link->flip(i, 0) ;
        Reheapify_max(0, i) ;
        m_link->solved(i) ;
    }
}

void Sorter::Heapsort_min () {
    for (int i = m_size / 2 + 1; i >= 0; i--) {
        Reheapify_min(i, m_size) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        m_link->flip(i, 0) ;
        Reheapify_min(0, i) ;
        m_link->tmpsolve(i) ;
    }
    for (int i = 0; i <= m_size/2; i++) {
        m_link->flip(i, m_size-1-i) ;
        m_link->solved(i) ;
        m_link->solved(m_size-1-i) ;
    }
    m_link->solved(m_size/2+1) ;
}

void Sorter::Heapsort_tern () {
    for (int i = m_size / 3 + 2; i >= 0; i--) {
        Reheapify_tern(i, m_size) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        m_link->flip(i, 0) ;
        Reheapify_tern(0, i) ;
        m_link->solved(i) ;
    }
}


#define UP(IA,IB) temp = IA; IA += IB + 1; IB = temp
#define DOWN(IA,IB) temp = IB; IB = IA - IB - 1; IA = temp

static int q, r, p, b, c, r1, b1, c1;

void Sorter::Sift () {
    int r0, r2, temp ;
    int t ;
    r0 = r1 ;
    t = m_link->get(r0) ;
    while (b1 >= 3) {
        r2 = r1 - b1 + c1 ;
        if (!m_link->cmp_ii(r1 - 1, r2)) {
            r2 = r1 - 1 ;
            DOWN(b1, c1) ;
        }
        if (m_link->cmp_ie(r2, t)) {
            b1 = 1 ;
        } else {
            m_link->set(r1, m_link->get(r2)) ;
            r1 = r2 ;
            DOWN(b1, c1) ;
        }
    }
    if (r1 - r0) m_link->set(r1, t) ;
}

void Sorter::Trinkle () {
    int p1, r2, r3, r0, temp ;
    int t ;
    p1 = p ;
    b1 = b ;
    c1 = c ;
    r0 = r1 ;
    t = m_link->get(r0) ;
    while (p1 > 0) {
        while ((p1 & 1) == 0) {
            p1 >>= 1 ;
            UP(b1, c1) ;
        }
        r3 = r1 - b1 ;
        if ((p1 == 1) || m_link->cmp_ie(r3, t)) {
            p1 = 0 ;
        } else {
            --p1 ;
            if (b1 == 1) {
                m_link->set(r1, m_link->get(r3)) ;
                r1 = r3 ;
            } else {
                if (b1 >= 3) {
                    r2 = r1 - b1 + c1 ;
                    if (!m_link->cmp_ii(r1 - 1, r2)) {
                        r2 = r1 - 1 ;
                        DOWN(b1, c1) ;
                        p1 <<= 1 ;
                    }
                    if (m_link->cmp_ii(r2, r3)) {
                        m_link->set(r1, m_link->get(r3));
                        r1 = r3 ;
                    } else {
                        m_link->set(r1, m_link->get(r2)) ;
                        r1 = r2 ;
                        DOWN(b1, c1) ;
                        p1 = 0 ;
                    }
                }
            }
        }
    }
    if (r0 - r1) m_link->set(r1, t) ;
    Sift() ;
}

void Sorter::SemiTrinkle () {
    r1 = r - c ;
    if (!m_link->cmp_ii(r1, r)) {
        m_link->flip(r, r1) ;
        Trinkle() ;
    }
}

void Sorter::SmoothSort () {
    int temp ;
    q = 1 ;
    r = 0 ;
    p = 1 ;
    b = 1 ;
    c = 1 ;
    while (q < m_size) {
        r1 = r ;
        if ((p & 7) == 3) {
            b1 = b ;
            c1 = c ;
            Sift() ;
            p = (p + 1) >> 2 ;
            UP(b, c) ;
            UP(b, c) ;
        } else if ((p & 3) == 1) {
            if (q + c < m_size) {
                b1 = b ;
                c1 = c ;
                Sift() ;
            } else {
                Trinkle() ;
            }
            DOWN(b, c) ;
            p <<= 1 ;
            while (b > 1) {
                DOWN(b, c) ;
                p <<= 1 ;
            }
            p++ ;
        }
        q++ ;
        r++ ;
    }
    r1 = r ;
    Trinkle() ;
    while (q > 1) {
        m_link->solved(--q) ;
        if (b == 1) {
            r-- ;
            p-- ;
            while ((p & 1) == 0) {
                p >>= 1 ;
                UP(b, c) ;
            }
        } else {
            if (b >= 3) {
                p-- ;
                r = r - b + c ;
                if (p > 0) SemiTrinkle() ;
                DOWN(b, c) ;
                p = (p << 1) + 1 ;
                r = r + c ;
                SemiTrinkle() ;
                DOWN(b, c) ;
                p = (p << 1) + 1 ;
            }
        }
    }
    m_link->solved(0) ;
}

// /////////// //
// SELECT-TYPE //
// /////////// //

void Sorter::Selection_max () {
    for (int i = m_size; i > 1; i--) {
        int M = 0 ;
        int MVal = m_link->get(M) ;
        for (int j = 1; j < i; j++) {
            if (m_link->cmp_ei(MVal, j)) {
                M = j ;
                MVal = m_link->get(j) ;
            }
        }
        m_link->flip(M, i-1) ;
        m_link->solved(i-1) ;
    }
    m_link->solved(0) ;
}

void Sorter::Selection_min () {
    for (int i = 0; i < m_size-1; i++) {
        int m = i ;
        int mVal = m_link->get(m) ;
        for (int j = i; j < m_size; j++) {
            if (m_link->cmp_ie(j, mVal)) {
                m = j ;
                mVal = m_link->get(j) ;
            }
        }
        m_link->flip(m, i) ;
        m_link->solved(i) ;
    }
    m_link->solved(m_size-1) ;
}

void Sorter::Selection_dbl () {
    int mid = m_size / 2 ;
    for (int i = 0; i < mid; i++) {
        int m = i, M = i+1 ;
        int mVal = m_link->get(m) ;
        int MVal = m_link->get(M) ;
        if (m_link->cmp_ii(M, m)) {
            m++ ; M-- ;
        }
        for (int j = i; j <= m_size-1-i; j++) {
            if (m_link->cmp_ie(j, mVal)) {
                m = j ;
                mVal = m_link->get(j) ;
            } else if (m_link->cmp_ei(MVal, j)) {
                M = j ;
                MVal = m_link->get(j) ;
            }
        }
        if ((M == i) && (m == m_size-1-i)) {
            m_link->flip(m, M) ;
        } else if (M == i) {
            m_link->flip(M, m_size-1-i) ;
            m_link->flip(m, i) ;
        } else {
            m_link->flip(m, i) ;
            m_link->flip(M, m_size-1-i) ;
        }
        m_link->solved(i) ;
        m_link->solved(m_size-1-i) ;
    }
    m_link->solved(mid) ;
}

void Sorter::Cycle () {
    for (int start = 0; start < m_size-1; start++) {
        if (start > 0) m_link->solved(start-1) ;
        int item = m_link->get(start) ;
        int pos = start ;
        for (int i = start+1; i < m_size; i++) {
            if (m_link->cmp_ie(i, item)) pos++ ;
        }
        if (pos == start) continue ;
        while (item == m_link->get(pos)) pos++ ;
        int tmp = m_link->get(pos) ;
        m_link->set(pos, item) ;
        m_link->solved(pos) ;
        item = tmp ;
        while (pos != start) {
            pos = start ;
            for (int i = start+1; i < m_size; i++) {
                if (m_link->cmp_ie(i, item)) pos++ ;
            }
            while (item == m_link->get(pos)) pos++ ;
            tmp = m_link->get(pos) ;
            m_link->set(pos, item) ;
            m_link->solved(pos) ;
            item = tmp ;
        }
    }
    m_link->solved(m_size-2) ;
    m_link->solved(m_size-1) ;
}

void Sorter::CycleOpt () {
    bool placed [m_size] ;
    for (int i = 0; i < m_size; i++) placed[i] = false ;
    for (int start = 0; start < m_size-1; start++) {
        if (start > 0) m_link->solved(start-1) ;
        if (placed[start]) continue ;
        int item = m_link->get(start) ;
        int pos = start ;
        for (int i = start+1; i < m_size; i++) {
            if (m_link->cmp_ie(i, item)) pos++ ;
        }
        if (pos == start) continue ;
        while (item == m_link->get(pos)) pos++ ;
        int tmp = m_link->get(pos) ;
        m_link->set(pos, item) ;
        m_link->solved(pos) ;
        placed[pos] = true ;
        item = tmp ;
        while (pos != start) {
            pos = start ;
            for (int i = start+1; i < m_size; i++) {
                if (m_link->cmp_ie(i, item)) pos++ ;
            }
            while (item == m_link->get(pos)) pos++ ;
            tmp = m_link->get(pos) ;
            m_link->set(pos, item) ;
            m_link->solved(pos) ;
            placed[pos] = true ;
            item = tmp ;
        }
    }
    m_link->solved(m_size-2) ;
    m_link->solved(m_size-1) ;
}

void Sorter::Pancake () {
    for (int i0 = 0; i0 < m_size; i0++) {
        int best = i0 ;
        for (int i = i0; i < m_size; i++) {
            if (m_link->cmp_ii(i, best)) {
                best = i ;
            }
        }
        for (int i = 0; i < (m_size-best)/2; i++) {
            m_link->flip(best+i, m_size-1-i) ;
        }
        for (int i = 0; i < (m_size-i0)/2; i++) {
            m_link->flip(i0+i, m_size-1-i) ;
        }
        m_link->solved(i0) ;
    }
}


// //////// //
// NETWORKS //
// //////// //

void Sorter::Bitonic () {
    Bitonic_merge(0, m_size) ;
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::Bitonic_sort (int a, int b) {
    int m = (b - a) / 2 ;
    if (b-a == 2) {
        m_link->flip_ifn_cmp(a, a+1) ;
        return ;
    }
    for (int i = 0; i < m; i++) {
        m_link->flip_ifn_cmp(a+i, a+m+i) ;
    }
    Bitonic_sort(a, a+m) ;
    Bitonic_sort(a+m, b) ;
}

void Sorter::Bitonic_merge (int a, int b) {
    if (b-a == 2) {
        m_link->flip_ifn_cmp(a, a+1) ;
        return ;
    }
    int m = (b - a) / 2 ;
    Bitonic_merge(a, a+m) ;
    Bitonic_merge(a+m, b) ;
    for (int i = 0; i < m; i++) {
        m_link->flip_ifn_cmp(a+i, b-1-i) ;
    }
    Bitonic_sort(a, a+m) ;
    Bitonic_sort(a+m, b) ;
}

void Sorter::OddEven () {
    OddEven_sort(0, m_size) ;
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

void Sorter::OddEven_sort (int a, int b) {
    if (b-a <= 1) return ;
    int m = a + (b-1-a)/2 ;
    OddEven_sort(a, m+1) ;
    OddEven_sort(m+1, b) ;
    OddEven_merge(a, b, 1) ;
}

void Sorter::OddEven_merge (int a, int b, int r) {
    int step = 2 * r ;
    if (step < b-1-a) {
        OddEven_merge(a, b, step) ;
        OddEven_merge(a + r, b, step) ;
        for (int i = a + r; i < b - 1 - r; i += step) {
            m_link->flip_ifn_cmp(i, i + r) ;
        }
    } else {
        m_link->flip_ifn_cmp(a, a + r) ;
    }
}
#endif

// EXTENSIONS TO ADD :
/* Recursive bucket- and sample-sort
 */
