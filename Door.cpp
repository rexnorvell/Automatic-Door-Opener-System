#include "Door.h"

Door::Door() {
    this->current_position = Door::Position::CLOSED;
}

// RULE 7: At initialization, the Automatic Door Opener system shall cause the door to OPEN if the door is not in either CLOSE or OPEN position.
void Door::initialize() {
    if (this->current_position != Door::Position::OPEN && this->current_position != Door::Position::CLOSED) {
        this->current_position = Door::Position::OPENING;
        this->movement_start_time = std::chrono::steady_clock::now();
    }
}

// RULE 4: The Automatic Door Opener system shall be able to monitor door position.
Door::Position Door::get_current_door_position() {
    return this->current_position;
}

std::chrono::steady_clock::time_point Door::get_movement_start_time() {
    return this->movement_start_time;
}

int Door::calculate_percent_open() {
    int percent_open = 0;
    if (this->current_position == Door::Position::OPENING || this->current_position == Door::Position::CLOSING) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration<float>(now - this->movement_start_time);
        if (current_position == Door::Position::OPENING) {
            percent_open = elapsed.count() * 100;
        }
        else {
            percent_open = 100 - (elapsed.count() * 100);
        }
    }
    else if (this->current_position == Door::Position::OPEN) {
        percent_open = 100;
    }
    return std::max(0, std::min(percent_open, 100));
}

// RULE 5: The Automatic Door Opener system shall be able to command the motor(s) to move the door to the OPEN position.
// RULE 6: The Automatic Door Opener system shall be able to command the motor(s) to move the door to the CLOSE position.
void Door::set_next_door_position(Door::Position position) {
    if (this->current_position == Door::Position::OPENING || this->current_position == Door::Position::CLOSING) {
        return;
    }
    if ((this->current_position == Door::Position::OPEN && position == Door::Position::OPENING) || (this->current_position == Door::Position::CLOSED && position == Door::Position::CLOSING)) {
        return;
    }
    this->current_position = position;
    if (this->current_position == Door::Position::OPENING || this->current_position == Door::Position::CLOSING) {
        this->movement_start_time = std::chrono::steady_clock::now();
    }
}

// RULE 1: When the button is pressed, the Automatic Door Opener system shall cause the door to CLOSE if it is currently in the OPEN position.
// RULE 2: When the button is pressed, the Automatic Door Opener system shall cause the door to OPEN if it is currently in the CLOSE position.
// RULE 3: When the button is pressed, the Automatic Door Opener system shall ignore the button press if the door is currently MOVING.
void Door::press_button() {
    if (this->current_position == Door::Position::OPENING || this->current_position == Door::Position::CLOSING) {
        return;
    }
    if (this->current_position == Door::Position::OPEN || this->current_position == Door::Position::OPENING) {
        this->current_position = Door::Position::CLOSING;
    }
    else if (this->current_position == Door::Position::CLOSED || this->current_position == Door::Position::CLOSING) {
        this->current_position = Door::Position::OPENING;
    }
    this->movement_start_time = std::chrono::steady_clock::now();
}

void Door::update() {
    if (this->current_position != Door::Position::OPENING && this->current_position != Door::Position::CLOSING) {
        return;
    }
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<float>(now - this->movement_start_time);
    if (elapsed.count() >= MOVEMENT_DURATION_S) {
        this->current_position = (this->current_position == Door::Position::OPENING) ? (Door::Position::OPEN) : (Door::Position::CLOSED);
    }
}
