#include "window.h"

Window::Window (QWidget * parent, QGraphicsScene * scene, QGraphicsView * view) {
    setParent(parent) ;
    m_parent = parent ;
    m_scene = scene ;
    m_view = view ;
    show() ;
    setFixedSize(160, 500) ;
    m_array = new int [0] ;
    move(550, 10) ;
    m_chooseAlgo = new QComboBox (this) ;
    m_chooseAlgo->addItem("Bubblesort") ;
#ifndef LIMITED
    m_chooseAlgo->addItem("Quicksort") ;
    m_chooseAlgo->addItem("Mergesort") ;
    m_chooseAlgo->addItem("Radix sort") ;
    m_chooseAlgo->addItem("Bucket sort") ;
    m_chooseAlgo->addItem("Insertion sort") ;
    m_chooseAlgo->addItem("Heapsort") ;
    m_chooseAlgo->addItem("Selection sort") ;
    m_chooseAlgo->addItem("Sorting Networks") ;
#endif
    m_chooseOption = new QComboBox (this) ;
    m_optmgr = new OptionMgr (m_chooseOption) ;
    m_optmgr->connect(m_chooseAlgo, SIGNAL(currentIndexChanged(int)), m_optmgr, SLOT(updateOptions(int))) ;
    m_chooseAlgo->setCurrentIndex(1) ;
    m_chooseAlgo->setCurrentIndex(0) ; // To force a refresh
//    m_chooseDelay = new QComboBox (this) ;
//    m_chooseDelay->addItem("None") ;
//    m_chooseDelay->addItem("1") ;
//    m_chooseDelay->addItem("2") ;
//    m_chooseDelay->addItem("5") ;
//    m_chooseDelay->addItem("10") ;
//    m_chooseUnit = new QComboBox (this) ;
//    m_chooseUnit->addItem("x1 ms") ;
//    m_chooseUnit->addItem("x10 ms") ;
//    m_chooseUnit->addItem("x100 ms") ;
    QVector<int> delayVals = {0, 1, 2, 3, 5, 7, 9} ;
    QVector<int> unitVals = {1, 10, 100} ;
    m_chooseDelay = new XSlider (delayVals, unitVals) ;
    m_chooseDistrib = new QComboBox (this) ;
    m_chooseDistrib->addItem("Regular") ;
    m_chooseDistrib->addItem("Uniform (random)") ;
#ifndef LIMITED
    m_chooseDistrib->addItem("Centered (random)") ;
    m_chooseDistrib->addItem("Split (random)") ;
    m_chooseDistrib->addItem("High (random)") ;
    m_chooseDistrib->addItem("Low (random)") ;
#endif
    m_chooseScramble = new QComboBox (this) ;
    m_chooseScramble->addItem("Random") ;
    m_chooseScramble->addItem("Sorted-ish") ;
#ifndef LIMITED
    m_chooseScramble->addItem("Reversed-ish") ;
    m_chooseScramble->addItem("Shuffle") ;
#endif
    m_chooseSize = new QComboBox (this) ;
    m_chooseSize->addItem("8") ;
    m_chooseSize->addItem("16") ;
    m_chooseSize->addItem("32") ;
    m_chooseSize->addItem("64") ;
#ifndef LIMITED
    m_chooseSize->addItem("128") ;
    m_chooseSize->addItem("256") ;
    m_chooseSize->addItem("512") ;
    m_chooseSize->addItem("1024") ;
    m_chooseSize->connect(m_chooseSize, SIGNAL(currentIndexChanged(int)), this, SLOT(Resize(int))) ;
//    QVector<int> sizeVals = {8, 16, 32, 64, 128, 256, 512, 1024} ;
//    m_chooseSize = new XSlider (sizeVals) ;
//    m_chooseSize->connect(m_chooseSize, SIGNAL(valChanged(int)), this, SLOT(Resize(int))) ;
#endif
    m_nbCmp = new QLCDNumber (this) ;
    m_nbCmp->setDigitCount(10) ;
    m_nbRead = new QLCDNumber (this) ;
    m_nbRead->setDigitCount(10) ;
    m_nbWrite = new QLCDNumber (this) ;
    m_nbWrite->setDigitCount(10) ;
    m_start = new QPushButton ("Start !", this) ;
    m_exit = new QPushButton ("Exit", this) ;
    m_optmgr->connect(m_exit, SIGNAL(clicked()), m_parent, SLOT(close())) ;
    m_chooseMenu = new QVBoxLayout ;
    m_chooseMenu->addWidget(new QLabel ("Algorithm:")) ;
    m_chooseMenu->addWidget(m_chooseAlgo) ;
    m_chooseMenu->addWidget(new QLabel ("Specification:")) ;
    m_chooseMenu->addWidget(m_chooseOption) ;
    m_chooseMenu->addWidget(new QLabel ("Delay:")) ;
    m_chooseMenu->addWidget(m_chooseDelay) ;
    m_chooseMenu->addWidget(new QLabel ("Size:")) ;
    m_chooseMenu->addWidget(m_chooseSize) ;
    m_chooseMenu->addWidget(new QLabel ("Distribution:")) ;
    m_chooseMenu->addWidget(m_chooseDistrib) ;
    m_chooseMenu->addWidget(new QLabel ("Scramble:")) ;
    m_chooseMenu->addWidget(m_chooseScramble) ;
    m_chooseMenu->addWidget(new QLabel ("Comparisons")) ;
    m_chooseMenu->addWidget(m_nbCmp) ;
    m_chooseMenu->addWidget(new QLabel ("Accesses (read)")) ;
    m_chooseMenu->addWidget(m_nbRead) ;
    m_chooseMenu->addWidget(new QLabel ("Accesses (write)")) ;
    m_chooseMenu->addWidget(m_nbWrite) ;
    m_chooseMenu->addWidget(m_start) ;
    m_chooseMenu->addWidget(m_exit) ;
    setLayout(m_chooseMenu) ;

    setWindowTitle("Sorts -- View") ;
    m_optmgr->connect(m_start, SIGNAL(clicked()), this, SLOT(run())) ;
}

