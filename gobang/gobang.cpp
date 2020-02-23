#include "gobang.h"

#ifdef DEBUG_GOBANG
#include <QDebug>
#endif

Gobang::Gobang(int rows, int columns, bool is_ai)
{
    int ret = 0;
    // 是人机对战，还是人人对战
    is_ai_ = is_ai;

    // 初始化五子棋资源
    positions_ = nullptr;
    rows_ = 0;
    columns_ = 0;

    ret = InitGobang(rows, columns);
    if (ret < 0){
        BangDebug("Initialized error: %d \n", ret);
    }
    if(is_ai){
        // 如果人机对战需要初始化ai系统

    }

    BangDebug("GoBang Created");

}

Gobang::~Gobang()
{
    int i = 0;

    if (nullptr == positions_ || !is_initialized_){
        return;
    }

    for(i=0; i<rows_; i++){
        if (nullptr != positions_[i])
            delete[] positions_[i];
    }
    if (nullptr != positions_)
        delete[] positions_;

    rows_ = 0;
    columns_ = 0;
}

/*
 * 初始化
*/
int Gobang::InitGobang(int rows, int columns)
{
    int i;
    int j;

    is_initialized_ = false;

    current_turn_ = false; // 初始化黑子开始

    // 初始化棋局，分配落子位置数组，并初始化

    if (nullptr == positions_){
        positions_ = new int*[rows];
        if (nullptr == *positions_)
            return -1;

        for(i =0; i<rows; i++){
            positions_[i] = new int[columns];
            if (nullptr == positions_[i])
                return -2;
        }
        rows_ = rows;
        columns_ = columns;
    }

    for (i=0; i<rows_;i++)
        for (j=0;j<columns_;j++)
            positions_[i][j] = 0;

    is_initialized_ = true;

    return 0;
}

/*
 * 判断输赢
 * 如果有人赢了：则返回1；
 * 如果没有   ：返回0；
 * 如果有错误  ：返回负数；
*/
int Gobang::IsWin(int row, int column)
{
    int i; //行
    int j; //列
    int left;
    int right;
    int top;
    int bottom;
    int rect[4];
    int is_win = false;

    int current = 0;//当前落子的颜色，白色为1，黑色为-1；

    if (!is_initialized_)
        return -1;

    current = GetPositionValue(row, column);
    if (1 != current && -1 != current)
        return -2;

    // 获取当前位置上下左右5个字的矩形范围
    if (GetRectangle(row, column, rect) < 0)
        return -3;

    left  = rect[0];
    right = rect[1];
    top   = rect[2];
    bottom = rect[3];

    // 行是否是5个
    for (j = left; j+4<=right && j<=column; j++ ){
        if((GetPositionValue(row, j) == current) &&
                (GetPositionValue(row, j+1) == current) &&
                (GetPositionValue(row, j+2) == current) &&
                (GetPositionValue(row, j+3) == current) &&
                (GetPositionValue(row, j+4) == current)
                ){
            is_win = true;
            break;
        }
    }

    // 列是否是5个
    for (i = top; i+4<=bottom && i<=row; i++ ){
        if((GetPositionValue(i, column) == current) &&
                (GetPositionValue(i + 1, column) == current) &&
                (GetPositionValue(i + 2, column) == current) &&
                (GetPositionValue(i + 3, column) == current) &&
                (GetPositionValue(i + 4, column) == current)
                ){
            is_win = true;
            break;
        }
    }
    // 左斜方“/”是否有5个
    // 需要和当前的子在一个对角线
    int bottomMargin = bottom-row;  //离最下边的距离
    int leftMargin   = column - left; //离最左边的距离
    int step = 0;

    if (bottomMargin>leftMargin) {//如果左边的距离小于垂直的距离
        step = leftMargin;
    }
    else {//如果离底边的距离小于左边的距离
        step = bottomMargin;
    }

    for (i = row+step, j=column-step; i>=row&&i-4>=top&&j+4<=right; i--, j++){
        if((GetPositionValue(i, j) == current) &&
                (GetPositionValue(i - 1, j+1) == current) &&
                (GetPositionValue(i - 2, j+2) == current) &&
                (GetPositionValue(i - 3, j+3) == current) &&
                (GetPositionValue(i - 4, j+4) == current)
                ){
            is_win = true;
            break;
        }
    }

    // 右斜方“\”是否有5个
    int topMargin = row - top;  //离顶边的距离
    if (topMargin>leftMargin) {//如果左边的距离小于垂直的距离
        step = leftMargin;
    }
    else {//如果离顶的距离小于左边的距离
        step = topMargin;
    }
    for (i = row-step, j = column-step; i+4<=bottom && i<=row && j+4<=right && j<=column; i++, j++ ){
        if((GetPositionValue(i, j) == current) &&
                (GetPositionValue(i + 1, j+1) == current) &&
                (GetPositionValue(i + 2, j+2) == current) &&
                (GetPositionValue(i + 3, j+3) == current) &&
                (GetPositionValue(i + 4, j+4) == current)
                ){
            is_win = true;
            break;
        }
    }

    if (is_win) {
        return 1;
    }
    else {
        return 0;
    }
}

