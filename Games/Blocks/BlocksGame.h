#pragma once

#include <AK/NonnullRefPtrVector.h>
#include <LibDraw/GraphicsBitmap.h>
#include <LibGUI/GWidget.h>

class BlocksGame : public GWidget {
    C_OBJECT(BlocksGame)
public:
    virtual ~BlocksGame() override;

    void reset();

    const int columns { 10 };
    const int rows { 20 };
    const int tile_size { 24 };

private:
    explicit BlocksGame(GWidget* parent = nullptr);

    virtual void paint_event(GPaintEvent&) override;
    virtual void keydown_event(GKeyEvent&) override;
    virtual void timer_event(CTimerEvent&) override;

    void game_over();
    void generate_block();
    void update_block();
    void move_block_down();

    struct Coordinate {
        int row { 0 };
        int column { 0 };

        bool operator==(const Coordinate& other) const
        {
            return (row == other.row && column == other.column);
        }
    };

    //Rect cell_rect(const Coordinate&) const;
    Rect score_rect() const;
    Rect high_score_rect() const;

    // Score
    unsigned m_score { 0 };
    String m_score_text;
    unsigned m_high_score { 0 };
    String m_high_score_text;

    // Bitmaps
    void generate_block_bitmaps();
    NonnullRefPtrVector<GraphicsBitmap, 7> m_block_bitmaps;
    Vector<Color, 7> m_block_colors = {
        Color::from_rgb(0x00C9EA), /* 0: |   */
        Color::from_rgb(0xC2BA00), /* 1: []  */
        Color::from_rgb(0x8200D4), /* 2: T   */
        Color::from_rgb(0xD99300), /* 3: L   */
        Color::from_rgb(0x0032BB), /* 4: R-L */
        Color::from_rgb(0xA90000), /* 5: Z   */
        Color::from_rgb(0x3FC700), /* 6: R-Z */
    };
    Vector<Color, 7> m_block_dark_colors = {
        Color::from_rgb(0x00A8EA), /* 0: |   */
        Color::from_rgb(0xA09A00), /* 1: []  */
        Color::from_rgb(0x7500BF), /* 2: T   */
        Color::from_rgb(0xB97E00), /* 3: L   */
        Color::from_rgb(0x0017BB), /* 4: R-L */
        Color::from_rgb(0x810000), /* 5: Z   */
        Color::from_rgb(0x2F9A00), /* 6: R-Z */
    };
    Vector<Color, 7> m_block_light_colors = {
        Color::from_rgb(0x00E3EA), /* 0: |   */
        Color::from_rgb(0xE1D800), /* 1: []  */
        Color::from_rgb(0x9400F1), /* 2: T   */
        Color::from_rgb(0xF3A500), /* 3: L   */
        Color::from_rgb(0x0056BB), /* 4: R-L */
        Color::from_rgb(0xD20000), /* 5: Z   */
        Color::from_rgb(0x3FEA00), /* 6: R-Z */
    };

    Vector<Vector<char>> m_field;

    // Contains the current block including the rotation
    Vector<char, 16> m_block = { };

    unsigned m_block_type { 0 };
    unsigned m_rotation { 0 };
    Coordinate m_block_pos;
    //Vector<Coordinate> m_tail;

    Vector<Vector<char>, 7> m_all_blocks = {
        Vector<char, 16> { /* 0: | */
            '\0', '1',  '\0', '\0',
            '\0', '1',  '\0', '\0',
            '\0', '1',  '\0', '\0',
            '\0', '1',  '\0', '\0',
        },
        Vector<char, 16> { /* 1: [] */
            '\0', '2',  '2',  '\0',
            '\0', '2',  '2',  '\0',
            '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0',
        },
        Vector<char, 16> { /* 2: T */
            '\0', '3',  '\0', '\0',
            '3',  '3',  '3',  '\0',
            '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0',
        },
        Vector<char, 16> { /* 3: L */
            '\0', '4',  '\0', '\0',
            '\0', '4',  '\0', '\0',
            '\0', '4',  '4',  '\0',
            '\0', '\0', '\0', '\0',
        },
        Vector<char, 16> { /* 4: R-L */
            '\0', '\0', '5',  '\0',
            '\0', '\0', '5',  '\0',
            '\0', '5',  '5',  '\0',
            '\0', '\0', '\0', '\0',
        },
        Vector<char, 16> { /* 5: Z */
            '6',  '6',  '\0', '\0',
            '\0', '6',  '6',  '\0',
            '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0',
        },
        Vector<char, 16> { /* 6: R-Z */
            '\0', '7',  '7',  '\0',
            '7',  '7',  '\0', '\0',
            '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0',
        },
    };
};
