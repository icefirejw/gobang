#ifndef GOBANG_H
#define GOBANG_H


class Gobang
{
private:
    int row_;
    int column_;
    const int kGridWidth = 40; //棋盘每个格子的大小
    const int kClickPixel = 5; //鼠标点击点离棋盘交叉点的误差范围
    int** positions_;

public:
    explicit Gobang(int row, int column);
    ~Gobang();
};

#endif // GOBANG_H
