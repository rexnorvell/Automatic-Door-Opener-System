#include <iostream>

#include <catch2/catch_amalgamated.hpp>

#include "Door.h"

constexpr int DOOR_MOVEMENT_TIME_MS = Door::MOVEMENT_DURATION_S * 1200;

Door create_and_initialize_door(Door::Position starting_position);
void fast_forward(Door& door, int ms_to_pass = DOOR_MOVEMENT_TIME_MS);
void assert_position(Door& door, Door::Position desired_position);
void assert_position_changes(Door& door);
void press_button(Door& door);

// System Requirements
// SR1: When the button is pressed, the Automatic Door Opener system shall cause the door to CLOSE if it is currently in the OPEN position.
TEST_CASE("SR1: TC1 - When the button is pressed, the door will close if it is currently open.") {
    std::cout << "SR1: TC1 - When the button is pressed, the door will close if it is currently open.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    press_button(door);
    fast_forward(door);
    assert_position(door, Door::Position::CLOSED);
}

// SR2: When the button is pressed, the Automatic Door Opener system shall cause the door to OPEN if it is currently in the CLOSE position.
TEST_CASE("SR2: TC1 - When the button is pressed, the door will open if it is currently closed.") {
    std::cout << "SR1: TC1 - When the button is pressed, the door will open if it is currently closed.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    press_button(door);
    fast_forward(door);
    assert_position(door, Door::Position::OPEN);
}

// SR3: When the button is pressed, the Automatic Door Opener system shall ignore the button press if the door is currently MOVING.
TEST_CASE("SR3: TC1 - When the button is pressed, the door will ignore the press if it is currently opening.") {
    std::cout << "SR3: TC1 - When the button is pressed, the door will ignore the press if it is currently opening.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    press_button(door);
    assert_position(door, Door::Position::OPENING);
    press_button(door);
    assert_position(door, Door::Position::OPENING);
}

TEST_CASE("SR3: TC2 - When the button is pressed, the door will ignore the press if it is currently closing.") {
    std::cout << "SR3: TC2 - When the button is pressed, the door will ignore the press if it is currently closing.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    press_button(door);
    assert_position(door, Door::Position::CLOSING);
    press_button(door);
    assert_position(door, Door::Position::CLOSING);
}

// SR4: The Automatic Door Opener system shall be able to monitor door position.
TEST_CASE("SR4: TC1 - The door position is correctly reported as the door is opening.") {
    std::cout << "SR4: TC1 - The door position is correctly reported as the door is opening.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    press_button(door);
    assert_position(door, Door::Position::OPENING);
    assert_position_changes(door);
}

TEST_CASE("SR4: TC2 - The door position is correctly reported as the door is closing.") {
    std::cout << "SR4: TC2 - The door position is correctly reported as the door is closing.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    press_button(door);
    assert_position(door, Door::Position::CLOSING);
    assert_position_changes(door);
}

TEST_CASE("SR4: TC3 - The door position is correctly reported when the door is open.") {
    std::cout << "SR4: TC3 - The door position is correctly reported when the door is open.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    REQUIRE(door.calculate_percent_open() == 100);
}

TEST_CASE("SR4: TC4 - The door position is correctly reported when the door is closed.") {
    std::cout << "SR4: TC4 - The door position is correctly reported when the door is closed.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    REQUIRE(door.calculate_percent_open() == 0);
}

// SR5: The Automatic Door Opener system shall be able to command the motor(s) to move the door to the OPEN position.
TEST_CASE("SR5: TC1 - The door can be opened without pressing the button.") {
    std::cout << "SR5: TC1 - The door can be opened without pressing the button.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    door.set_next_door_position(Door::Position::OPENING);
    fast_forward(door);
    assert_position(door, Door::Position::OPEN);
}

// SR6: The Automatic Door Opener system shall be able to command the motor(s) to move the door to the CLOSED position.
TEST_CASE("SR6: TC1 - The door can be closed without pressing the button.") {
    std::cout << "SR6: TC1 - The door can be closed without pressing the button.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    door.set_next_door_position(Door::Position::CLOSING);
    fast_forward(door);
    assert_position(door, Door::Position::CLOSED);
}

// SR7: At initialization, the Automatic Door Opener system shall cause the door to OPEN if the door is not in either CLOSE or OPEN position.
TEST_CASE("SR7: TC1 - If the door is closing, initializing the door will fully open it.") {
    std::cout << "SR7: TC1 - If the door is closing, initializing the door will fully open it.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSING);
    fast_forward(door);
    assert_position(door, Door::Position::OPEN);
}

