#ifndef GOBANG_H
#define GOBANG_H


class Gobang
{
private:
    int rows_;          //棋盘多少行
    int columns_;       //棋盘多少列
    bool current_turn_; //当前下的是黑子（false）还是白字（true）
    bool is_initialized_;
    bool is_ai_;

    int** positions_;

    int WhoWin(int row, int column);
    int InitGobang(int rows, int columns);

public:
    explicit Gobang(int rows, int columns, bool is_ai);
    ~Gobang();

    int MoveInChess(int row, int column);
    int GetCurrentTurn();
    bool GetMode();
    int GetPositionValue(int row, int column);
    int GetRectangle(int row, int column, int *rect);

};

#endif // GOBANG_H
