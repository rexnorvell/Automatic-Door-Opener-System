#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>
#include <string>

class Door;

class Renderer {
    public:
        static constexpr int RESIZING_DELAY_MS = 100;
    
        Renderer();
        void resize();
        void draw_welcome();
        void draw_frame(Door& door);
        void delete_windows();
        void add_log_message(std::string message);
    private:
        WINDOW* welcome_window, *welcome_content, *log_window, *log_content, *door_window, *door_content;
        static constexpr const char* DOOR_OPEN_100 = 
R"( ________________________
/                        \
|                        |
|                        |
|                        |
|                        |
|                        |
|                        |
|                        |
|                        |
        )";
        static constexpr const char* DOOR_OPEN_75 = 
R"( ________________________
/|       RR   RR        |\
||       RR   RR        ||
|========================|
|                        |
|                        |
|                        |
|                        |
|                        |
|                        |
        )";
        static constexpr const char* DOOR_OPEN_50 = 
R"( ________________________
/|       RRRRR          |\
||       RR  RR         ||
||       RR   RR        ||
||       RR   RR        ||
|========================|
|                        |
|                        |
|                        |
|                        |
        )";
        static constexpr const char* DOOR_OPEN_25 = 
R"( ________________________
/|       RR   RR        |\
||       RR   RR        ||
||       RRRRR          ||
||       RR  RR         ||
||       RR   RR        ||
||       RR   RR        ||
|========================|
|                        |
|                        |
        )";
        static constexpr const char* DOOR_OPEN_0 = 
R"( ________________________
/========================\
||        RRRRR         ||
||       RR   RR        ||
||       RR   RR        ||
||       RRRRR          ||
||       RR  RR         ||
||       RR   RR        ||
||       RR   RR        ||
|========================|
        )";

        void create_windows();
        void draw_door(Door& door);
};

#endif