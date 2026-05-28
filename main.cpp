#include <iostream>
#include <iomanip>
#include <ncurses.h>

#include "Door.h"

using namespace std;

void print_welcome_message(WINDOW* welcome_content);
void print_rules(WINDOW* welcome_content);
void print_controls(WINDOW* welcome_content);
void handle_button_pressed(Door& door, WINDOW* log_content);
void handle_get_current_door_position(Door& door, WINDOW* log_content);
void handle_open_door(Door& door, WINDOW* log_content);
void handle_close_door(Door& door, WINDOW* log_content);
void draw_door(Door& door, WINDOW* door_content);
void draw_sprite(int percent_open, WINDOW* door_content);

int main() {
    // Capture user input as they type
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);

    // Create welcome windows
    WINDOW* welcome_window, *welcome_content;
    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);
    welcome_window = newwin(screen_height * 0.5, screen_width, 0, 0);
    int welcome_h, welcome_w;
    getmaxyx(welcome_window, welcome_h, welcome_w);
    welcome_content = derwin(welcome_window, welcome_h - 2, welcome_w - 3, 1, 2);
    
    // Create door windows
    WINDOW* door_window, *door_content;
    door_window = newwin(screen_height * 0.5, screen_width * 0.5, screen_height * 0.5, screen_width * 0.5);
    int door_h, door_w;
    getmaxyx(door_window, door_h, door_w);
    door_content = derwin(door_window, door_h - 2, door_w - 4, 1, 3);

    // Create log windows
    WINDOW* log_window, *log_content;
    log_window = newwin(screen_height * 0.5, screen_width * 0.5, screen_height * 0.5, 0);
    int log_h, log_w;
    getmaxyx(log_window, log_h, log_w);
    wsetscrreg(log_window, 1, log_h - 2);
    log_content = derwin(log_window, log_h - 2, log_w - 3, 1, 2);
    scrollok(log_content, true);

    // Draw borders around the windows
    box(welcome_window, 0, 0);
    box(log_window, 0, 0);
    box(door_window, 0, 0);

    // Welcome the user, introduce the seven rules, and inform them of the controls
    print_welcome_message(welcome_content);
    print_rules(welcome_content);
    print_controls(welcome_content);

    // Create and initialize the door
    Door door = Door();
    door.initialize();

    // Capture user input and act accordingly
    while (true) {
        
        // Call the appropriate handler based on the character pressed
        int ch = getch();
        if (ch == 'q') {
            break;
        }
        else if (ch == ' ') {
            handle_button_pressed(door, log_content);
        }
        else if (ch == 'g') {
            handle_get_current_door_position(door, log_content);
        }
        else if (ch == 'o') {
            handle_open_door(door, log_content);
        }
        else if (ch == 'c') {
            handle_close_door(door, log_content);
        }

        // Process the door's movement and redraw its panel
        door.update();
        werase(door_content);
        werase(door_window);
        box(door_window, 0, 0);
        draw_door(door, door_content);
        
        // Refresh windows
        wrefresh(welcome_window);
        wrefresh(welcome_content);
        wrefresh(door_window);
        wrefresh(door_content);
        wrefresh(log_window);
        wrefresh(log_content);

        // Wait 16 ms (~60fps)
        napms(16);
    }

    // End
    delwin(welcome_window);
    delwin(log_window);
    delwin(door_window);
    delwin(welcome_content);
    delwin(log_content);
    delwin(door_content);
    endwin();
    return 0;
}

void print_welcome_message(WINDOW* welcome_content) {
    string welcome_message = "Welcome to the Automatic Door Opener System!";
    string dash_border(welcome_message.length(), '-');
    wprintw(welcome_content, "%s\n%s\n%s\n\n", dash_border.c_str(), welcome_message.c_str(), dash_border.c_str());
}

void print_rules(WINDOW* welcome_content) {
    wprintw(welcome_content, "This Automatic Door Opener System (ADOS) follows the seven rules below:\n\n");
    wprintw(welcome_content, "\t1. When the button is pressed, the ADOS shall cause the door to CLOSE if it is currently in the OPEN position.\n");
    wprintw(welcome_content, "\t2. When the button is pressed, the ADOS shall cause the door to OPEN if it is currently in the CLOSE position.\n");
    wprintw(welcome_content, "\t3. When the button is pressed, the ADOS shall ignore the button press if the door is currently MOVING.\n");
    wprintw(welcome_content, "\t4. The ADOS shall be able to monitor door position.\n");
    wprintw(welcome_content, "\t5. The ADOS shall be able to command the motor(s) to move the door to the OPEN position.\n");
    wprintw(welcome_content, "\t6. The ADOS shall be able to command the motor(s) to move the door to the CLOSE position.\n");
    wprintw(welcome_content, "\t7. At initialization, the ADOS shall cause the door to OPEN if the door is not in either CLOSE or OPEN position.\n\n");
}

