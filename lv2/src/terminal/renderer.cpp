/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
    // TODO: init state

}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    // TODO: cleanup nếu cần
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    // TODO: init renderer theo config
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    // TODO: clear screen
    std::cout << "\033[2J\033[H";
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: render menu theo loại
    switch (selectType) {
    case SelectType::TITLE_UI:
        std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
        break;

    case SelectType::SIZE_UI:
        std::cout << "Nhap kich thuoc ban co (3 - " << BOARD_N_MAX << "): ";
        break;

    case SelectType::GOAL_UI:
        std::cout << "Nhap so quan de thang (3 - " << context << "): ";
        break;

    case SelectType::GAME_MODE_UI:
        std::cout << "Chon che do choi: (1) PvP  (2) PvE  (3) EvE\nLua chon: ";
        break;

    case SelectType::BOT_LEVEL_UI:
        std::cout << "Chon do kho cua Bot: (1) EASY  (2) MEDIUM  (3) HARD\nLua chon: ";
        break;

    case SelectType::PLAYER_UI:
        std::cout << "Nhap nuoc di (hang, cot): ";
        break;

    case SelectType::MUL_BOT_LEVEL_UI:
        std::cout << "Chon do kho cua Bot: (1) EASY  (2) MEDIUM  (3) HARD\nLua chon: ";
        break;

    default:
        break;
    }

    return;
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: render invalid message
    switch (selectType)
    {
    case SelectType::SIZE_UI:
        std::cout << "[!] Kich thuoc khong hop le! Nhap lai.\n";
        break;

    case SelectType::GOAL_UI:
        std::cout << "[!] Goal khong hop le! Nhap lai.\n";
        break;
    
    case SelectType::GAME_MODE_UI:
        std::cout << "[!] Che do choi khong hop le! Nhap lai.\n";
        break;
    
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        std::cout << "[!] Do kho cua Bot khong hop le! Nhap lai.\n";
        break;
    
    default:
        std::cout << "[!] Lua chon khong hop le! Nhap lai.\n";
        break;
    }
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: render success message
    switch (selectType)
    {
    case SelectType::SIZE_UI:
        std::cout << "Ban co " << context << "x" << context << "\n";
        break;
    case SelectType::GOAL_UI:
        std::cout << "Thang khi co " << context << " quan lien tiep.\n";
        break;
    case SelectType::GAME_MODE_UI:
        if (context == 0) std::cout <<  "Che do: PvP\n";
        else if (context == 1) std::cout << "Che do: PvE\n";
        else std::cout << "Che do: EvE\n";
        break;
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI:
        if (context == 0) std::cout << "Do kho: EAZY\n";
        else if (context == 1) std::cout << "Do kho: MEDIUM\n";
        else std::cout << "Do kho: HARD\n";
        break;
    
    default:
        break;
    }
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO: render board
    std::cout << "   ";
    for (int j = 0; j < size; j++)
        std::cout << " " << j;
    std::cout << "\n";

    std::cout << "   ";
    for (int j = 0; j < size; j++)
        std::cout << "--";
    std::cout << "\n";

    for (int i = 0; i < size; i++) {
        std::cout << " " << i << " |";
        for (int j = 0; j < size; j++)
            std::cout << board[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    // TODO: render player info
    if (is_bot)
    {
        std::cout << "Bot (Nguoi choi " << player << ") dang suy nghi...\n";
    }
    else
    {
        std::cout << "Luot cua nguoi choi " << player << "\n";
    }
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    // TODO: render move
    std::cout << "Nuoc di: hang " << row << ", cot " << col << ")\n";
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    // TODO: render invalid move message
    std::cout << "[!] Nuoc di khong hop le! O da co quan hoac ngoai ban co.\n";
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: render result
    if (winner == DRAW_RESULT)
    {
        std::cout << "Hoa!\n";
    }
    else if (is_bot)
    {
        std::cout << "Bot (Nguoi choi " << (winner + 1) << ") thang!\n";
    }
    else
    {
        std::cout << "Nguoi choi " << (winner + 1) << " thang!\n";
    }
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    // TODO: print result đơn giản
    fprintf(stdout, "%d %d\n", gameResult.winner, gameResult.turns);
    fflush(stdout);
    //throw NotImplementedException();
}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    // TODO: cleanup renderer
}