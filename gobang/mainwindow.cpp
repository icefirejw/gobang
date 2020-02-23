#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gobang.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Gobang *goBang = new Gobang(64,64,false);
    goBang->PutPieceInChess(3,4);
    goBang->PutPieceInChess(5,3);
    goBang->PutPieceInChess(2,2);

}

MainWindow::~MainWindow()
{
    delete ui;
}
