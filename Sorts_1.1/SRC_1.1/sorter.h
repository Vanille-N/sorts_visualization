#ifndef SORTER_H
#define SORTER_H

#include "linker.h"
#include <math.h>
#include "restrict.h"

class Sorter {
public:
    Sorter (Linker *) ;
    void Bubble_upwards () ;
    void Bubble_downwards () ;
    void Bubble_shaker () ;
    void Comb () ;
#ifndef LIMITED
    void Quick_naive () ;
    void Quick_middle () ;
    void Quick_optimized () ;
    void Mergesort_standard () ;
    void Mergesort_pack () ;
    void Mergesort_weave () ;
    void RadixLSD2 () ;
    void RadixLSD4 () ;
    void RadixLSD8 () ;
    void RadixMSD2 () ;
    void RadixMSD4 () ;
    void RadixMSD8 () ;
    void Counting () ;
    void Insertion () ;
    void Shell () ;
    void Heapsort_max () ;
    void Heapsort_min () ;
    void Heapsort_tern () ;
    void Selection_max () ;
    void Selection_min () ;
    void Selection_dbl () ;
    void Sampler () ;
    void Bucket () ;
    void Bitonic () ;
    void OddEven () ;
    void Cycle () ;
    void CycleOpt () ;
    void Pancake () ;
    void SmoothSort () ;
    void RecSampler () ;
    void RecBucket () ;
#endif
private:
#ifndef LIMITED
    void Quicksort (int, int, int) ;
    int Naive_partition (int, int) ;
    int Middle_partition (int, int) ;
    int Fast_partition (int, int) ;
    void Mergesort (int, int, item * []) ;
    void merge (int, int, int, item * []) ;
    void RadixLSD (int) ;
    void RadixMSDLauncher (int) ;
    void RadixMSD (int, int, int, int, int, int, int) ;
    void Reheapify_max (int, int) ;
    void Reheapify_min (int, int) ;
    void Reheapify_tern (int, int) ;
    void LocalInsertSort (int, int) ;
    int Dichotomy (item *, int, int) ;
    void Weavesort (int, int, item * []) ;
    void weave (int, int, int, item * []) ;
    void Bitonic_sort (int, int) ;
    void Bitonic_merge (int, int) ;
    void OddEven_sort (int, int) ;
    void OddEven_merge (int, int, int) ;
    void Sift () ;
    void Trinkle () ;
    void SemiTrinkle () ;
#endif

    Linker * m_link ;
    int m_size ;
};

#endif // SORTER_H
