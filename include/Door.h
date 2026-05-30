#ifndef DOOR_H
#define DOOR_H

#include <chrono>

class Door {
    public:
        static constexpr int MOVEMENT_DURATION_S = 1;
        enum class Position {OPEN, CLOSED, OPENING, CLOSING};
        Door(Position starting_position = Position::CLOSED);
        void initialize();
        Position get_current_door_position();
        std::chrono::steady_clock::time_point get_movement_start_time();
        int calculate_percent_open(std::chrono::milliseconds extra_time = std::chrono::milliseconds(0));
        void set_next_door_position(Position position);
        void press_button();
        void update(std::chrono::milliseconds extra_time = std::chrono::milliseconds(0));
        std::string position_to_string(Position position);
    private:
        Position current_position;
        std::chrono::steady_clock::time_point movement_start_time;
};

#endif