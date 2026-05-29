#include "Door.h"
#include "Renderer.h"

void handle_button_pressed(Door& door, Renderer& renderer);
void handle_get_current_door_position(Door& door, Renderer& renderer);
void handle_open_door(Door& door, Renderer& renderer);
void handle_close_door(Door& door, Renderer& renderer);

int main() {
    // Capture user input as they type
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);

    // Create and initialize the door and renderer
    Door door = Door();
    Renderer renderer = Renderer();
    renderer.draw_welcome();
    door.initialize();

    // Capture user input and call the appropriate handler based on the character pressed
    while (true) {
        int ch = getch();
        if (ch == 'q') {
            break;
        }
        else if (ch == ' ') {
            handle_button_pressed(door, renderer);
        }
        else if (ch == 'g') {
            handle_get_current_door_position(door, renderer);
        }
        else if (ch == 'o') {
            handle_open_door(door, renderer);
        }
        else if (ch == 'c') {
            handle_close_door(door, renderer);
        }

        // Process the door's movement and redraw its panel
        door.update();
        renderer.draw_frame(door);

        // Wait 16 ms (~60fps)
        napms(16);
    }

    // End
    renderer.delete_windows();
    endwin();
    return 0;
}

void handle_button_pressed(Door& door, Renderer& renderer) {
    Door::Position previous_position = door.get_current_door_position();
    door.press_button();
    Door::Position current_position = door.get_current_door_position();
    if (previous_position == current_position) {
        renderer.add_log_message("RULE 3: The button press was ignored since the door is currently moving.\n");
    }
    else if (current_position == Door::Position::CLOSING) {
        renderer.add_log_message("RULE 1: Pressing the button closed the door.\n");
    }
    else if (current_position == Door::Position::OPENING) {
        renderer.add_log_message("RULE 2: Pressing the button opened the door.\n");
    }
}

void handle_get_current_door_position(Door& door, Renderer& renderer) {
    Door::Position current_position = door.get_current_door_position();
    std::string current_position_string = "";
    switch (current_position) {
        case Door::Position::OPEN:
            current_position_string = "open";
            break;
        case Door::Position::OPENING:
            current_position_string = "opening";
            break;
        case Door::Position::CLOSED:
            current_position_string = "closed";
            break;
        case Door::Position::CLOSING:
            current_position_string = "closing";
            break;
        default:
            break;
    }
    if (current_position_string != "") {
        renderer.add_log_message("RULE 4: The door is currently " + current_position_string + ".\n");
    }
}

void handle_open_door(Door& door, Renderer& renderer) {
    Door::Position previous_position = door.get_current_door_position();
    door.set_next_door_position(Door::Position::OPENING);
    Door::Position current_position = door.get_current_door_position();
    if (previous_position != current_position) {
        renderer.add_log_message("RULE 5: Opening the door.\n");
    }
    else {
        if (previous_position == Door::Position::OPENING || previous_position == Door::Position::CLOSING) {
            renderer.add_log_message("ASSUMPTION: The ADOS shall ignore any incoming requests if the door is moving.\n");    
        }
        else if (previous_position == Door::Position::OPEN) {
            renderer.add_log_message("ASSUMPTION: The ADOS shall not attempt to open an already open door.\n");
        }
    }
}

void handle_close_door(Door& door, Renderer& renderer) {
    Door::Position previous_position = door.get_current_door_position();
    door.set_next_door_position(Door::Position::CLOSING);
    Door::Position current_position = door.get_current_door_position();
    if (previous_position != current_position) {
        renderer.add_log_message("RULE 6: Closing the door.\n");
    }
    else {
        if (previous_position == Door::Position::OPENING || previous_position == Door::Position::CLOSING) {
            renderer.add_log_message("ASSUMPTION: The ADOS shall ignore any incoming requests if the door is moving.\n");    
        }
        else if (previous_position == Door::Position::CLOSED) {
            renderer.add_log_message("ASSUMPTION: The ADOS shall not attempt to close an already closed door.\n");
        }
    }
}