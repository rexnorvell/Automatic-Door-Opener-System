#ifndef DOOR_H
#define DOOR_H

#include <chrono>

class Door {
    public:
        static constexpr int MOVEMENT_DURATION_S = 1;
        enum class Position {OPEN, CLOSED, OPENING, CLOSING};
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
};

#endif