void Window::run () {
//    printf("Clicked !") ;
    m_start->setText("") ;
    m_start->setEnabled(false) ;
    int size = (1 << (m_chooseSize->currentIndex()+3)) ;
//    printf("%d\n", size) ;
    delete[] m_array ;
    m_array = new int [size] ;
    Distributor * distr = new Distributor (size, m_array) ;
    m_chooseAlgo->setEnabled(false) ;
    m_chooseOption->setEnabled(false) ;
    m_chooseDelay->setEnabled(false) ;
    m_chooseSize->setEnabled(false) ;
    m_chooseDistrib->setEnabled(false) ;
    m_chooseScramble->setEnabled(false) ;
    switch (m_chooseDistrib->currentIndex()) {
        case 0: distr->Dregular() ; break ;
        case 1: distr->Duniform() ; break ;
#ifndef LIMITED
        case 2: distr->Dcentered() ; break ;
        case 3: distr->Dsplit() ; break ;
        case 4: distr->Dhigh() ; break ;
        case 5: distr->Dlow() ; break ;
#endif
    }
    switch (m_chooseScramble->currentIndex()) {
        case 0: distr->Srandom() ; break ;
        case 1: distr->Ssorted() ; break ;
#ifndef LIMITED
        case 2: distr->Sreversed() ; break ;
        case 3: distr->Sshuffle() ; break ;
#endif
    }
    int delay = m_chooseDelay->getval()*1000 ;

//    printf("Create Link \n") ;
    m_link = new Linker (this, m_array, size, m_scene, delay) ;
    m_sort = new Sorter (m_link) ;

    switch (m_chooseAlgo->currentIndex()) {
        case 0: // Bubblesort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Bubble_upwards() ; break ;
                case 1: m_sort->Bubble_downwards() ; break ;
                case 2: m_sort->Bubble_shaker() ; break ;
                case 3: m_sort->Comb() ; break ;
            } break ;
#ifndef LIMITED
        case 1: // Quicksort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Quick_naive() ; break ;
                case 1: m_sort->Quick_middle() ; break ;
                case 2: m_sort->Quick_optimized() ; break ;
            } break ;
        case 2: // Mergesort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Mergesort_standard() ; break ;
                case 1: m_sort->Mergesort_pack() ; break ;
                case 2: m_sort->Mergesort_weave() ; break ;
            } break ;
        case 3: // Radix sort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->RadixLSD2() ; break ;
                case 1: m_sort->RadixLSD4() ; break ;
                case 2: m_sort->RadixLSD8() ; break ;
                case 3: m_sort->RadixMSD2() ; break ;
                case 4: m_sort->RadixMSD4() ; break ;
                case 5: m_sort->RadixMSD8() ; break ;
                case 6: m_sort->Counting() ; break ;
            } break ;
        case 4: // Sample sort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Bucket() ; break ;
                case 1: m_sort->Sampler() ; break ;
            } break ;
        case 5: // Insertion sort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Insertion() ; break ;
                case 1: m_sort->Shell() ; break ;
            } break ;
        case 6: // Heapsort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Heapsort_max() ; break ;
                case 1: m_sort->Heapsort_min() ; break ;
                case 2: m_sort->Heapsort_tern() ; break ;
                case 3: m_sort->SmoothSort() ; break ;
            } break ;
        case 7: // Selection sort
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Selection_max() ; break ;
                case 1: m_sort->Selection_min() ; break ;
                case 2: m_sort->Selection_dbl() ; break ;
                case 3: m_sort->Cycle() ; break ;
                case 4: m_sort->CycleOpt() ; break ;
                case 5: m_sort->Pancake() ; break ;
            } break ;
        case 8: // Network
            switch (m_chooseOption->currentIndex()) {
                case 0: m_sort->Bitonic() ; break ;
                case 1: m_sort->OddEven() ; break ;
            } break ;
