#include <QApplication>
#include <QRectF>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QPalette>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

#include "window.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    auto mainWindow = new QWidget ;
    mainWindow->setFixedSize(820, 620) ;

    auto scene = new QGraphicsScene() ;
    auto view = new QGraphicsView(scene) ;

    mainWindow->setLayout(new QHBoxLayout) ;
    auto menu = new Window (mainWindow, scene, view) ;
    menu->setFixedHeight(590) ;
    auto frame = new QFrame;
    frame->setLayout(new QVBoxLayout);
    mainWindow->layout()->addWidget(frame) ;
    mainWindow->layout()->addWidget(menu) ;

    view->setFixedSize(600, 580) ;
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
    view->setScene(scene) ;
    //view->fitInView(0, 0, 500, 500, Qt::KeepAspectRatio) ;
    frame->layout()->addWidget(view) ;

    mainWindow->show();

    return app.exec();
}
