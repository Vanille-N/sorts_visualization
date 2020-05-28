#include "sorter.h"

Sorter::Sorter(Linker * link) {
    m_link = link ;
    m_size = m_link->getSize() ;
    ok = true ;
}

void Sorter::stop () {
    ok = false ;
}

// /////////// //
// BUBBLE-TYPE //
// /////////// //

void Sorter::Bubble_upwards () {
    for (int i = 0; i < m_size; i++) {
        bool sorted = true ;
        for (int j = m_size-1; j > i; j--) {
            BREAKPOINT ;
            if (m_link->cmp_ii(j, j-1)) {
                sorted = false ;
                m_link->flip(j, j-1) ;
            }
        }
        m_link->solved(i) ;
        if (sorted) {
            for (int j = i+1; j < m_size; j++) {
                BREAKPOINT ;
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
            BREAKPOINT ;
            if (m_link->cmp_ii(j, j-1)) {
                sorted = false ;
                m_link->flip(j, j-1) ;
            }
        }
        m_link->solved(i-1) ;
        if (sorted) {
            for (int j = 0; j < i; j++) {
                BREAKPOINT ;
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
                BREAKPOINT ;
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
                BREAKPOINT ;
                if (m_link->cmp_ii(i, i-1)) {
                    m_link->flip(i, i-1) ;
                    k = i ;
                }
            }
            do {
                BREAKPOINT ;
                m_link->solved(lt++) ;
            } while (lt < k) ;
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
            BREAKPOINT ;
            if (m_link->cmp_ii(j, j-s)) {
                sorted = false ;
                m_link->flip(j, j-s) ;
            }
        }
        if (sorted) {
            for (int j = 0; j < m_size; j++) {
                BREAKPOINT ;
                m_link->solved(j) ;
            }
            break ;
        }
        step /= float(1.4) ;
    }

}

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

void Sorter::Quick_median () {
    Quicksort(0, m_size, 3) ;
}

void Sorter::Quicksort (int a, int b, int part) {
    if (b - a == 1) {
        m_link->solved(a) ;
        return ;
    } else if (b - a < 1) {
        return ;
    } else if (b - a == 2) {
        m_link->flip_ifn_cmp(a, b-1) ;
        m_link->solved(a) ;
        m_link->solved(b-1) ;
        return ;
    }
    int pv = 0 ;
    switch (part) {
        case 0: pv = Naive_partition(a, b) ; break ;
        case 1: pv = Middle_partition(a, b) ; break ;
        case 2: pv = Fast_partition(a, b) ; break ;
        case 3: pv = Median_partition(a, b) ; break ;
    }
    BREAKPOINT ;
    m_link->solved(pv) ;
    Quicksort(a, pv, part) ;
    BREAKPOINT ;
    Quicksort(pv+1, b, part) ;
    BREAKPOINT ;
}

int Sorter::Naive_partition (int a, int b) {
    item * pvVal = m_link->send(a) ;
    int pvIdx = a ;
    int k = a + 1 ;
    while (k < b) {
        BREAKPOINT 0 ;
        if (m_link->cmp_ie(k, pvVal)) {
            m_link->flip(pvIdx++, k) ;
            if (pvIdx != k) m_link->flip(pvIdx, k) ;
            k++ ;
        } else {
            k++ ;
        }
    }
    return pvIdx ;
}

int Sorter::Middle_partition (int a, int b) {
    int pvIdx = a ;
    m_link->flip(pvIdx, (b+a)/2) ;
    item * pvVal = m_link->send(a) ;
    int k = a + 1 ;
    while (k < b) {
        BREAKPOINT 0 ;
        if (m_link->cmp_ie(k, pvVal)) {
            m_link->flip(pvIdx++, k) ;
            if (pvIdx != k) m_link->flip(pvIdx, k) ;
            k++ ;
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
    item * pVal1 = m_link->send(p1) ;
    item * pVal2 = m_link->send(p2) ;
    item * pVal3 = m_link->send(p3) ;
    int p ;
    // Find median
    if (m_link->cmp_ii(p1, p3)) {
        if (m_link->cmp_ee(pVal1, pVal2)) { if (m_link->cmp_ee(pVal2, pVal3)) { p = p2 ; } else { p = p3 ; } } else { p = p1 ; }
    } else {
        if (m_link->cmp_ee(pVal1, pVal2)) { p = p1 ; } else { if (m_link->cmp_ee(pVal2, pVal3)) { p = p2 ; } else { p = p3 ; } }
    }
    m_link->flip(p, a) ;
    int pv = a ;
    item * pvVal = m_link->send(pv) ;
    int i = a, j = b ;
    for (;;) {
        do {
            BREAKPOINT 0 ;
            j-- ;
        } while ((i < j) && !m_link->cmp_ie(j, pvVal)) ;
        do {
            BREAKPOINT 0 ;
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

inline void flip (int * a, int * b) {
    int t = *a ;
    *a = *b ;
    *b = t ;
}

int Sorter::Median_of_5 (int idx) {
    int a = idx, b = idx+1, c = idx+2, d=idx+3, e = idx+4 ;
    if (m_link->cmp_ii(b, a)) flip(&a, &b) ; // (a < b)
    if (m_link->cmp_ii(d, c)) flip(&c, &d) ; // (c < d)
    if (m_link->cmp_ii(c, a)) { flip(&a, &c) ; flip(&b, &d) ; } // (a < b) ; (c < d) ; (a < c)
    // Therefore a < b, c, d : a is not the median
    if (m_link->cmp_ii(b, e)) flip(&e, &b) ; // (e < b) ; (c < d)
    if (m_link->cmp_ii(c, e)) { flip(&e, &c) ; flip(&b, &d) ; } // (e < b) ; (c < d) ; (e < c) ;
    // Therefore e < b, c, d : e is not the median
    if (m_link->cmp_ii(c, b)) flip(&b, &c) ;
//    printf("Med5 : %d | (%d,%d,%d,%d,%d) -> %d\n", idx, m_link->get(idx).val, m_link->get(idx+1).val, m_link->get(idx+2).val, m_link->get(idx+3).val, m_link->get(idx+4).val, m_link->get(b).val) ;
    return b ;
}

int Sorter::Qselect (int a, int b , int k) {
//    printf("Sel : %d %d %d\n", a, b, k) ;
    if (b - a <= 1) return a ;
    if (b - a == 2) {
        m_link->flip_ifn_cmp(a, a+1) ;
        return a+k ;
    }
    int pv = Median_partition(a+1, b-1) ;
    BREAKPOINT 0 ;
    if (a + k < pv) {
        return Qselect(a, pv, k) ;
    } else {
        return Qselect(pv, b, k-pv+a) ;
    }
}

int Sorter::Median_of_medians (int a, int b) {
//    printf("MoM : %d %d\n", a, b) ;
    if (b - a <= 1) {
        return a ;
    }
    int j = a ;
    for (int i = a ; i < b-5 ; i += 5) {
        BREAKPOINT 0 ;
        m_link->flip(j++, Median_of_5(i)) ;
    }
    m_link->flip(b-1, j++) ;
    return Qselect(a, j, (j-a)/2) ;
}

int Sorter::Median_partition (int a, int b) {
//    printf("MeP : %d %d\n", a, b) ;
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
    int pv = Median_of_medians(a, b) ;
    BREAKPOINT 0 ;
    m_link->flip(pv, a) ;
    pv = a ;
    item * pvVal = m_link->send(pv) ;
    int i = a, j = b ;
    for (;;) {
        do {
            BREAKPOINT 0 ;
            j-- ;
        } while ((i < j) && !m_link->cmp_ie(j, pvVal)) ;
        do {
            BREAKPOINT 0 ;
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
    item * tab [m_size] ;
    Mergesort(0, m_size, tab) ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Mergesort_pack () {
    item * tab [m_size] ;
    for (int d = 1; d < m_size; d <<= 1) {
        for (int i = 0; i+d < m_size; i += 2*d) {
            BREAKPOINT ;
            merge(i, i+d, i+2*d, tab) ;
        }
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Mergesort_weave () {
    item * tab [m_size] ;
    Weavesort(0, m_size, tab) ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Weavesort (int a, int b, item * tab []) {
    if (b - a < 2) return ;
    int m = (a + b) / 2 ;
    Weavesort(a, m, tab) ;
    BREAKPOINT ;
    Weavesort(m, b, tab) ;
    BREAKPOINT ;
    weave(a, m, b, tab) ;
    BREAKPOINT ;
    LocalInsertSort(a, b) ;
}

void Sorter::Mergesort (int a, int b, item * tab []) {
    if (b - a < 2) return ;
    int m = (a + b) / 2 ;
    Mergesort(a, m, tab) ;
    BREAKPOINT ;
    Mergesort(m, b, tab) ;
    BREAKPOINT ;
    merge(a, m, b, tab) ;
}

void Sorter::merge (int a, int b, int c, item * tab []) {
    int i = a, j = b, k = 0 ;
    while ((i < b) && (j < c) && (j < m_size)) {
        BREAKPOINT ;
        if (m_link->cmp_ii(i, j)) {
            tab[a + k++] = m_link->send(i++) ;
        } else {
            tab[a + k++] = m_link->send(j++) ;
        }
    }
    while ((i < b) && (i < m_size)) {
        BREAKPOINT ;
        tab[a + k++] = m_link->send(i++) ;
    }
    for (int h = a; h < j; h++) {
        BREAKPOINT ;
        m_link->receive(h, tab[h]) ;
        m_link->tmpsolve(h) ;
    }
    for (int h = j; (h < c) && (h < m_size); h++) {
        BREAKPOINT ;
        m_link->tmpsolve(h) ;
    }
}

void Sorter::weave (int a, int b, int c, item * tab []) {
    int i = a, j = b, k = 0 ;
    while ((i < b) && (j < c) && (j < m_size)) {
        BREAKPOINT ;
        tab[a + k++] = m_link->send(i++) ;
        tab[a + k++] = m_link->send(j++) ;
    }
    while ((i < b) && (i < m_size)) {
        BREAKPOINT ;
        tab[a + k++] = m_link->send(i++) ;
    }
    for (int h = a; h < j; h++) {
        BREAKPOINT ;
        m_link->receive(h, tab[h]) ;
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
        BREAKPOINT ;
        if (m_link->cmp_ii(maxidx, i)) {
            maxidx = i ;
        }
    }
    item * maxval = m_link->send(maxidx) ;
    int access [M] ;
    item * cpy [m_size] ;
    for (int filter = M-1, d = 0; (filter <= maxval->val) || (filter & maxval->val); filter <<= baselg, d += baselg) {
        for (int m = 0; m < M; m++) {
            BREAKPOINT ;
            access[m] = 0 ;
        }
        for (int n = 0; n < m_size; n++) {
            BREAKPOINT ;
            cpy[n] = m_link->send(n) ;
        }
        for (int n = 0; n < m_size; n++) {
            BREAKPOINT ;
            access[(cpy[n]->val&filter)>>d]++ ;
        }
        for (int m = 1; m < M; m++) {
            BREAKPOINT ;
            access[m] += access[m-1] ;
        }
        for (int n = m_size-1; n >= 0; n--) {
            BREAKPOINT ;
            int k = --access[(cpy[n]->val&filter)>>d] ;
            m_link->receive(k, cpy[n]) ;
            m_link->tmpsolve(k) ;
        }
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
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
        BREAKPOINT ;
        if (m_link->cmp_ii(maxidx, i)) {
            maxidx = i ;
        }
    }
    item * maxval = m_link->send(maxidx) ;
    int filter = M - 1 ;
    int shift = 0 ;
    int level ;
    for (level = 0; (filter <= maxval->val) || (filter & maxval->val) ; level++) {
        BREAKPOINT ;
        filter <<= baselg ;
        shift += baselg ;
    }
    RadixMSD(0, m_size, baselg, M, filter, shift, level) ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::RadixMSD (int start, int end, int baselg, int M, int filter, int shift, int level) {
    int access [M] ;
    item * cpy [end-start] ;
    for (int m = 0; m < M; m++) {
        access[m] = 0 ;
    }
    for (int n = start; n < end; n++) {
        BREAKPOINT ;
        cpy[n-start] = m_link->send(n) ;
    }
    for (int n = start; n < end; n++) {
        BREAKPOINT ;
        access[(cpy[n-start]->val&filter)>>shift]++ ;
    }
    for (int m = 1; m < M; m++) {
        BREAKPOINT ;
        access[m] += access[m-1] ;
    }
    for (int n = end-1; n >= start; n--) {
        BREAKPOINT ;
        int k = --access[((cpy[n-start]->val)&filter)>>shift] + start ;
        m_link->receive(k, cpy[n-start]) ;
        m_link->tmpsolve(k) ;
    }
    if (level) {
        for (int m = 0; m < M; m++) {
            BREAKPOINT ;
            RadixMSD(start + access[m], (m==M-1)?end:start+access[m+1], baselg, M, filter>>baselg, shift-baselg, level-1) ;
        }
    }
}

void Sorter::Counting () {
    int access [m_size] ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        access[i] = 0 ;
    }
    item * cpy [m_size] ;
    for (int n = 0; n < m_size; n++) {
        BREAKPOINT ;
        cpy[n] = m_link->send(n) ;
        access[cpy[n]->val]++ ;
    }
    for (int m = 1; m < m_size; m++) {
        BREAKPOINT ;
        access[m] += access[m-1] ;
    }
    for (int n = m_size-1; n >= 0; n--) {
        BREAKPOINT ;
        int k = --access[cpy[n]->val] ;
        m_link->receive(k, cpy[n]) ;
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
            BREAKPOINT ;
            m_link->flip(j, j-1) ;
            m_link->tmpsolve(j) ;
            j-- ;
        }
        m_link->tmpsolve(j) ;
        if (j > beg) m_link->tmpsolve(j-1) ;
    }
}

int Sorter::Dichotomy (item * val, int beg, int end) {
    if (end == beg+1) {
        return beg ;
    } else {
        BREAKPOINT 0 ;
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

void Sorter::reverse (int lo, int hi) {
    int len = (hi - lo) / 2 ;
    for (int i = 0; i < len; i++) {
        BREAKPOINT ;
        m_link->flip(lo+i, hi-1-i) ;
    }
}

void Sorter::rotate (int lo, int hi, int qty) {
    BREAKPOINT ;
    reverse(lo, hi) ;
    reverse(hi-qty, hi) ;
    reverse(lo, hi-qty) ;
}

void Sorter::Sampler () {
    int sampleSize = std::ceil(std::sqrt(m_size)) ;
    if (m_size < 16) {
        LocalInsertSort(0, m_size) ;
        for (int i = 0; i < m_size; i++) m_link->solved(i) ;
        return ;
    }
    for (int i = 0; i < sampleSize; i++) {
        BREAKPOINT ;
        m_link->flip(i, std::min(i*sampleSize, m_size-1)) ;
    }
    LocalInsertSort(0, sampleSize) ;
    int access [sampleSize+1] ;
    for (int i = 0; i <= sampleSize; i++) {
        BREAKPOINT ;
        access[i] = 0 ;
    }
    // Count members of each interval
    for (int i = sampleSize; i < m_size; i++) {
        BREAKPOINT ;
        access[Dichotomy(m_link->send(i), 0, sampleSize+1)]++ ;
    }
    // Turn into cumulative
    for (int j = 1; j <= sampleSize; j++) {
        BREAKPOINT ;
        access[j] += access[j-1] ;
    }
    // Copy in order then rewrite
    item * cpy [m_size - sampleSize] ;
    for (int i = m_size-1; i >= sampleSize; i--) {
        BREAKPOINT ;
        cpy[--access[Dichotomy(m_link->send(i), 0, sampleSize+1)]] = m_link->send(i) ;
    }
    for (int i = sampleSize; i < m_size; i++) {
        BREAKPOINT ;
        m_link->receive(i, cpy[i - sampleSize]) ;
    }
    // Sub-sort
    for (int i = 0; i < sampleSize; i++) {
        BREAKPOINT ;
        LocalInsertSort(sampleSize+access[i], sampleSize+access[i+1]) ;
    }
    LocalInsertSort(sampleSize+access[sampleSize], m_size) ;
    BREAKPOINT ;
    int lo = 0 ;
    int qty = sampleSize ;
    int hi = sampleSize ;
    for (;;) {
        while (hi < m_size && qty > 0 && m_link->cmp_ii(hi, lo)) {
            BREAKPOINT ;
            hi++ ;
        }
        rotate(lo, hi, qty) ;
        lo = hi - (--qty) ;
        if (hi == m_size || qty == 0) break ;
    }
    BREAKPOINT ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Bucket () {
    int bucketcount = std::ceil(std::sqrt(m_size)) ;
    if (m_size < 16) {
        LocalInsertSort(0, m_size) ;
        return ;
    }
    int access [bucketcount+1] ;
    for (int i = 0; i <= bucketcount; i++) {
        BREAKPOINT ;
        access[i] = 0 ;
    }
    // Count members of each interval
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        access[m_link->get(i).val / bucketcount]++ ;
    }
    // Turn into cumulative
    for (int j = 1; j <= bucketcount; j++) {
        BREAKPOINT ;
        access[j] += access[j-1] ;
    }
    // Copy in order then rewrite
    item * cpy [m_size] ;
    for (int i = m_size-1; i >= 0; i--) {
        BREAKPOINT ;
        cpy[--access[m_link->get(i).val / bucketcount]] = m_link->send(i) ;
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->receive(i, cpy[i]) ;
    }
    // Sub-sort
    for (int i = 0; i < bucketcount; i++) {
        BREAKPOINT ;
        LocalInsertSort(access[i], access[i+1]) ;
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

// /////////// //
// INSERT-TYPE //
// /////////// //

void Sorter::Insertion () {
    for (int i = 1; i < m_size; i++) {
        int j = i ;
        item * val = m_link->send(j) ;
        while ((j > 0) && (m_link->cmp_ei(val, j-1))) {
            BREAKPOINT ;
            m_link->flip(j, j-1) ;
            m_link->tmpsolve(j) ;
            j-- ;
        }
        m_link->tmpsolve(j) ;
        if (j > 0) m_link->tmpsolve(j-1) ;
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Shell () {
    int gaps [8] = {701, 301, 132, 57, 23, 10, 4, 1} ;
    for (int g = 0; g < 8; g++) {
        int gap = gaps[g] ;
        for (int i = gap; i < m_size; i++) {
            item * val = m_link->send(i) ;
            for (int j = i; j >= gap && m_link->cmp_ei(val, j-gap); j -= gap) {
                BREAKPOINT ;
                m_link->flip(j, j-gap) ;
            }
        }
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Gnomesort () {
    int i = 0 ;
    while (i < m_size-1) {
        BREAKPOINT ;
        if (i >= 0) {
            if (!m_link->cmp_ii(i, i+1)) {
                m_link->flip(i, i+1) ;
                i-- ;
            } else {
                m_link->tmpsolve(i++) ;
            }
        } else {
            i++ ;
        }
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

// ////////////////////////////////////////////////// //
// DEBUG /////////////////////////////////////////// //
// //////////////////////////////////////////////// //

int Sorter::Dichotomy (item x, int lt, int rt) {
    // tab[lt] <= x < tab[rt]
    BREAKPOINT 0 ;
    if (rt - lt <= 0) {
        return lt ;
    } else if (rt - lt == 1) {
        return rt ;
    }  else {
        int m = (rt + lt) / 2 ;
        if (!m_link->cmp_ei(&x, m)) {
            m_link->tmpsolve(m) ;
            return Dichotomy(x, m, rt) ;
        } else {
            m_link->tmpsolve(m) ;
            return Dichotomy(x, lt, m) ;
        }
    }
}

void Sorter::BinInsertion () {
    m_link->flip_ifn_cmp(0, 1) ;
    for (int i = 2; i < m_size; i++) {
        BREAKPOINT ;
        item x = m_link->get(i) ;
        int pos = Dichotomy(x, 0, i) ;
        if (pos < i) {
            for (int j = i; j > pos; j--) {
                BREAKPOINT ;
                m_link->set(j, m_link->get(j-1)) ;
                m_link->tmpsolve(j) ;
            }
            m_link->set(pos, x) ;
            m_link->flip_ifn_cmp(pos-1, pos) ;
            m_link->tmpsolve(pos-1) ;
            m_link->tmpsolve(pos) ;
        }
        m_link->tmpsolve(i) ;
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}

// ////////////////////////////////////////////////// //
// !DEBUG ////////////////////////////////////////// //
// //////////////////////////////////////////////// //


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
        BREAKPOINT ;
        Reheapify_max(maxson, end) ;
    } else {
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(maxson) ;
    }
}

void Sorter::Reheapify_sift (int i) {
    if (i == 0) return ;
    int father = (i-1)/2 ;
    if (m_link->cmp_ii(father, i)) {
        BREAKPOINT ;
        m_link->flip(i, father) ;
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(father) ;
        Reheapify_sift(father) ;
    } else {
        m_link->setBinHeapLv(i) ;
        m_link->setBinHeapLv(father) ;
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
        BREAKPOINT ;
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
        BREAKPOINT ;
        Reheapify_tern(maxson, end) ;
    }else {
        m_link->setTerHeapLv(i) ;
        m_link->setTerHeapLv(maxson) ;
    }
}

void Sorter::Heapsort_max () {
    for (int i = m_size / 2 + 1; i >= 0; i--) {
        BREAKPOINT ;
        Reheapify_max(i, m_size) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        BREAKPOINT ;
        m_link->flip(i, 0) ;
        m_link->solved(i) ;
        Reheapify_max(0, i) ;
        BREAKPOINT ;
    }
}

void Sorter::Heapsort_sift () {
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        Reheapify_sift(i) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        BREAKPOINT ;
        m_link->flip(i, 0) ;
        m_link->solved(i) ;
        Reheapify_max(0, i) ;
        BREAKPOINT ;
    }
}

void Sorter::Heapsort_min () {
    for (int i = m_size / 2 + 1; i >= 0; i--) {
        BREAKPOINT ;
        Reheapify_min(i, m_size) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        BREAKPOINT ;
        m_link->flip(i, 0) ;
        m_link->tmpsolve(i) ;
        Reheapify_min(0, i) ;
        BREAKPOINT ;
    }
    for (int i = 0; i <= m_size/2; i++) {
        BREAKPOINT ;
        m_link->flip(i, m_size-1-i) ;
        m_link->solved(i) ;
        m_link->solved(m_size-1-i) ;
    }
    m_link->solved(m_size/2+1) ;
}

void Sorter::Heapsort_tern () {
    for (int i = m_size / 3 + 2; i >= 0; i--) {
        BREAKPOINT ;
        Reheapify_tern(i, m_size) ;
    }
    for (int i = m_size-1; i >= 0; i--) {
        BREAKPOINT ;
        m_link->flip(i, 0) ;
        m_link->solved(i) ;
        Reheapify_tern(0, i) ;
        BREAKPOINT ;
    }
}


#define UP(IA,IB) temp = IA; IA += IB + 1; IB = temp
#define DOWN(IA,IB) temp = IB; IB = IA - IB - 1; IA = temp

static int q, r, p, b, c, r1, b1, c1;

void Sorter::Sift () {
    int r0, r2, temp ;
    item * t ;
    r0 = r1 ;
    t = m_link->send(r0) ;
    while (b1 >= 3) {
        BREAKPOINT ;
        r2 = r1 - b1 + c1 ;
        if (!m_link->cmp_ii(r1 - 1, r2)) {
            r2 = r1 - 1 ;
            DOWN(b1, c1) ;
        }
        if (m_link->cmp_ie(r2, t)) {
            b1 = 1 ;
        } else {
            m_link->receive(r1, m_link->send(r2)) ;
            r1 = r2 ;
            DOWN(b1, c1) ;
        }
    }
    if (r1 - r0) m_link->receive(r1, t) ;
}

void Sorter::Trinkle () {
    int p1, r2, r3, r0, temp ;
    item * t ;
    p1 = p ;
    b1 = b ;
    c1 = c ;
    r0 = r1 ;
    t = m_link->send(r0) ;
    while (p1 > 0) {
        BREAKPOINT ;
        while ((p1 & 1) == 0) {
            BREAKPOINT ;
            p1 >>= 1 ;
            UP(b1, c1) ;
        }
        r3 = r1 - b1 ;
        if ((p1 == 1) || m_link->cmp_ie(r3, t)) {
            p1 = 0 ;
        } else {
            --p1 ;
            if (b1 == 1) {
                m_link->receive(r1, m_link->send(r3)) ;
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
                        m_link->receive(r1, m_link->send(r3));
                        r1 = r3 ;
                    } else {
                        m_link->receive(r1, m_link->send(r2)) ;
                        r1 = r2 ;
                        DOWN(b1, c1) ;
                        p1 = 0 ;
                    }
                }
            }
        }
    }
    if (r0 - r1) m_link->receive(r1, t) ;
    BREAKPOINT ;
    Sift() ;
}

void Sorter::SemiTrinkle () {
    r1 = r - c ;
    if (!m_link->cmp_ii(r1, r)) {
        m_link->flip(r, r1) ;
        BREAKPOINT ;
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
        BREAKPOINT ;
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
                BREAKPOINT ;
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
        BREAKPOINT ;
        m_link->solved(--q) ;
        if (b == 1) {
            r-- ;
            p-- ;
            while ((p & 1) == 0) {
                BREAKPOINT ;
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
                BREAKPOINT ;
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
        item * MVal = m_link->send(M) ;
        for (int j = 1; j < i; j++) {
            BREAKPOINT ;
            if (m_link->cmp_ei(MVal, j)) {
                M = j ;
                MVal = m_link->send(j) ;
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
        item * mVal = m_link->send(m) ;
        for (int j = i; j < m_size; j++) {
            BREAKPOINT ;
            if (m_link->cmp_ie(j, mVal)) {
                m = j ;
                mVal = m_link->send(j) ;
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
        BREAKPOINT ;
        int m = i, M = i+1 ;
        item * mVal = m_link->send(m) ;
        item * MVal = m_link->send(M) ;
        if (m_link->cmp_ii(M, m)) {
            m++ ; M-- ;
        }
        for (int j = i; j <= m_size-1-i; j++) {
            BREAKPOINT ;
            if (m_link->cmp_ie(j, mVal)) {
                m = j ;
                mVal = m_link->send(j) ;
            } else if (m_link->cmp_ei(MVal, j)) {
                M = j ;
                MVal = m_link->send(j) ;
            }
        }
        BREAKPOINT ;
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
        BREAKPOINT ;
        if (start > 0) m_link->solved(start-1) ;
        item * k = m_link->send(start) ;
        int pos = start ;
        for (int i = start+1; i < m_size; i++) {
            BREAKPOINT ;
            if (m_link->cmp_ie(i, k)) pos++ ;
        }
        if (pos == start) continue ;
        while (k == m_link->send(pos)) pos++ ;
        item * tmp = m_link->send(pos) ;
        m_link->receive(pos, k) ;
        m_link->solved(pos) ;
        k = tmp ;
        while (pos != start) {
            BREAKPOINT ;
            pos = start ;
            for (int i = start+1; i < m_size; i++) {
                BREAKPOINT ;
                if (m_link->cmp_ie(i, k)) pos++ ;
            }
            while (*k == m_link->get(pos)) pos++ ;
            tmp = m_link->send(pos) ;
            m_link->receive(pos, k) ;
            m_link->solved(pos) ;
            k = tmp ;
        }
    }
    m_link->solved(m_size-2) ;
    m_link->solved(m_size-1) ;
}

void Sorter::CycleOpt () {
    bool placed [m_size] ;
    for (int i = 0; i < m_size; i++) placed[i] = false ;
    for (int start = 0; start < m_size-1; start++) {
        BREAKPOINT ;
        if (start > 0) m_link->solved(start-1) ;
        if (placed[start]) continue ;
        item * k = m_link->send(start) ;
        int pos = start ;
        for (int i = start+1; i < m_size; i++) {
            BREAKPOINT ;
            if (m_link->cmp_ie(i, k)) pos++ ;
        }
        if (pos == start) continue ;
        while (*k == m_link->get(pos)) pos++ ;
        item * tmp = m_link->send(pos) ;
        m_link->receive(pos, k) ;
        m_link->solved(pos) ;
        placed[pos] = true ;
        k = tmp ;
        while (pos != start) {
            BREAKPOINT ;
            pos = start ;
            for (int i = start+1; i < m_size; i++) {
                BREAKPOINT ;
                if (m_link->cmp_ie(i, k)) pos++ ;
            }
            while (*k == m_link->get(pos)) pos++ ;
            tmp = m_link->send(pos) ;
            m_link->receive(pos, k) ;
            m_link->solved(pos) ;
            placed[pos] = true ;
            k = tmp ;
        }
    }
    m_link->solved(m_size-2) ;
    m_link->solved(m_size-1) ;
}

void Sorter::Pancake () {
    for (int i0 = 0; i0 < m_size; i0++) {
        int best = i0 ;
        for (int i = i0; i < m_size; i++) {
            BREAKPOINT ;
            if (m_link->cmp_ii(i, best)) {
                best = i ;
            }
        }
        for (int i = 0; i < (m_size-best)/2; i++) {
            BREAKPOINT ;
            m_link->flip(best+i, m_size-1-i) ;
        }
        for (int i = 0; i < (m_size-i0)/2; i++) {
            BREAKPOINT ;
            m_link->flip(i0+i, m_size-1-i) ;
        }
        m_link->solved(i0) ;
    }
}


// //////// //
// NETWORKS //
// //////// //

void Sorter::Bitonic () {
    BREAKPOINT ;
    Bitonic_merge(0, m_size) ;
    BREAKPOINT ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Bitonic_sort (int a, int b) {
    int m = (b - a) / 2 ;
    if (b-a == 2) {
        m_link->flip_ifn_cmp(a, a+1) ;
        return ;
    }
    for (int i = 0; i < m; i++) {
        BREAKPOINT ;
        m_link->flip_ifn_cmp(a+i, a+m+i) ;
    }
    BREAKPOINT ;
    Bitonic_sort(a, a+m) ;
    BREAKPOINT ;
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
        BREAKPOINT ;
        m_link->flip_ifn_cmp(a+i, b-1-i) ;
    }
    BREAKPOINT ;
    Bitonic_sort(a, a+m) ;
    BREAKPOINT ;
    Bitonic_sort(a+m, b) ;
}

void Sorter::OddEven () {
    BREAKPOINT ;
    OddEven_sort(0, m_size) ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::OddEven_sort (int a, int b) {
    if (b-a <= 1) return ;
    int m = a + (b-1-a)/2 ;
    BREAKPOINT ;
    OddEven_sort(a, m+1) ;
    BREAKPOINT ;
    OddEven_sort(m+1, b) ;
    BREAKPOINT ;
    OddEven_merge(a, b, 1) ;
}

void Sorter::OddEven_merge (int a, int b, int r) {
    int step = 2 * r ;
    if (step < b-1-a) {
        BREAKPOINT ;
        OddEven_merge(a, b, step) ;
        BREAKPOINT ;
        OddEven_merge(a + r, b, step) ;
        for (int i = a + r; i < b - 1 - r; i += step) {
            BREAKPOINT ;
            m_link->flip_ifn_cmp(i, i + r) ;
        }
    } else {
        m_link->flip_ifn_cmp(a, a + r) ;
    }
}



void Sorter::BitonicPara () {
    for (int lv = 1; lv < m_size; lv <<= 1) {
        for (int offset = 0; offset < lv; offset++) {
            for (int group = 0; group*lv*2 < m_size; group++) {
                BREAKPOINT ;
                m_link->flip_ifn_cmp(lv*group*2+offset, lv*(group+1)*2-1-offset) ;
                //printf("%d %d\n", lv*group*2+offset, lv*(group+1)*2-1-offset) ;
            }
        }
        for (int decr = lv/2; decr > 0; decr >>= 1) {
            for (int offset = 0; offset < decr; offset++) {
                for (int group = 0; group*decr*2 < m_size; group++) {
                    BREAKPOINT ;
                    m_link->flip_ifn_cmp(decr*group*2+offset, decr*(group*2+1)+offset) ;
                    //printf("%d %d\n", decr*group*2+offset, decr*(group*2+1)+offset) ;
                }
            }
        }
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::OddEvenPara () {
    for (int lv = 1; lv < m_size; lv <<= 1) {
        for (int offset = 0; offset < lv; offset++) {
            for (int group = 0; group*lv*2 < m_size; group++) {
                BREAKPOINT ;
                m_link->flip_ifn_cmp(lv*group*2+offset, lv*(group*2+1)+offset) ;
                //printf("%d %d\n", lv*group*2+offset, lv*(group*2+1)+offset) ;
            }
        }
        for (int decr = lv/2; decr > 0; decr >>= 1) {
            for (int group = 0; group*lv*2 < m_size; group++) {
                for (int tailing = 0; (tailing+1)*decr*2 < lv*2; tailing++) {
                    for (int offset = 0; offset < decr; offset++) {
                        BREAKPOINT ;
                        m_link->flip_ifn_cmp(lv*group*2+decr*(tailing*2+1)+offset, lv*group*2+decr*(tailing+1)*2+offset) ;
                        //printf("%d %d\n", decr*(group*2+1)+offset, decr*(group+1)*2+offset) ;
                    }
                }
            }
        }
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

int access_shear (int N, int n, int i, int j) {
    int k ;
    if (i % 2 == 0) {
        k = n * i + j ;
    } else {
        k = n * i + n - j - 1 ;
    }
    //printf("%d\n", k) ;
    return k < N ? k : -1 ;
}

bool Sorter::shear_line (int N, int n, int i) {
    bool useful = false ;
    if (i % 2 == 0) {
        for (int j = 0;  j < n && access_shear(N, n, i, j) != -1; j++) {
            int k = j ;
            while ((k > 0) && (m_link->cmp_ii(access_shear(N, n, i, k), access_shear(N, n, i, k-1)))) {
                BREAKPOINT false ;
                useful = true ;
                m_link->flip(access_shear(N, n, i, k), access_shear(N, n, i, k-1)) ;
                k-- ;
            }
        }
    } else {
        for (int j = 0;  j < n && access_shear(N, n, i, j) != -1; j++) {
            int k = j ;
            while ((k > 0) && (m_link->cmp_ii(access_shear(N, n, i, k-1), access_shear(N, n, i, k)))) {
                BREAKPOINT false ;
                useful = true ;
                m_link->flip(access_shear(N, n, i, k), access_shear(N, n, i, k-1)) ;
                k-- ;
            }
        }
    }
    return useful ;
}

bool Sorter::shear_col (int N, int n, int j) {
    bool useful = false ;
    for (int i = 0; i < n && access_shear(N, n, i, j) != -1; i++) {
        int k = i ;
        while ((k > 0) && (m_link->cmp_ii(access_shear(N, n, k, j), access_shear(N, n, k-1, j)))) {
            BREAKPOINT false ;
            useful = true ;
            m_link->flip(access_shear(N, n, k, j), access_shear(N, n, k-1, j)) ;
            k-- ;
        }
    }
    return useful ;
}


void Sorter::ShearSort () {
    bool keepGoing = true ;
    int N = m_size ;
    int n = (int)std::ceil(std::sqrt(N)) ;
    while (keepGoing) {
        BREAKPOINT ;
        keepGoing = false ;
        for (int i = 0; i < n; i++) {
            keepGoing |= shear_line(N, n, i) ;
            BREAKPOINT ;
        }
        for (int j = 0; j < n; j++) {
            keepGoing |= shear_col(N, n, j) ;
            BREAKPOINT ;
        }
    }
    for (int i = 0; i < m_size; i++) m_link->solved(i) ;
}



// ////// //
// STUPID //
// ////// //

void Sorter::Slowsort () {
    Slowmerge(0, m_size) ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Slowmerge (int a, int b) {
    if (b - a <= 1) return ;
    int m = (a + b) / 2 ;
    BREAKPOINT ;
    Slowmerge(a, m) ;
    BREAKPOINT ;
    Slowmerge(m, b) ;
    BREAKPOINT ;
    m_link->flip_ifn_cmp(a, m) ;
    m_link->tmpsolve(a) ;
    Slowmerge(a+1, b) ;
}

void Sorter::Stoogesort () {
    Stooge(0, m_size) ;
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Stooge (int a, int b) {
    if (b - a <= 1) return ;
    if (b - a == 2) {
        m_link->flip_ifn_cmp(a, a+1) ;
    } else if (b - a == 3) {
        BREAKPOINT ;
        Stooge(a, a+2) ;
        BREAKPOINT ;
        Stooge(a+1, a+3) ;
        BREAKPOINT ;
        Stooge(a, a+2) ;
    } else {
        int tercile1 = std::floor(a*0.67 + b*0.33), tercile2 = std::ceil(a*0.33 + b*0.67) ;
        BREAKPOINT ;
        Stooge(a, tercile2) ;
        BREAKPOINT ;
        Stooge(tercile1, b) ;
        BREAKPOINT ;
        Stooge(a, tercile2) ;
    }
}

void Sorter::Bogosort () {
    for (;;) {
        Scramble(0, m_size) ;
        for (int i = 1; i < m_size; i++) {
            BREAKPOINT ;
            if (!m_link->cmp_ii(i-1, i)) goto BREAK ;
        }
        break ;
        BREAK: continue ;
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::LessBogosort () {
    int I = 0 ;
    while (I < m_size) {
        Scramble(I, m_size) ;
        for (int i = I+1; i < m_size; i++) {
            BREAKPOINT ;
            if (!m_link->cmp_ii(I, i)) goto BREAK ;
        }
        m_link->solved(I++) ;
        BREAK: continue ;
    }
}

void Sorter::Randsort () {
    for (;;) {
        int i = std::rand()%m_size ;
        int j = std::rand()%m_size ;
        m_link->flip_ifn_cmp(std::min(i, j), std::max(i, j)) ;
        for (int i = 1; i < m_size; i++) {
            BREAKPOINT ;
            if (m_link->cmp_ii(i, i-1)) goto BREAK ;
        }
        break ;
        BREAK: continue ;
    }
    for (int i = 0; i < m_size; i++) {
        BREAKPOINT ;
        m_link->solved(i) ;
    }
}

void Sorter::Scramble(int a, int b) {
    for (int i = a; i < b; i++) {
        BREAKPOINT ;
        m_link->flip(i, i+(std::rand()%(b-i))) ;
    }
}

// EXTENSIONS TO ADD :
/* Recursive bucket- and sample-sort
 */
