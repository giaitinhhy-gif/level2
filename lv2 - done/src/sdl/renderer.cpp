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

/* ================================================================
   Màu sắc dùng chung (palette)
   ================================================================ */
static const SDL_Color C_BG         = { 13,  13,  26, 255};  // nền
static const SDL_Color C_PANEL      = { 22,  22,  45, 255};  // panel
static const SDL_Color C_BORDER     = { 74,  74, 122, 255};  // viền idle
static const SDL_Color C_BORDER_HV  = {120, 120, 200, 255};  // viền hover
static const SDL_Color C_BTN_IDLE   = { 30,  30,  58, 255};  // nền button idle
static const SDL_Color C_BTN_HOVER  = { 42,  42,  90, 255};  // nền button hover
static const SDL_Color C_X          = { 79, 195, 247, 255};  // màu X
static const SDL_Color C_O          = {255, 112,  67, 255};  // màu O
static const SDL_Color C_TEXT       = {232, 232, 255, 255};  // text chính
static const SDL_Color C_TEXT_DIM   = {120, 120, 168, 255};  // text phụ
static const SDL_Color C_YELLOW     = {255, 215,   0, 255};  // vàng (title/win)
static const SDL_Color C_GREEN      = {105, 240, 174, 255};  // xanh lá (ok)
static const SDL_Color C_RED        = {255,  82,  82, 255};  // đỏ (lỗi)
static const SDL_Color C_WIN_GLOW   = {255, 215,   0, 120};  // glow ô thắng

static void drawCirclePoints(SDL_Renderer* r, int cx, int cy, int x, int y) {
    SDL_RenderDrawPoint(r, cx+x, cy+y); SDL_RenderDrawPoint(r, cx-x, cy+y);
    SDL_RenderDrawPoint(r, cx+x, cy-y); SDL_RenderDrawPoint(r, cx-x, cy-y);
    SDL_RenderDrawPoint(r, cx+y, cy+x); SDL_RenderDrawPoint(r, cx-y, cy+x);
    SDL_RenderDrawPoint(r, cx+y, cy-x); SDL_RenderDrawPoint(r, cx-y, cy-x);
}

static void drawCircleThick(SDL_Renderer* r, int cx, int cy, int radius, int thickness) {
    for (int t = 0; t < thickness; t++) {
        int rad = radius - t, x = 0, y = rad, d = 1 - rad;
        while (x <= y) {
            drawCirclePoints(r, cx, cy, x, y);
            if (d < 0) d += 2*x+3; else { d += 2*(x-y)+5; y--; }
            x++;
        }
    }
}

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
    screenW   = config.screenWidth;
    screenH   = config.screenHeight;
    boardPad  = config.boardPadding;
 
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
 
    window = SDL_CreateWindow(
        "TicTacToe",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screenW, screenH, 0);
 
    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
 
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
 
    // Tải font — fallback graceful nếu không tìm thấy file
    fontLg = TTF_OpenFont("assets/font.ttf", 48);
    fontMd = TTF_OpenFont("assets/font.ttf", 26);
    fontSm = TTF_OpenFont("assets/font.ttf", 18);
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, C_BG.r, C_BG.g, C_BG.b, 255);
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
    if (filled) SDL_RenderFillRect(renderer, &rect);
    else        SDL_RenderDrawRect(renderer, &rect);
}

