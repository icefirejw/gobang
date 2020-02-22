#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const int kGridWidth = 40; //棋盘每个格子的大小
    const int kClickPixel = 5; //鼠标点击点离棋盘交叉点的误差范围
};

#endif // MAINWINDOW_H