TEST_CASE("SR7: TC2 - If the door is opening, initializing the door will fully open it.") {
    std::cout << "SR7: TC2 - If the door is opening, initializing the door will fully open it.\n";
    Door door = create_and_initialize_door(Door::Position::OPENING);
    fast_forward(door);
    assert_position(door, Door::Position::OPEN);
}

// Logical Assumptions
// Assumption 1: The ADOS shall ignore any incoming requests if the door is moving.
TEST_CASE("A1: TC1 - If the door is closing, it cannot be opened.") {
    std::cout << "A1: TC1 - If the door is closing, it cannot be opened.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    door.set_next_door_position(Door::Position::CLOSING);
    assert_position(door, Door::Position::CLOSING);
    door.set_next_door_position(Door::Position::OPENING);
    assert_position(door, Door::Position::CLOSING);
}

TEST_CASE("A1: TC2 - If the door is closing, it cannot be closed.") {
    std::cout << "A1: TC2 - If the door is closing, it cannot be closed.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    door.set_next_door_position(Door::Position::CLOSING);
    assert_position(door, Door::Position::CLOSING);
    int start = door.calculate_percent_open(std::chrono::milliseconds((Door::MOVEMENT_DURATION_S * 1000) / 4));
    door.set_next_door_position(Door::Position::CLOSING);
    int end = door.calculate_percent_open(std::chrono::milliseconds((Door::MOVEMENT_DURATION_S * 1000) / 2));
    assert_position(door, Door::Position::CLOSING);
    REQUIRE(start > end);
}

TEST_CASE("A1: TC3 - If the door is opening, it cannot be closed.") {
    std::cout << "A1: TC3 - If the door is opening, it cannot be closed.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    door.set_next_door_position(Door::Position::OPENING);
    assert_position(door, Door::Position::OPENING);
    door.set_next_door_position(Door::Position::CLOSING);
    assert_position(door, Door::Position::OPENING);
}

TEST_CASE("A1: TC4 - If the door is opening, it cannot be opened.") {
    std::cout << "A1: TC4 - If the door is opening, it cannot be opened.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    door.set_next_door_position(Door::Position::OPENING);
    assert_position(door, Door::Position::OPENING);
    int start = door.calculate_percent_open(std::chrono::milliseconds((Door::MOVEMENT_DURATION_S * 1000) / 4));
    door.set_next_door_position(Door::Position::OPENING);
    int end = door.calculate_percent_open(std::chrono::milliseconds((Door::MOVEMENT_DURATION_S * 1000) / 2));
    assert_position(door, Door::Position::OPENING);
    REQUIRE(start < end);
}

// Assumption 2: The ADOS shall not attempt to open an already open door.
TEST_CASE("A2: TC1 - If the door is open, opening it again will do nothing.") {
    std::cout << "A2: TC1 - If the door is open, opening it again will do nothing.\n";
    Door door = create_and_initialize_door(Door::Position::OPEN);
    door.set_next_door_position(Door::Position::OPENING);
    assert_position(door, Door::Position::OPEN);
}

// Assumption 3: The ADOS shall not attempt to close an already closed door.
TEST_CASE("A3: TC1 - If the door is closed, closing it again will do nothing.") {
    std::cout << "A3: TC1 - If the door is closed, closing it again will do nothing.\n";
    Door door = create_and_initialize_door(Door::Position::CLOSED);
    door.set_next_door_position(Door::Position::CLOSING);
    assert_position(door, Door::Position::CLOSED);
}

// Helper functions
Door create_and_initialize_door(Door::Position starting_position) {
    Door door(starting_position);
    std::cout << "\tDoor constructed with position \"" << door.position_to_string(starting_position) << ".\"\n";
    door.initialize();
    std::cout << "\tDoor initialized.\n";
    return door;
}

void fast_forward(Door& door, int ms_to_pass) {
    door.update(std::chrono::milliseconds(ms_to_pass));
    std::cout << "\tFast forward " << ms_to_pass << "ms.\n";
}

void assert_position(Door& door, Door::Position desired_position) {
    Door::Position current_position = door.get_current_door_position();
    std::cout << "\tDoor position: \"" << door.position_to_string(current_position) << ".\"\n";
    REQUIRE(current_position == desired_position);
}

void assert_position_changes(Door& door) {
    Door::Position current_position = door.get_current_door_position();
    int start = door.calculate_percent_open();
    int end = door.calculate_percent_open(std::chrono::milliseconds((Door::MOVEMENT_DURATION_S * 1000) / 2));
    if (current_position == Door::Position::OPENING) {
        REQUIRE(start < end);
    }
    else {
        REQUIRE(start > end);
    }
}

void press_button(Door& door) {
    door.press_button();
    std::cout << "\tButton pressed.\n";
}