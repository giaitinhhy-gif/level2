/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"
#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>
#include <SDL2/SDL.h>
#include <algorithm>

/* ---------- Definitions ---------- */

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */
bool SDLInteraction::waitForQuit(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        throw QuitException();
    }
    return false;
}

int SDLInteraction::waitForClick(const std::vector<ClickArea>& areas) {
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e);
 
        // Hover: chuột di chuyển → gọi renderer highlight lại buttons
        if (e.type == SDL_MOUSEMOTION && sdlRenderer) {
            sdlRenderer->highlightHovered(e.motion.x, e.motion.y);
        }
 
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x, my = e.button.y;
            for (const auto& area : areas) {
                if (mx >= area.rect.x && mx <= area.rect.x + area.rect.w &&
                    my >= area.rect.y && my <= area.rect.y + area.rect.h) {
                    return area.value;
                }
            }
            // Click ngoài tất cả areas → bỏ qua, tiếp tục chờ
        }
    }
    return -1;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction() {
}

/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction() {
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig& config) {
    // TODO:
    // - Khởi tạo các thành phần cần thiết cho input SDL
    // - Có thể reset event queue hoặc trạng thái input
}

/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout) {
    // Nếu có timeout cụ thể -> delay trực tiếp bằng SDL
    if (timeout > 0) {
        SDL_Delay(timeout);
        return;
    }
    // Chờ bất kỳ phím hoặc click chuột
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e);
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) return;
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */

/*
bool SDLInteraction::selectSize(int* size) {
    // TODO:
    // - Lắng nghe event từ SDL (keyboard/mouse)
    // - Parse input thành số nguyên
    // - Kiểm tra điều kiện hợp lệ (BOARD_N_MIN <= size <= BOARD_N_MAX)
    // - Trả về true nếu hợp lệ, ngược lại false
    const int btnW = 110, btnH = 60, gap = 16;
    const int sizes[] = {3, 4, 5, 6, 7};
    const int totalW  = 5*btnW + 4*gap;
    const int startX  = (800 - totalW) / 2;
    const int y       = 300;
 
    std::vector<ClickArea> areas;
    for (int i = 0; i < 5; i++) {
        areas.push_back({{startX + i*(btnW+gap), y, btnW, btnH}, sizes[i]});
    }
 
    int val = waitForClick(areas);
    if (val >= BOARD_N_MIN && val <= BOARD_N_MAX) {
        *size     = val;
        boardSize = val;
        return true;
    }
    return false;
    //throw NotImplementedException();
} */

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */

/*
bool SDLInteraction::selectGoal(int* goal, const int size) {
    // TODO:
    // - Lấy input từ SDL
    // - Parse thành số nguyên
    // - Kiểm tra điều kiện (3 <= goal <= size)
    // - Trả về true nếu hợp lệ
    int maxGoal = std::min(size, GOAL_MAX);
    int count   = maxGoal - 3 + 1;
 
    const int btnW = 110, btnH = 60, gap = 16;
    int totalW = count*btnW + (count-1)*gap;
    int startX = (800 - totalW) / 2;
    const int y = 300;
 
    std::vector<ClickArea> areas;
    for (int i = 0; i < count; i++) {
        areas.push_back({{startX + i*(btnW+gap), y, btnW, btnH}, 3+i});
    }
 
    int val = waitForClick(areas);
    if (val >= 3 && val <= maxGoal) {
        *goal = val;
        return true;
    }
    //throw NotImplementedException();
    return false;
} */

bool SDLInteraction::selectSize(int* size) {
    int val = waitForPicker(BOARD_N_MIN, BOARD_N_MIN, BOARD_N_MAX,
                             SelectType::SIZE_UI);
    *size     = val;
    boardSize = val;
    return true;  // picker luôn trả về giá trị hợp lệ
}

