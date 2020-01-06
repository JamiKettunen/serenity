#include "BlocksGame.h"
#include <LibDraw/PNGLoader.h>
#include <LibGUI/GAboutDialog.h>
#include <LibGUI/GAction.h>
#include <LibGUI/GApplication.h>
#include <LibGUI/GBoxLayout.h>
#include <LibGUI/GButton.h>
#include <LibGUI/GMenu.h>
#include <LibGUI/GMenuBar.h>
#include <LibGUI/GWindow.h>

int main(int argc, char** argv)
{
    GApplication app(argc, argv);

    auto window = GWindow::construct();
    window->set_resizable(false);
    window->set_double_buffering_enabled(false);
    window->set_title("Blocks");

    auto game = BlocksGame::construct();
    window->set_rect(100, 100, game->columns * game->tile_size, game->rows * game->tile_size);
    // TODO: Implement this property
    //window->set_minimum_size(window->rect().size())

    window->set_main_widget(game);

    auto menubar = make<GMenuBar>();

    auto app_menu = GMenu::construct("Blocks");
    app_menu->add_action(GAction::create("New game", { Mod_None, Key_F2 }, [&](const GAction&) {
        game->reset();
    }));
    app_menu->add_action(GCommonActions::make_quit_action([](auto&) {
        GApplication::the().quit(0);
    }));
    menubar->add_menu(move(app_menu));

    auto view_menu = GMenu::construct("View");
    view_menu->add_action(GAction::create("Fullscreen", { Mod_None, Key_F11 }, [&](const GAction&) {
        // TODO: Inform game & resize everything properly
        window->set_fullscreen(true);
    }));
    menubar->add_menu(move(view_menu));

    auto help_menu = GMenu::construct("Help");
    help_menu->add_action(GAction::create("About", [&](const GAction&) {
        GAboutDialog::show("Blocks", load_png("/res/icons/32x32/app-blocks.png"), window);
    }));
    menubar->add_menu(move(help_menu));

    app.set_menubar(move(menubar));

    window->set_icon(load_png("/res/icons/16x16/app-blocks.png"));
    window->show();

    return app.exec();
}
