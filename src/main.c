#include <stdio.h>
#include <string.h>
#include "keyboard/keyboard_man.h"
#include "dbus_parser/dbus_parser.h"
#include "ncurses_display/ncurses_display.h"


static int _get_args(char *arg, bool *col, char **font, bool *debug){
    if (strcmp(arg, "--nocolor")==0){
        *col = false;
        return 0;
    }
    if (strncmp(arg, "--font=", 7)==0){
        *font = (char *)realloc(*font, strlen(arg)-6);
        arg += 7;
        strcpy(*font, arg);
        return 0;
    }
    if (strncmp(arg, "--debug", 7)==0){
        *debug = true;
        return 0;
    }
    if (strcmp(arg, "--help")==0){
        fprintf(stdout, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        "Usage: txm [OPTIONS]...",
        "Display currently playing music in the terminal",
        "Example: txm --font=small.k9",
        "Options:\n  --nocolor\tDisables color, shows only ascii art",
        "  --font\tSets a custom .k9 font.",
        "\t\tFonts are loaded locally first then through .txm HOME folder.",
        "\t\tNo spaces allowed. Enclose in quotes if path contains spaces.",
        "  --help\tYou are here"
        );
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]){
    bool color = true;
    bool debug = false;
    char *font_name = malloc(2);
    font_name[0] = '\n';
    if (argc > 1){
        for (int i = 1; i < argc; i++){
            if (_get_args(argv[i], &color, &font_name, &debug)) return 0;
        }
    }
    DBusConnection *connection = setup_dbus_connection("/org/mpris/MediaPlayer2", "interface=org.freedesktop.DBus.Properties");
    get_dbus_player_instances(connection, false);
    SET_TITLE("TxM");
    KeyBinds *binds = init_keybinds();
    init_screen(&color, font_name);
    DBus_Info main_info;
    // asking for info here, then we just wait for updates in main loop
    while (!binds->exit){
        dbus_connection_read_write_dispatch(connection, 100);
        main_info = get_dbus_info();
        render_album_cover(main_info, color);
        display_song_metadata(main_info, font_name);
        handle_inputs(binds);
        if (binds->command == 4 && debug){
            while (!binds->exit){
                render_dbus_sources(main_info, *binds);
                handle_inputs(binds);
            }
            binds->exit = false;
            binds->command = 0;
        }
        check_info_and_send(binds, connection);
    }
    endwin();
}