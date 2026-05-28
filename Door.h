#ifndef DOOR_H

#include <chrono>

class Door {
    public:
        enum class Position {OPEN, CLOSED, OPENING, CLOSING};
        static constexpr int MOVEMENT_DURATION_S = 1;
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
        Door();
        void initialize();
        Position get_current_door_position();
        std::chrono::steady_clock::time_point get_movement_start_time();
        void set_next_door_position(Position position);
        void press_button();
        void update();
    private:
        Position current_position;
        std::chrono::steady_clock::time_point movement_start_time;

        void draw_door(int percent_open);
};

#endif