void print_controls(WINDOW* welcome_content) {
    wprintw(welcome_content, "The controls for the door are described below:\n\n");
    wprintw(welcome_content, "\t%8s - presses \"the button\"\n", "SPACEBAR");
    wprintw(welcome_content, "\t%8s - gets and prints the current position of the door\n", "G");
    wprintw(welcome_content, "\t%8s - opens the door\n", "O");
    wprintw(welcome_content, "\t%8s - closes the door\n", "C");
    wprintw(welcome_content, "\t%8s - quits the program\n\n", "Q");
}

void handle_button_pressed(Door& door, WINDOW* log_content) {
    Door::Position previous_position = door.get_current_door_position();
    door.press_button();
    Door::Position current_position = door.get_current_door_position();
    if (previous_position == current_position) {
        wprintw(log_content, "RULE 3: The button press was ignored since the door is currently moving.\n");
    }
    else if (current_position == Door::Position::CLOSING) {
        wprintw(log_content, "RULE 1: Pressing the button closed the door.\n");
    }
    else if (current_position == Door::Position::OPENING) {
        wprintw(log_content, "RULE 2: Pressing the button opened the door.\n");
    }
}

void handle_get_current_door_position(Door& door, WINDOW* log_content) {
    Door::Position current_position = door.get_current_door_position();
    string current_position_string = "";
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
        wprintw(log_content, "RULE 4: The door is currently %s.\n", current_position_string.c_str());
    }
}

void handle_open_door(Door& door, WINDOW* log_content) {
    Door::Position previous_position = door.get_current_door_position();
    door.set_next_door_position(Door::Position::OPENING);
    Door::Position current_position = door.get_current_door_position();
    if (previous_position != current_position) {
        wprintw(log_content, "RULE 5: Opening the door.\n");
    }
    else {
        if (previous_position == Door::Position::OPENING || previous_position == Door::Position::CLOSING) {
            wprintw(log_content, "ASSUMPTION: The ADOS shall ignore any incoming requests if the door is moving.\n");    
        }
        else if (previous_position == Door::Position::OPEN) {
            wprintw(log_content, "ASSUMPTION: The ADOS shall not attempt to open an already open door.\n");
        }
    }
}

void handle_close_door(Door& door, WINDOW* log_content) {
    Door::Position previous_position = door.get_current_door_position();
    door.set_next_door_position(Door::Position::CLOSING);
    Door::Position current_position = door.get_current_door_position();
    if (previous_position != current_position) {
        wprintw(log_content, "RULE 6: Closing the door.\n");
    }
    else {
        if (previous_position == Door::Position::OPENING || previous_position == Door::Position::CLOSING) {
            wprintw(log_content, "ASSUMPTION: The ADOS shall ignore any incoming requests if the door is moving.\n");    
        }
        else if (previous_position == Door::Position::CLOSED) {
            wprintw(log_content, "ASSUMPTION: The ADOS shall not attempt to close an already closed door.\n");
        }
    }
}

void draw_door(Door& door, WINDOW* door_content) {
    Door::Position current_position = door.get_current_door_position();
    if (current_position != Door::Position::OPENING && current_position != Door::Position::CLOSING) {
        draw_sprite((current_position == Door::Position::OPEN) ? (100) : (0), door_content);
        return;
    }

    std::chrono::steady_clock::time_point movement_start_time = door.get_movement_start_time();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<float>(now - movement_start_time);

    if (current_position == Door::Position::OPENING) {
        draw_sprite(elapsed.count() * 100, door_content);
    }
    else {
        draw_sprite(100 - (elapsed.count() * 100), door_content);
    }
}

void draw_sprite(int percent_open, WINDOW* door_content) {
    if (percent_open == 0) {
        wprintw(door_content, "%s", Door::DOOR_OPEN_0);
    }
    else if (percent_open > 0 && percent_open < 33) {
        wprintw(door_content, "%s", Door::DOOR_OPEN_25);
    }
    else if (percent_open >= 33 && percent_open < 66) {
        wprintw(door_content, "%s", Door::DOOR_OPEN_50);
    }
    else if (percent_open >= 66 && percent_open < 100) {
        wprintw(door_content, "%s", Door::DOOR_OPEN_75);
    }
    else if (percent_open == 100) {
        wprintw(door_content, "%s", Door::DOOR_OPEN_100);
    }
}