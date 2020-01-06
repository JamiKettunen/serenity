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
    dbg() << "BlocksGame::generate_block()";

    m_block_type = 1 + (rand() % m_all_blocks.size());
    update_block();
}

void BlocksGame::update_block()
{
    dbg() << "BlocksGame::update_block()";

    update();
}

void BlocksGame::move_block_down()
{
    dbg() << "BlocksGame::move_block_down()";

    // TODO: Move the block down by 1

}

void BlocksGame::timer_event(CTimerEvent&)
{
    dbg() << "BlocksGame::timer_event()";

    update();
}

void BlocksGame::keydown_event(GKeyEvent& event)
{
    dbg() << "BlocksGame::keydown_event(" << event.key() << ")";

    switch (event.key()) {
    case KeyCode::Key_A:
    case KeyCode::Key_Left:
        //move_block(-1, 0);
        break;
    case KeyCode::Key_D:
    case KeyCode::Key_Right:
        //move_block(1, 0);
        break;
    case KeyCode::Key_W:
    case KeyCode::Key_Up:
        // TODO: Put block on hold?
        break;
    case KeyCode::Key_S:
    case KeyCode::Key_Down:
        //move_block(0, 1);
        break;
    default:
        break;
    }
}

void BlocksGame::paint_event(GPaintEvent& event)
{
    dbg() << "BlocksGame::paint_event()";

    GPainter painter(*this);
    Rect main_rect(event.rect());
    Rect inner_rect(1, 1, main_rect.width() - 2, main_rect.height() - 2);
    painter.add_clip_rect(main_rect);

    // Background + white border
    painter.fill_rect(main_rect, Color::from_rgb(0xEBEBEB)); // 235, 235, 235
    painter.fill_rect(inner_rect, Color::from_rgb(0x0C0C0C)); // 16, 16, 16

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

    // Draw all blocks for testing!
    for (int t = 0; t < m_block_colors.size(); t++) {
        int x = t * tile_size;
        Rect tile_rect(x, 0, tile_size, tile_size);
        painter.draw_scaled_bitmap(tile_rect, m_block_bitmaps[t], m_block_bitmaps[t].rect());
    }
}

void BlocksGame::game_over()
{
    dbg() << "BlocksGame::game_over()";

    reset();
}
