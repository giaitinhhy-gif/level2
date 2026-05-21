/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer() {
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer() {
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */
void SDLRenderer::init(const RunConfig& config) {
    // read config
    int screenWidth = config.screenWidth;
    int screenHeight = config.screenHeight;
    int boardPadding = config.boardPadding;

    // int boardPadding = config.boardPadding;
    // init(...)

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // load font
    font = TTF_OpenFont("assets/font.ttf", 24);

    board_offset_x = boardPadding;
    board_offset_y = boardPadding;
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // dark background
    SDL_RenderClear(renderer);
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent() {
    SDL_RenderPresent(renderer);
}

/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

void SDLRenderer::drawText(const std::string& text, int x, int y, SDL_Color color, int size) {
    if (!font) return;

    TTF_Font* f = TTF_OpenFont("assets/font.ttf", size);
    if (!f) f = font;

    SDL_Surface* surface = TTF_RenderText_Blended(f, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    if (f != font) TTF_CloseFont(f);
}

/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: Render menu UI tương ứng với selectType
    clearScreen();
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 220, 0, 255};

    switch (selectType) {
        case SelectType::TITLE_UI:
            drawText("=== TIC-TAC-TOE ===", 250, 300, yellow);
            drawText("Nhan phim bat ky de tiep tuc...", 220, 360, white);
            break;
        case SelectType::SIZE_UI:
            drawText("Nhap kich thuoc ban co (3 - " + std::to_string(BOARD_N_MAX) + "): ", 200, 350, white);
            break;
        case SelectType::GOAL_UI:
            drawText("Nhap so quan de thang (3 - " + std::to_string(context) + "): ", 200, 350, white);
            break;
        case SelectType::GAME_MODE_UI:
            drawText("Chon che do: (1) PvP  (2) PvE  (3) EvE", 180, 320, white);
            drawText("Nhan phim 1, 2 hoac 3: ", 250, 370, white);
            break;
        case SelectType::BOT_LEVEL_UI:
        case SelectType::MUL_BOT_LEVEL_UI:
            drawText("Chon do kho Bot " + std::to_string(context + 1) + ":", 250, 320, white);
            drawText("(1) EAZY  (2) MEDIUM  (3) HARD", 230, 370, white);
            break;
        case SelectType::PLAYER_UI:
            drawText("Click vao o de di chuyen!", 230, 740, white);
            break;
        default:
            break;
    }
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: Render thông báo lỗi
    SDL_Color red = {255, 80, 80, 255};
    drawText("Invalid input! Thu lai.", 200, 450, red, 22);
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: Render thông báo thành công
    SDL_Color green = {100, 255, 100, 255};
    drawText("OK: " + std::to_string(context), 200, 450, green, 22);
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO: Render board

    //Tính cell size theo size board thực tế
    int padding = 80;
    int boardArea = 800 - padding * 2;   // vùng vẽ board
    cell_size = boardArea / size;
    board_offset_x = padding;
    board_offset_y = padding;

    //Vẽ nền board
    SDL_Rect bg = {board_offset_x, board_offset_y,
                   cell_size * size, cell_size * size};
    SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
    SDL_RenderFillRect(renderer, &bg);

    //Vẽ grid lines
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for (int i = 0; i <= size; i++) {
        //ngang
        int y = board_offset_y + i * cell_size;
        SDL_RenderDrawLine(renderer,
            board_offset_x, y,
            board_offset_x + cell_size * size, y);

        //dọc
        int x = board_offset_x + i * cell_size;
        SDL_RenderDrawLine(renderer,
            x, board_offset_y,
            x, board_offset_y + cell_size * size);
    }

    //Vẽ X / O trong từng ô
    int margin = cell_size / 6;  // khoảng cách trong ô
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int cx = board_offset_x + c * cell_size;
            int cy = board_offset_y + r * cell_size;

            if (board[r][c] == 'X') {
                // Vẽ X bằng 2 đường chéo
                SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255);
                SDL_RenderDrawLine(renderer,
                    cx + margin, cy + margin,
                    cx + cell_size - margin, cy + cell_size - margin);
                SDL_RenderDrawLine(renderer,
                    cx + cell_size - margin, cy + margin,
                    cx + margin, cy + cell_size - margin);

            } else if (board[r][c] == 'O') {
                //Vẽ O bằng hình vuông bo góc (xấp xỉ vòng tròn)
                SDL_SetRenderDrawColor(renderer, 255, 120, 100, 255); // đỏ cam
                //Vẽ nhiều vòng tròn lồng nhau để tạo viền
                int cx_center = cx + cell_size / 2;
                int cy_center = cy + cell_size / 2;
                int radius = cell_size / 2 - margin;

                int x0 = 0, y0 = radius, d = 1 - radius;
                while (x0 <= y0) {
                    SDL_RenderDrawPoint(renderer, cx_center+x0, cy_center+y0);
                    SDL_RenderDrawPoint(renderer, cx_center-x0, cy_center+y0);
                    SDL_RenderDrawPoint(renderer, cx_center+x0, cy_center-y0);
                    SDL_RenderDrawPoint(renderer, cx_center-x0, cy_center-y0);
                    SDL_RenderDrawPoint(renderer, cx_center+y0, cy_center+x0);
                    SDL_RenderDrawPoint(renderer, cx_center-y0, cy_center+x0);
                    SDL_RenderDrawPoint(renderer, cx_center+y0, cy_center-x0);
                    SDL_RenderDrawPoint(renderer, cx_center-y0, cy_center-x0);
                    if (d < 0) d += 2 * x0 + 3;
                    else { d += 2 * (x0 - y0) + 5; y0--; }
                    x0++;
                }
            }
        }
    }

    //Hiển thị tọa độ
    SDL_Color gray = {180, 180, 180, 255};
    for (int i = 0; i < size; i++) {
        //cột
        drawText(std::to_string(i),
                 board_offset_x + i * cell_size + cell_size/2 - 6,
                 board_offset_y - 25, gray, 16);
        //hàng
        drawText(std::to_string(i),
                 board_offset_x - 25,
                 board_offset_y + i * cell_size + cell_size/2 - 8, gray, 16);
    }

    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col) {
    // TODO: Highlight move
    if (cell_size == 0) return;
    int x = board_offset_x + col * cell_size;
    int y = board_offset_y + row * cell_size;
    drawRect(x, y, cell_size, cell_size, {255, 255, 0, 60}, true); // vàng trong suốt
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove() {
    // TODO: Render invalid move message
    drawRect(0, 750, 800, 50, {20, 20, 20, 255}, true);
    SDL_Color red = {255, 80, 80, 255};
    drawText("Invalid move!", 20, 760, red, 18);
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot) {
    // TODO: Render player info
    drawRect(0, 0, 800, 50, {20, 20, 20, 255}, true);

    std::string text = is_bot
        ? "Bot (Player " + std::to_string(player + 1) + ") is thinking..."
        : "Player " + std::to_string(player + 1) + "'s turn";

    SDL_Color white = {255, 255, 255, 255};
    drawText(text, 20, 12, white, 22);
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: Render result
    if (winLine && cell_size > 0) {
        for (auto [r, c] : winLine->cells) {
            int x = board_offset_x + c * cell_size;
            int y = board_offset_y + r * cell_size;
            drawRect(x, y, cell_size, cell_size, {255, 215, 0, 100}, true);
        }
    }

    // Vẽ overlay kết quả
    drawRect(150, 320, 500, 100, {30, 30, 30, 230}, true);

    SDL_Color green = {100, 255, 100, 255};
    SDL_Color white = {255, 255, 255, 255};

    std::string msg;
    if (winner == DRAW_RESULT)
        msg = "Hoa!";
    else if (is_bot)
        msg = "Bot (Nguoi choi " + std::to_string(winner + 1) + ") thang!";
    else
        msg = "Nguoi choi " + std::to_string(winner + 1) + " thang!";

    drawText(msg, 200, 350, green, 32);
    drawText("Exit", 230, 390, white, 18);
    renderPresent();
    //throw NotImplementedException();
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult& gameResult) {
    // TODO: Print result
    fprintf(stdout, "%d %d\n", gameResult.winner, gameResult.turns);
    fflush(stdout);
    //throw NotImplementedException();
}

/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}