/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            board[i][j] = '-';
        }
    }
    //throw NotImplementedException();
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    // TODO: implement
    if (row<0 || row >= size || col<0 || col>=size) {
        return false;
    }
    return board[row][col];
    //throw NotImplementedException();
    //return false;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    // TODO: implement
    board[row][col] = symbol;
    //throw NotImplementedException();
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    // TODO: implement
    if (x < 0 || x >= size || y < 0 || y >= size)
        return true;
    return board[x][y] == '-' || board[x][y] == symbol;
    //throw NotImplementedException();
    //return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    // TODO: implement
    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != symbol)
                continue;

            for (int d = 0; d < 4; d++)
            {
                // Count consecutive symbols
                int count = 0;
                int x = i, y = j;
                while (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == symbol)
                {
                    count++;
                    x += dx[d];
                    y += dy[d];
                }
                if (count < goal)
                    continue;
                if (rule == EndRule::NONE)
                    return true;
                int headX = i - dx[d];
                int headY = j - dy[d];
                bool headOpen = (headX < 0 || headX >= size || headY < 0 || headY >= size || board[headX][headY] == '-');

                bool tailOpen = (x < 0 || x >= size || y < 0 || y >= size || board[x][y] == '-');

                if (rule == EndRule::OPEN_ONE && (headOpen || tailOpen))
                    return true;
                if (rule == EndRule::OPEN_TWO && headOpen && tailOpen)
                    return true;
            }
        }
    }
    return false;
    //throw NotImplementedException();
    //return false;
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == '-')
                return false;
    return true;
    //throw NotImplementedException();
    //return false;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    // TODO: implement
    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != symbol)
                continue;

            for (int d = 0; d < 4; d++)
            {
                // Count consecutive symbols
                int count = 0;
                int x = i, y = j;
                while (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == symbol)
                {
                    count++;
                    x += dx[d];
                    y += dy[d];
                }
                if (count < goal)
                    continue;

                int headX = i - dx[d], headY = j - dy[d];
                bool headOpen = (headX < 0 || headX >= size || 
                                 headY < 0 || headY >= size || 
                                 board[headX][headY] == '-');
                bool tailOpen = (x < 0 || x >= size || 
                                 y < 0 || y >= size || 
                                 board[x][y] == '-');

                bool isWin = false;
                if (rule == EndRule::NONE) {
                    isWin = true;
                }
                if (rule == EndRule::OPEN_ONE) {
                    isWin = headOpen || tailOpen;
                }
                if (rule == EndRule::OPEN_TWO) {
                    isWin = headOpen && tailOpen;
                }

                if (!isWin) continue;

                WinLine winLine;
                for (int k = 0; k < count; k++) {
                    winLine.cells.push_back({
                        i + k * dx[d], 
                        j + k * dy[d]
                    });
                }
                return winLine;
            }
        }
    }
    //return false;
    return std::nullopt;
}