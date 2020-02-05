#include "optionmgr.h"

OptionMgr::OptionMgr (QComboBox * menu) {
    m_menu = menu ;
}

void OptionMgr::updateOptions (int algorithm) {
    m_menu->clear() ;
    switch (algorithm) {
        case 0:
            m_menu->addItem("Upwards") ;
            m_menu->setItemData(0, "Ω(n) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Downwards") ;
            m_menu->setItemData(1, "Ω(n) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Cocktail Shaker") ;
            m_menu->setItemData(2, "Ω(n) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Comb sort") ;
            m_menu->setItemData(3, "Ω(n log n) | ??? | O(n²) || O(1)", Qt::ToolTipRole) ;
            break ;
        case 1:
            m_menu->addItem("Naive Partition") ;
            m_menu->setItemData(0, "Ω(n log n) | Θ(n log n) | O(n²) || O(log n)", Qt::ToolTipRole) ;
            m_menu->addItem("Middle Pivot") ;
            m_menu->setItemData(1, "Ω(n log n) | Θ(n log n) | O(n²) || O(log n)", Qt::ToolTipRole) ;
            m_menu->addItem("Best of three") ;
            m_menu->setItemData(2, "Ω(n log n) | Θ(n log n) | O(n log n) (normal cases) || O(log n)", Qt::ToolTipRole) ;
            m_menu->addItem("Median of medians") ;
            m_menu->setItemData(3, "Ω(n log n) | Θ(n log n) | O(n log n) || O(log n)", Qt::ToolTipRole) ;
            break ;
        case 2:
            m_menu->addItem("Standard") ;
            m_menu->setItemData(0, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Bottom-up") ;
            m_menu->setItemData(1, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Weavesort") ;
            m_menu->setItemData(2, "Ω(n log n) | ??? | O(n²) || O(n)", Qt::ToolTipRole) ;
            break ;
        case 3:
            m_menu->addItem("Base 2 LSD") ;
            m_menu->setItemData(0, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Base 4 LSD") ;
            m_menu->setItemData(1, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Base 8 LSD") ;
            m_menu->setItemData(2, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Base 2 MSD") ;
            m_menu->setItemData(3, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Base 4 MSD") ;
            m_menu->setItemData(4, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Base 8 MSD") ;
            m_menu->setItemData(5, "Ω(n log n) | Θ(n log n) | O(n log n) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Counting sort") ;
            m_menu->setItemData(6, "Ω(n) | Θ(n) | O(n) || O(n)", Qt::ToolTipRole) ;
            break ;
        case 4:
            m_menu->addItem("Bucket") ;
            m_menu->setItemData(0, "Ω(n³⸍²) | Θ(n³⸍²) | O(n³⸍²) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Sampler") ;
            m_menu->setItemData(1, "Ω(n³⸍²) | Θ(n³⸍² log n) | O(n³⸍² log n) || O(n)", Qt::ToolTipRole) ;
            break ;
        case 5:
            m_menu->addItem("Standard") ;
            m_menu->setItemData(0, "Ω(n) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Shell sort") ;
            m_menu->setItemData(1, "Ω(n log²n) | Θ(n log²n) | O(n log²n) || O(1)", Qt::ToolTipRole) ;
            break ;
        case 6:
            m_menu->addItem("Max heap") ;
            m_menu->setItemData(0, "Ω(n log n) | Θ(n log n) | O(n log n) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Max heap (variant)") ;
            m_menu->setItemData(1, "Ω(n log n) | Θ(n log n) | O(n log n) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Min heap") ;
            m_menu->setItemData(2, "Ω(n log n) | Θ(n log n) | O(n log n) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Ternary heap") ;
            m_menu->setItemData(3, "Ω(n log n) | Θ(n log n) | O(n log n) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Smoothsort") ;
            m_menu->setItemData(4, "Ω(n) | Θ(n log n) | O(n log n) || O(1)", Qt::ToolTipRole) ;
            break ;
        case 7:
            m_menu->addItem("Select max") ;
            m_menu->setItemData(0, "Ω(n²) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Select min") ;
            m_menu->setItemData(1, "Ω(n²) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Double select") ;
            m_menu->setItemData(2, "Ω(n²) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Cycle sort") ;
            m_menu->setItemData(3, "Ω(n²) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Cycle optimized") ;
            m_menu->setItemData(4, "Ω(n²) | Θ(n²) | O(n²) || O(n)", Qt::ToolTipRole) ;
            m_menu->addItem("Pancake") ;
            m_menu->setItemData(5, "Ω(n²) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole) ;
            break ;
        case 8:
            m_menu->addItem("Bitonic sort") ;
            m_menu->setItemData(0, "Θ(log²n) parallel || Θ(n log²n) sequential", Qt::ToolTipRole) ;
            m_menu->addItem("Odd-Even Mergesort") ;
            m_menu->setItemData(1, "Θ(log²n) parallel || Θ(n log²n) sequential", Qt::ToolTipRole) ;
            m_menu->addItem("Simultaneous Bitonic sort") ;
            m_menu->setItemData(2, "Θ(log²n) parallel || Θ(n log²n) sequential", Qt::ToolTipRole) ;
            m_menu->addItem("Simultaneous Odd-Even Mergesort") ;
            m_menu->setItemData(3, "Θ(log²n) parallel || Θ(n log²n) sequential", Qt::ToolTipRole) ;
            break ;
        case 9:
            m_menu->addItem("Slowsort") ;
            m_menu->setItemData(0, "Ω(n^(log n / 2)) | Θ(n^(log n / 2)) | O(n^(log n / 2)) || O(log n)", Qt::ToolTipRole) ;
            m_menu->addItem("Stoogesort") ;
            m_menu->setItemData(1, "Ω(n²·⁷¹) | Θ(n²·⁷¹) | O(n²·⁷¹) || O(n²·⁷¹)", Qt::ToolTipRole) ;
            m_menu->addItem("Bogosort") ;
            m_menu->setItemData(2, "Ω(n) | Θ(n·n!) | ∞ || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Less bogo sort") ;
            m_menu->setItemData(3, "Ω(n) | Θ(n³) | ∞ || O(1)", Qt::ToolTipRole) ;
            m_menu->addItem("Randsort") ;
            m_menu->setItemData(4, "Ω(n) | Θ(n³) | ∞ || O(1)", Qt::ToolTipRole) ;
            break ;
    }
}

