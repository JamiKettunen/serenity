#include "BlocksGame.h"
#include <LibGUI/GFontDatabase.h>
#include <LibGUI/GPainter.h>
#include <stdlib.h>
#include <time.h>

BlocksGame::BlocksGame(GWidget* parent)
    : GWidget(parent)
{
    dbg() << "BlocksGame::init()";

    set_font(GFontDatabase::the().get_by_name("Liza Regular"));
    srand(time(nullptr));

    // TODO: Implement saving highscore to FS
    m_high_score = 0;
    m_high_score_text = "Best: 0";

    generate_block_bitmaps();
    reset();
}

BlocksGame::~BlocksGame()
{
}

void BlocksGame::generate_block_bitmaps() {
    dbg() << "BlocksGame::generate_block_bitmaps()";

    Rect block_rect(0, 0, tile_size, tile_size);
    Rect left_side(0, 0, 2, block_rect.height());
    Rect top_side(0, 0, block_rect.width(), 2);
    Rect right_side(block_rect.right() - 1, 0, 2, block_rect.height());
    Rect bottom_side(0, block_rect.bottom() - 1, block_rect.width(), 2);

    for (int i = 0; i < m_block_colors.size(); i++) {
        m_block_bitmaps.append(GraphicsBitmap::create(GraphicsBitmap::Format::RGB32, block_rect.size()));
        GPainter painter(m_block_bitmaps[i]);
        Color block_color = m_block_colors[i];
        Color block_light_color = m_block_light_colors[i];
        Color block_dark_color = m_block_dark_colors[i];
        painter.add_clip_rect(block_rect);

        painter.fill_rect(block_rect, block_color);
        painter.fill_rect(left_side, block_light_color);
        painter.fill_rect(right_side, block_dark_color);
        painter.fill_rect(top_side, block_light_color);
        painter.fill_rect(bottom_side, block_dark_color);
    }
}

void BlocksGame::reset()
{
    dbg() << "BlocksGame::reset()";

    m_score = 0;
    m_score_text = "Score: 0";
    stop_timer();
    start_timer(1000); // TODO: Change as difficulty increases
    generate_block();
}

void BlocksGame::generate_block()
{
    m_block.type = (rand() % m_all_blocks.size()); // 1 +
    dbg() << "BlocksGame::generate_block() >> New block is " << (m_block.type + 1);
    m_block.chars = m_all_blocks[m_block.type];

    m_block.rotation = 0;
    m_block.x = (columns / 2) - 2;
    m_block.y = 0;

    update_block();
}

void BlocksGame::update_block()
{
    dbg() << "BlocksGame::update_block()";

    update();
}

void BlocksGame::move_block(int dx, int dy)
{
    //dbg() << "BlocksGame::move_block(" << dx << ", " << dy << ")";

    if (dx != 0)
        m_block.x += dx;
    if (dy != 0)
        m_block.y += dy;

    // Loop block around horizontally & vertically
    if (m_block.y + 1 > rows)
        m_block.y = 0;

    update();
}

void BlocksGame::rotate_block(RotationDir dir)
{
    dbg() << "BlocksGame::rotate_block(" << (int)dir << ")";

    int char_index;
    Vector<char, 16> rotated_chars;

    if (dir == RotationDir::Clockwise) {
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15
        // =>
        // 12 8  4  0
        // 13 9  5  1
        // 14 10 6  2
        // 15 11 7  3
        for (int x = 0; x < c_area_tiles; x++) {
            for (int y = c_area_tiles - 1; y > -1; y--) {
                char_index = (y * c_area_tiles) + x;
                rotated_chars.append(m_block.chars[char_index]);
            }
        }
    } else {
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15
        // =>
        // 3  7  11 15
        // 2  6  10 14
        // 1  5  9  13
        // 0  4  8  12
        for (int x = c_area_tiles - 1; x > -1; x--) {
            for (int y = 0; y < c_area_tiles; y++) {
                char_index = (y * c_area_tiles) + x;
                rotated_chars.append(m_block.chars[char_index]);
            }
        }
    }

    m_block.chars = rotated_chars;
    update();
}

void BlocksGame::timer_event(CTimerEvent&)
{
    //dbg() << "BlocksGame::timer_event()";

    // TODO: Don't move block down if so was done in last 1s
    move_block(0, 1);
    update();
}

void BlocksGame::keydown_event(GKeyEvent& event)
{
    //dbg() << "BlocksGame::keydown_event(" << event.key() << ")";

    // TODO: Perhaps use Z & X for block rotation instead?

    if (event.modifiers() == Mod_None) {
        switch (event.key()) {
        case Key_A:
        case Key_Left:
            move_block(-1, 0);
            break;
        case Key_D:
        case Key_Right:
            move_block(1, 0);
            break;
        case Key_W:
        case Key_Up:
            // TODO: Put block on hold?
            break;
        case Key_S:
        case Key_Down:
            move_block(0, 1);
            break;
        case Key_R:
            rotate_block(RotationDir::Clockwise);
            break;
        default:
            break;
        }
    } else if (event.modifiers() == Mod_Shift) {
        switch (event.key()) {
        case Key_R:
            rotate_block(RotationDir::Counterclockwise);
            break;
        default:
            break;
        }
    }
}

void BlocksGame::paint_event(GPaintEvent& event)
{
    //dbg() << "BlocksGame::paint_event()";

    GPainter painter(*this);
    Rect main_rect(event.rect());
    Rect inner_rect(1, 1, main_rect.width() - 2, main_rect.height() - 2);
    painter.add_clip_rect(main_rect);

    // Background + white border
    painter.fill_rect(main_rect, Color::from_rgb(0xEBEBEB)); // 235, 235, 235
    painter.fill_rect(inner_rect, Color::from_rgb(0x181818)); // 24, 24, 24
    //painter.fill_rect(main_rect, Color::from_rgb(0x181818)); // 24, 24, 24

    // Horizontal & vertical lines
    int width = columns * tile_size;
    int height = rows * tile_size;
    for (int x = tile_size; x < width; x += tile_size) {
        Rect vertical_line_rect(x, 0, 1, height);
        painter.fill_rect(vertical_line_rect, Color::from_rgb(0xEBEBEB)); // 235, 235, 235
    }
    for (int y = tile_size; y < height; y += tile_size) {
        Rect horizontal_line_rect(0, y, width, 1);
        painter.fill_rect(horizontal_line_rect, Color::from_rgb(0xEBEBEB)); // 235, 235, 235
    }

    // Draw blocks
    int block_x = m_block.x * tile_size;
    int block_y = m_block.y * tile_size;

    // Draw current dropping piece
    int area_size = tile_size * c_area_tiles;
    Rect block_area_rect(block_x, block_y, area_size, area_size);

    for (int y = 0; y < c_area_tiles; y++) {
        for (int x = 0; x < c_area_tiles; x++) {
            int char_index = (y * c_area_tiles) + x;
            bool should_draw = (m_block.chars[char_index] != '\0');
            if (should_draw) {
                Rect tile_rect(block_area_rect.x() + x * tile_size, block_area_rect.y() + y * tile_size, tile_size, tile_size);
                painter.draw_scaled_bitmap(tile_rect, m_block_bitmaps[m_block.type], m_block_bitmaps[m_block.type].rect());
            }
        }
    }
}

void BlocksGame::game_over()
{
    dbg() << "BlocksGame::game_over()";

    reset();
}