#endif
    }

    m_start->setText("Done !") ;
    m_start->setEnabled(true) ;
    m_optmgr->disconnect(m_start, SIGNAL(clicked()), this, SLOT(run())) ;
    m_optmgr->connect(m_start, SIGNAL(clicked()), this, SLOT(done())) ;
}

void Window::done () {
    delete m_link ;
    m_chooseAlgo->setEnabled(true) ;
    m_chooseOption->setEnabled(true) ;
    m_chooseDelay->setEnabled(true) ;
    m_chooseSize->setEnabled(true) ;
    m_chooseDistrib->setEnabled(true) ;
    m_chooseScramble->setEnabled(true) ;
    m_nbCmp->display(0) ;
    m_nbRead->display(0) ;
    m_nbWrite->display(0) ;
    m_start->setText("Start !") ;
    m_start->setEnabled(true) ;
    m_optmgr->disconnect(m_start, SIGNAL(clicked()), this, SLOT(done())) ;
    m_optmgr->connect(m_start, SIGNAL(clicked()), this, SLOT(run())) ;
}

void Window::INCRCMP () {
    m_nbCmp->display(int(m_nbCmp->value()) + 1) ;
}

void Window::INCRREAD () {
    m_nbRead->display(int(m_nbRead->value()) + 1) ;
}

void Window::INCRWRITE () {
    m_nbWrite->display(int(m_nbWrite->value()) + 1) ;
}

void Window::Resize (int s) {
    if (s == 7) {
        m_parent->setFixedSize(1310, 620) ;
        m_view->setFixedSize(1100, 580) ;
        m_view->setSceneRect(0, 0, 1035, 580);
    } else {
        m_parent->setFixedSize(820, 620) ;
        m_view->setFixedSize(600, 580) ;
        m_view->setSceneRect(0, 0, 550, 580);
    }
}