void SDLRenderer::drawRoundRect(int x, int y, int w, int h, int r, SDL_Color col, bool filled) {
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    if (filled) {
        // Thân giữa
        SDL_Rect mid = {x, y+r, w, h-2*r};
        SDL_RenderFillRect(renderer, &mid);
        SDL_Rect top = {x+r, y, w-2*r, r};
        SDL_RenderFillRect(renderer, &top);
        SDL_Rect bot = {x+r, y+h-r, w-2*r, r};
        SDL_RenderFillRect(renderer, &bot);
        // Góc tròn (xấp xỉ)
        for (int dy = 0; dy < r; dy++) {
            int dx = (int)(r - std::sqrt((double)(r*r - (r-dy-1)*(r-dy-1))) + 0.5);
            SDL_RenderDrawLine(renderer, x+dx,   y+dy,     x+w-dx-1, y+dy);
            SDL_RenderDrawLine(renderer, x+dx,   y+h-dy-1, x+w-dx-1, y+h-dy-1);
        }
    } else {
        // Chỉ vẽ viền (4 cạnh thẳng + 4 góc tròn)
        SDL_RenderDrawLine(renderer, x+r,   y,     x+w-r, y);
        SDL_RenderDrawLine(renderer, x+r,   y+h-1, x+w-r, y+h-1);
        SDL_RenderDrawLine(renderer, x,     y+r,   x,     y+h-r);
        SDL_RenderDrawLine(renderer, x+w-1, y+r,   x+w-1, y+h-r);
        for (int dy = 0; dy < r; dy++) {
            int dx = (int)(r - std::sqrt((double)(r*r - (r-dy-1)*(r-dy-1))) + 0.5);
            // top-left
            SDL_RenderDrawPoint(renderer, x+dx,   y+dy);
            // top-right
            SDL_RenderDrawPoint(renderer, x+w-dx-1, y+dy);
            // bot-left
            SDL_RenderDrawPoint(renderer, x+dx,   y+h-dy-1);
            // bot-right
            SDL_RenderDrawPoint(renderer, x+w-dx-1, y+h-dy-1);
        }
    }
}