bool SDLInteraction::selectGoal(int* goal, const int size) {
    int maxGoal = std::min(size, GOAL_MAX);
    int val = waitForPicker(3, 3, maxGoal,
                             SelectType::GOAL_UI, size);
    *goal = val;
    return true;
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    // TODO:
    // - Lắng nghe input từ SDL (phím số hoặc click)
    // - Map input sang GameMode tương ứng
    // - Validate giá trị (1-3)
    const int btnW = 170, btnH = 72, gap = 24;
    const int totalW = 3*btnW + 2*gap;
    const int startX = (800 - totalW) / 2;
    const int y = 288;
 
    std::vector<ClickArea> areas = {
        {{startX,               y, btnW, btnH}, 1},  // PVP
        {{startX + btnW+gap,    y, btnW, btnH}, 2},  // PVE
        {{startX + 2*(btnW+gap),y, btnW, btnH}, 3},  // EVE
    };
 
    int val = waitForClick(areas);
    if (val < 1 || val > 3) return false;
    *mode = (GameMode)(val - 1);
    return true;
    //throw NotImplementedException();
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 *   - levels: mảng lưu cấp độ bot.
 *   - index: vị trí bot cần gán.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    // TODO:
    // - Kiểm tra index hợp lệ (0 hoặc 1)
    // - Lấy input từ SDL
    // - Map sang BotLevel (EASY, MEDIUM, HARD)
    // - Trả về true nếu hợp lệ
    const int btnW = 160, btnH = 68, gap = 24;
    const int totalW = 3*btnW + 2*gap;
    const int startX = (800 - totalW) / 2;
    const int y = 290;
 
    std::vector<ClickArea> areas = {
        {{startX,               y, btnW, btnH}, 1},  // EASY
        {{startX + btnW+gap,    y, btnW, btnH}, 2},  // MEDIUM
        {{startX + 2*(btnW+gap),y, btnW, btnH}, 3},  // HARD
    };
 
    int val = waitForClick(areas);
    if (val < 1 || val > 3) return false;
    levels[index] = (BotLevel)(val - 1);
    return true;
    //throw NotImplementedException();
}

/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
    // TODO:
    // - Lắng nghe mouse click hoặc keyboard input
    // - Chuyển đổi tọa độ click thành (row, col)
    // - Validate phạm vi hợp lệ
    const int screenW  = 800;
    const int boardPad = 70;   // ← phải khớp với RunConfig::boardPadding
    int boardArea = screenW - 2 * boardPad;
    int cs        = boardArea / boardSize;  // cellSize
 
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e);
 
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x, my = e.button.y;
 
            // Kiểm tra click nằm trong vùng board
            int boardL = boardPad;
            int boardT = boardPad;
            int boardR = boardPad + cs * boardSize;
            int boardB = boardPad + cs * boardSize;
 
            if (mx >= boardL && mx < boardR && my >= boardT && my < boardB) {
                *col = (mx - boardPad) / cs;
                *row = (my - boardPad) / cs;
                return true;
            }
            // Click ngoài board → bỏ qua
        }
    }
    //throw NotImplementedException();
    return false;
}

int SDLInteraction::waitForPicker(int initVal, int minVal, int maxVal,
                                   SelectType menuType, int context) {
    // Tọa độ nút − và + (phải khớp với drawNumberPicker)
    const int btnSize = 60, numW = 120, gap = 16;
    const int totalW  = btnSize + gap + numW + gap + btnSize;
    const int startX  = (800 - totalW) / 2;
    const int y       = 290;
    const int plusX   = startX + btnSize + gap + numW + gap;
    const int confirmY = y + btnSize + 46;

    int value = initVal;

    auto redraw = [&]() {
        if (!sdlRenderer) return;
        sdlRenderer->clearScreen();
        sdlRenderer->showSelectMenu(menuType, context);
        // showSelectMenu đã gọi drawNumberPicker với value=initVal
        // → cần vẽ lại với value hiện tại
        sdlRenderer->drawNumberPicker(value, minVal, maxVal);
    };

    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e);

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mx = e.button.x, my = e.button.y;

            // Click "−"
            if (mx >= startX && mx <= startX + btnSize &&
                my >= y      && my <= y + btnSize) {
                if (value > minVal) { value--; redraw(); }
            }
            // Click "+"
            else if (mx >= plusX && mx <= plusX + btnSize &&
                     my >= y     && my <= y + btnSize) {
                if (value < maxVal) { value++; redraw(); }
            }
            // Click "Xac nhan"
            else if (mx >= (800-160)/2 && mx <= (800-160)/2 + 160 &&
                     my >= confirmY     && my <= confirmY + 50) {
                return value;
            }
        }

        // Phím tắt: mũi tên trái/phải hoặc −/+
        if (e.type == SDL_KEYDOWN) {
            if ((e.key.keysym.sym == SDLK_LEFT  ||
                 e.key.keysym.sym == SDLK_MINUS) && value > minVal) {
                value--; redraw();
            }
            if ((e.key.keysym.sym == SDLK_RIGHT ||
                 e.key.keysym.sym == SDLK_EQUALS) && value < maxVal) {
                value++; redraw();
            }
            if (e.key.keysym.sym == SDLK_RETURN) {
                return value;
            }
        }
    }
    return value;
}


/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
    // TODO:
    // - Dọn dẹp trạng thái, nếu có resource thì giải phóng
}