/*
 * 在棋盘当前位置下棋
*/
int Gobang::PutPieceInChess(int row, int column)
{
    // 判断是否已经初始化了，如果没有初始化，返回-1
    if (!is_initialized_){
        BangDebug("PutPieceInChess: the chess is not initialized\n");
        return -1;
    }
    // 判断这个地方是否有落子（值为0），如有已经有落子了则不能再下棋
    if (GetPositionValue(row, column) != 0){
        BangDebug("The value is %d\n", GetPositionValue(row, column));
        return -2;
    }

    // 判断当前的落子方，如果是黑子，则需要将当前位置的值设置为-1
    // 否则将当前位置的值设置为1
    if (current_turn_){
        positions_[row][column] = 1;
    }
    else {
        positions_[row][column] = -1;
    }

    //改变落子方为对方
    current_turn_ = !current_turn_;

#ifdef DEBUG_GOBANG
    PrintBang();
#endif
    return 0;

}

/*
 * 当前下棋方是谁
 * 返回1  ：白棋
 * 返回-1 ：黑棋
*/
int Gobang::GetCurrentTurn()
{
    if (current_turn_){
        return 1;
    }
    else {
        return -1;
    }

}

/*
 * 当前下棋模式，是否是ai模式
*/
bool Gobang::GetMode()
{
    return is_ai_;
}

/*
 * 获取当前位置棋子的颜色，白色为1，黑色为-1，没有棋子为0
*/
int Gobang::GetPositionValue(int row, int column)
{
    if(row>=rows_ || column>=columns_)
        return -99;

    return positions_[row][column];
}

/*
 * 获取当前位置上下左右5个字范围的矩形边框
 * rect[0]: left
 * rect[1]: right
 * rect[2]: top
 * rect[3]: bottom
*/
int Gobang::GetRectangle(int row, int column, int* rect)
{
    int left;
    int right;
    int top;
    int bottom;

    if (nullptr == rect || sizeof(rect) < 4)
        return -1;

    // 是否靠近了左边沿
    if (column - 4 <= 0) {
        left = 0;
    }
    else {
        left = column - 4;
    }

    // 是否靠近了右边沿
    if (column + 4 >= columns_ - 1){ // 0, 1, 2, ..., columns_-1
        right = columns_ -1;
    }
    else {
        right = column + 4;
    }

    // 是否靠近了上方
    if (row - 4 <= 0) {
        top = 0;
    }
    else {
        top = row - 4;
    }

    // 是否靠近了下方
    if (row + 4 >= rows_ - 1){ // // 0, 1, 2, ..., rows_-1
        bottom = rows_ - 1;
    }
    else {
        bottom = row + 4;
    }

    rect[0] = left;
    rect[1] = right;
    rect[2] = top;
    rect[3] = bottom;

    return 0;

}

void Gobang::PrintBang()
{
    char buf[1000] = {0};

    BangDebug("=========================\n");
    for (int i=0; i<rows_; i++) {
        int cnt = 0;
        for (int j=0; j<columns_ && cnt<1000; j++) {
            int pos = positions_[i][j];
            if (pos == 1) { //白旗
                buf[cnt++] = 'x';
            }
            else if (pos == -1) {//黑旗
                buf[cnt++] = '*';
            }
            else {
                buf[cnt++] = '0';
            }
            if (cnt >= 1000)
                break;
        }
        BangDebug(buf);
    }

}

void Gobang::BangDebug(const char* output,...)
{
#ifdef DEBUG_GOBANG

    char buf[4096]={0};
    va_list vlArgs;
    va_start(vlArgs, output);

    _vsnprintf_s(buf, sizeof(buf)-1, output, vlArgs);

    qDebug()<< buf;

#endif
}