void SDLRenderer::drawText(const std::string& text, int x, int y, SDL_Color col, TTF_Font* font) {
    if (!font) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dst = {x, y, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void SDLRenderer::drawTextCentered(const std::string& text, int cx, int y, SDL_Color col, TTF_Font* font) {
    if (!font) return;
    int tw, th;
    TTF_SizeUTF8(font, text.c_str(), &tw, &th);
    drawText(text, cx - tw/2, y, col, font);
}

void SDLRenderer::drawButton(const Button& btn, bool hovered) {
    SDL_Color bg  = hovered ? C_BTN_HOVER : C_BTN_IDLE;
    SDL_Color brd = hovered ? C_BORDER_HV : C_BORDER;
 
    // Nền button bo góc
    drawRoundRect(btn.rect.x, btn.rect.y, btn.rect.w, btn.rect.h, 10, bg,  true);
    drawRoundRect(btn.rect.x, btn.rect.y, btn.rect.w, btn.rect.h, 10, brd, false);
 
    // Text căn giữa button
    if (!fontMd) return;
    int tw, th;
    TTF_SizeUTF8(fontMd, btn.label.c_str(), &tw, &th);
    int tx = btn.rect.x + (btn.rect.w - tw) / 2;
    int ty = btn.rect.y + (btn.rect.h - th) / 2;
    drawText(btn.label, tx, ty, C_TEXT, fontMd);
}
 
void SDLRenderer::drawButtons() {
    for (auto& btn : currentButtons) drawButton(btn, false);
}
 
void SDLRenderer::highlightHovered(int mx, int my) {
    for (auto& btn : currentButtons) {
        bool hov = (mx >= btn.rect.x && mx <= btn.rect.x + btn.rect.w &&
                    my >= btn.rect.y && my <= btn.rect.y + btn.rect.h);
        drawButton(btn, hov);
    }
    renderPresent();
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
    currentButtons.clear();

    // PLAYER_UI xử lý riêng — KHÔNG clearScreen, chỉ thêm text lên board hiện có
    if (selectType == SelectType::PLAYER_UI) {
        drawTextCentered("Click vào ô trên bàn cờ để di chuyển",
                         screenW/2, 752, C_TEXT_DIM, fontSm);
        renderPresent();
        return;  // thoát luôn, không chạy xuống dưới
    }

    clearScreen();
    //currentButtons.clear();
 
    drawRect(0, 0, screenW, 4, C_YELLOW, true);
 
    switch (selectType) {
 
    /* ------ TITLE ------ */
    case SelectType::TITLE_UI: {
        // Logo text lớn
        drawTextCentered("TIC TAC TOE", screenW/2, 240, C_YELLOW,   fontLg);
        drawTextCentered("SDL Edition", screenW/2, 300, C_TEXT_DIM,  fontSm);
 
        SDL_SetRenderDrawColor(renderer, C_BORDER.r, C_BORDER.g, C_BORDER.b, 255);
        SDL_RenderDrawLine(renderer, 200, 360, 600, 360);
 
        // Button bắt đầu
        currentButtons.push_back({{300, 390, 200, 56}, "START", 1});
 
        //drawTextCentered("Click de tiep tuc", screenW/2, 465, C_TEXT_DIM, fontSm);
        break;
    }
 
    /* ------ SIZE ------ */
    case SelectType::SIZE_UI: {
        drawTextCentered("KÍCH THƯỚC BÀN CỜ", screenW/2, 180, C_TEXT, fontMd);
        drawTextCentered("Chọn kích thước:",
                        screenW/2, 224, C_TEXT_DIM, fontSm);
        // Giá trị mặc định = 3, vẽ picker — interaction sẽ update realtime
        drawNumberPicker(3, BOARD_N_MIN, BOARD_N_MAX);
        break;
    }

    case SelectType::GOAL_UI: {
        drawTextCentered("GOAL", screenW/2, 180, C_TEXT, fontMd);
        drawTextCentered("Bàn cờ " + std::to_string(context)
                        + "x" + std::to_string(context),
                        screenW/2, 224, C_TEXT_DIM, fontSm);
        drawNumberPicker(3, 3, std::min(context, GOAL_MAX));
        break;
    }
 
    /* ------ GAME MODE ------ */
    case SelectType::GAME_MODE_UI: {
        drawTextCentered("CHẾ ĐỘ CHƠI", screenW/2, 180, C_TEXT, fontMd);
 
        // 3 button lớn xếp ngang
        const int btnW = 170, btnH = 72, gap = 24;
        int totalW = 3*btnW + 2*gap;
        int startX = (screenW - totalW) / 2;
 
        currentButtons.push_back({{startX,              288, btnW, btnH}, "PvP", 1});
        currentButtons.push_back({{startX + btnW+gap,   288, btnW, btnH}, "PvE", 2});
        currentButtons.push_back({{startX + 2*(btnW+gap),288, btnW, btnH}, "EvE", 3});
 
        // Sub-labels bên dưới mỗi button
        drawTextCentered("Người vs Người", startX + btnW/2,     372, C_TEXT_DIM, fontSm);
        drawTextCentered("Người vs Bot",   startX+btnW+gap+btnW/2, 372, C_TEXT_DIM, fontSm);
        drawTextCentered("Bot vs Bot",     startX+2*(btnW+gap)+btnW/2, 372, C_TEXT_DIM, fontSm);
        break;
    }
 
    /* ------ BOT LEVEL ------ */
    case SelectType::BOT_LEVEL_UI:
    case SelectType::MUL_BOT_LEVEL_UI: {
        std::string who = (selectType == SelectType::MUL_BOT_LEVEL_UI && context >= 0)
            ? "Bot " + std::to_string(context + 1) : "Bot";
        drawTextCentered("ĐỘ KHÓ CỦA " + who, screenW/2, 180, C_TEXT, fontMd);
 
        const int btnW = 160, btnH = 68, gap = 24;
        int totalW = 3*btnW + 2*gap;
        int startX = (screenW - totalW) / 2;
 
        currentButtons.push_back({{startX,               290, btnW, btnH}, "EASY",   1});
        currentButtons.push_back({{startX + btnW+gap,    290, btnW, btnH}, "MEDIUM", 2});
        currentButtons.push_back({{startX + 2*(btnW+gap),290, btnW, btnH}, "HARD",   3});
 
        drawTextCentered("Ngẫu nhiên",    startX+btnW/2,          370, C_TEXT_DIM, fontSm);
        drawTextCentered("Có chiến lược", startX+btnW+gap+btnW/2, 370, C_TEXT_DIM, fontSm);
        drawTextCentered("Thông minh",    startX+2*(btnW+gap)+btnW/2, 370, C_TEXT_DIM, fontSm);
        break;
    }
 
    /* ------ PLAYER MOVE ------ */
    case SelectType::PLAYER_UI:
        drawTextCentered("Click vào ô trên bàn cờ để di chuyển", screenW/2, 752, C_TEXT_DIM, fontSm);
        break;
 
    default: break;
    }
 
    drawButtons();
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
    drawRect(0, 440, screenW, 60, C_BG, true);
 
    // Panel lỗi
    drawRoundRect(200, 448, 400, 40, 8, {60, 20, 20, 220}, true);
    drawRoundRect(200, 448, 400, 40, 8, C_RED,              false);
    drawTextCentered("Lựa chọn không hợp lệ! Thử lại.", screenW/2, 458, C_RED, fontSm);
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
    boardSize = size;
 
    // Tính layout
    int boardArea = screenW - 2 * boardPad;
    cellSize  = boardArea / size;
    boardOffX = boardPad;
    boardOffY = boardPad;
 
    // ---- Nền board ----
    int bw = cellSize * size, bh = cellSize * size;
    drawRoundRect(boardOffX - 4, boardOffY - 4, bw + 8, bh + 8, 6, C_BORDER, false);
    drawRect(boardOffX, boardOffY, bw, bh, C_PANEL, true);
 
    // ---- Grid lines ----
    SDL_SetRenderDrawColor(renderer, C_BORDER.r, C_BORDER.g, C_BORDER.b, 255);
    for (int i = 0; i <= size; i++) {
        // ngang
        int y = boardOffY + i * cellSize;
        SDL_RenderDrawLine(renderer, boardOffX, y, boardOffX + bw, y);
        // dọc
        int x = boardOffX + i * cellSize;
        SDL_RenderDrawLine(renderer, x, boardOffY, x, boardOffY + bh);
    }
 
    // ---- Số tọa độ ----
    for (int i = 0; i < size; i++) {
        std::string s = std::to_string(i);
        // cột (trên)
        drawTextCentered(s, boardOffX + i*cellSize + cellSize/2,
                         boardOffY - 28, C_TEXT_DIM, fontSm);
        // hàng (trái)
        drawTextCentered(s, boardOffX - 24,
                         boardOffY + i*cellSize + cellSize/2 - 9, C_TEXT_DIM, fontSm);
    }
 
    // ---- Ký hiệu X / O ----
    int mg = cellSize / 5; 
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int cx = boardOffX + c * cellSize;
            int cy = boardOffY + r * cellSize;
            char sym = board[r][c];
 
            if (sym == 'X') {
                SDL_SetRenderDrawColor(renderer, C_X.r, C_X.g, C_X.b, 255);
                for (int t = -2; t <= 2; t++) {
                    SDL_RenderDrawLine(renderer,
                        cx+mg, cy+mg+t, cx+cellSize-mg, cy+cellSize-mg+t);
                    SDL_RenderDrawLine(renderer,
                        cx+cellSize-mg, cy+mg+t, cx+mg, cy+cellSize-mg+t);
                }
            } else if (sym == 'O') {
                SDL_SetRenderDrawColor(renderer, C_O.r, C_O.g, C_O.b, 255);
                int ocx = cx + cellSize/2;
                int ocy = cy + cellSize/2;
                int rad = cellSize/2 - mg;
                drawCircleThick(renderer, ocx, ocy, rad, 4);
            }
        }
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
    if (cellSize == 0) return;
    int x = boardOffX + col * cellSize;
    int y = boardOffY + row * cellSize;
    drawRect(x+2, y+2, cellSize-4, cellSize-4, {255, 255, 255, 18}, true);
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
    drawRect(0, screenH-48, screenW, 48, C_BG, true);
    drawRoundRect(160, screenH-42, 480, 34, 6, {60, 20, 20, 210}, true);
    drawRoundRect(160, screenH-42, 480, 34, 6, C_RED,              false);
    drawTextCentered("Ô không hợp lệ! Hãy chọn ô trống.", screenW/2, screenH-38, C_RED, fontSm);
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
    drawRect(0, 0, screenW, 56, C_BG, true);
    drawRect(0, 0, screenW, 4, player == 0 ? C_X : C_O, true);  // gạch màu top
 
    // Panel
    drawRoundRect(20, 10, screenW-40, 38, 6, C_PANEL, true);
    drawRoundRect(20, 10, screenW-40, 38, 6, C_BORDER, false);
 
    std::string sym  = (player == 0) ? "X" : "O";
    SDL_Color   symC = (player == 0) ? C_X : C_O;
 
    std::string msg = is_bot
        ? "Bot " + sym + " (Người chơi " + std::to_string(player+1) + ") đang suy nghĩ..."
        : "Lượt của người chơi " + std::to_string(player+1) + " [" + sym + "]";
 
    drawTextCentered(msg, screenW/2, 18, symC, fontSm);
    //renderPresent();
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
    // Highlight đường thắng
    if (winLine && cellSize > 0) {
        for (auto [r, c] : winLine->cells) {
            int x = boardOffX + c * cellSize;
            int y = boardOffY + r * cellSize;
            drawRect(x+2, y+2, cellSize-4, cellSize-4, C_WIN_GLOW, true);
        }
    }
 
    // Overlay tối mờ giữa màn hình
    drawRect(0, 0, screenW, screenH, {0, 0, 0, 120}, true);
 
    // Panel kết quả
    int pw = 460, ph = 160;
    int px = (screenW - pw) / 2, py = (screenH - ph) / 2;
    drawRoundRect(px, py, pw, ph, 14, C_PANEL, true);
 
    SDL_Color topBar = (winner == DRAW_RESULT) ? C_TEXT_DIM
                     : (winner == 0) ? C_X : C_O;
    drawRoundRect(px, py, pw, ph, 14, topBar, false);
 
    // Gạch màu trên panel
    drawRect(px+14, py, pw-28, 4, topBar, true);
 
    // Text kết quả
    std::string line1, line2;
    if (winner == DRAW_RESULT) {
        line1 = "HÒA!";
        //LINE2 = "Không ai thắng cả."
    } else {
        std::string sym = (winner == 0) ? "X" : "O";
        line1 = (is_bot ? "BOT " : "NGƯỜI CHƠI ") + std::to_string(winner+1) + " [" + sym + "] THẮNG!";
        //line2 = "Chuc mung!";
    }
 
    drawTextCentered(line1, screenW/2, py + 36, C_YELLOW, fontMd);
    drawTextCentered(line2, screenW/2, py + 78, C_TEXT,   fontSm);
    drawTextCentered("(Nhấn phím bất kỳ để thoát)", screenW/2, py + 116, C_TEXT_DIM, fontSm);
 
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

/*
void SDLRenderer::close() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}*/

void SDLRenderer::drawNumberPicker(int value, int minVal, int maxVal) {
    const int btnSize = 60;
    const int numW    = 120; 
    const int gap     = 16;
    const int y       = 290;
    const int totalW  = btnSize + gap + numW + gap + btnSize;
    const int startX  = (screenW - totalW) / 2;

    bool canDec = (value > minVal);
    SDL_Color cDec = canDec ? C_BTN_HOVER : C_BTN_IDLE;
    SDL_Color bDec = canDec ? C_BORDER_HV : C_BORDER;
    drawRoundRect(startX, y, btnSize, btnSize, 10, cDec, true);
    drawRoundRect(startX, y, btnSize, btnSize, 10, bDec, false);
    drawTextCentered("-", startX + btnSize/2, y, C_TEXT, fontLg);

    drawRoundRect(startX + btnSize + gap, y, numW, btnSize, 10, C_PANEL, true);
    drawRoundRect(startX + btnSize + gap, y, numW, btnSize, 10, C_YELLOW, false);
    std::string valStr = std::to_string(value);
    //if (!unit.empty()) valStr += unit;
    drawTextCentered(valStr, startX + btnSize + gap + numW/2,
                     y + 3, C_YELLOW, fontLg);


    bool canInc = (value < maxVal);
    SDL_Color cInc = canInc ? C_BTN_HOVER : C_BTN_IDLE;
    SDL_Color bInc = canInc ? C_BORDER_HV : C_BORDER;
    int plusX = startX + btnSize + gap + numW + gap;
    drawRoundRect(plusX, y, btnSize, btnSize, 10, cInc, true);
    drawRoundRect(plusX, y, btnSize, btnSize, 10, bInc, false);
    drawTextCentered("+", plusX + btnSize/2, y, C_TEXT, fontLg);

    std::string range = "(" + std::to_string(minVal)
                      + " - " + std::to_string(maxVal) + ")";
    drawTextCentered(range, screenW/2, y + btnSize + 14, C_TEXT_DIM, fontSm);

    currentButtons.clear();
    currentButtons.push_back({{(screenW-160)/2, y + btnSize + 46, 160, 50},
                               "XÁC NHẬN", 0});
    drawButtons();

    renderPresent();
}

void SDLRenderer::close() {
    if (fontLg) { TTF_CloseFont(fontLg); fontLg = nullptr; }
    if (fontMd) { TTF_CloseFont(fontMd); fontMd = nullptr; }
    if (fontSm) { TTF_CloseFont(fontSm); fontSm = nullptr; }
    TTF_Quit();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
}

