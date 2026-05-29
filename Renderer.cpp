#include "Renderer.h"
#include "Door.h"

Renderer::Renderer() {
    // Create welcome windows
    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);
    this->welcome_window = newwin(screen_height * 0.5, screen_width, 0, 0);
    int welcome_h, welcome_w;
    getmaxyx(welcome_window, welcome_h, welcome_w);
    this->welcome_content = derwin(welcome_window, welcome_h - 2, welcome_w - 3, 1, 2);

    // Create log windows
    this->log_window = newwin(screen_height * 0.5, screen_width * 0.5, screen_height * 0.5, 0);
    int log_h, log_w;
    getmaxyx(log_window, log_h, log_w);
    this->log_content = derwin(log_window, log_h - 2, log_w - 3, 1, 2);
    scrollok(log_content, true);

    // Create door windows
    this->door_window = newwin(screen_height * 0.5, screen_width * 0.5, screen_height * 0.5, screen_width * 0.5);
    int door_h, door_w;
    getmaxyx(door_window, door_h, door_w);
    this->door_content = derwin(door_window, door_h - 2, door_w - 4, 1, 3);

    // Draw borders around the windows
    box(welcome_window, 0, 0);
    box(log_window, 0, 0);
    box(door_window, 0, 0);
}

void Renderer::draw_welcome() {
    std::string welcome_message = "Welcome to the Automatic Door Opener System!";
    std::string dash_border(welcome_message.length(), '-');
    wprintw(this->welcome_content, "%s\n%s\n%s\n\n", dash_border.c_str(), welcome_message.c_str(), dash_border.c_str());
    wprintw(this->welcome_content, "This Automatic Door Opener System (ADOS) follows the seven rules below:\n\n");
    wprintw(this->welcome_content, "\t1. When the button is pressed, the ADOS shall cause the door to CLOSE if it is currently in the OPEN position.\n");
    wprintw(this->welcome_content, "\t2. When the button is pressed, the ADOS shall cause the door to OPEN if it is currently in the CLOSE position.\n");
    wprintw(this->welcome_content, "\t3. When the button is pressed, the ADOS shall ignore the button press if the door is currently MOVING.\n");
    wprintw(this->welcome_content, "\t4. The ADOS shall be able to monitor door position.\n");
    wprintw(this->welcome_content, "\t5. The ADOS shall be able to command the motor(s) to move the door to the OPEN position.\n");
    wprintw(this->welcome_content, "\t6. The ADOS shall be able to command the motor(s) to move the door to the CLOSE position.\n");
    wprintw(this->welcome_content, "\t7. At initialization, the ADOS shall cause the door to OPEN if the door is not in either CLOSE or OPEN position.\n\n");
    wprintw(this->welcome_content, "The controls for the door are described below:\n\n");
    wprintw(this->welcome_content, "\t%8s - presses \"the button\"\n", "SPACEBAR");
    wprintw(this->welcome_content, "\t%8s - gets and prints the current position of the door\n", "G");
    wprintw(this->welcome_content, "\t%8s - opens the door\n", "O");
    wprintw(this->welcome_content, "\t%8s - closes the door\n", "C");
    wprintw(this->welcome_content, "\t%8s - quits the program\n\n", "Q");
}

void Renderer::draw_frame(Door& door) {
    // Erase and redraw the door window
    werase(this->door_content);
    werase(this->door_window);
    box(this->door_window, 0, 0);
    draw_door(door);
    
    // Refresh all windows
    wrefresh(this->welcome_window);
    wrefresh(this->welcome_content);
    wrefresh(this->door_window);
    wrefresh(this->door_content);
    wrefresh(this->log_window);
    wrefresh(this->log_content);
}

void Renderer::delete_windows() {
    delwin(this->welcome_window);
    delwin(this->welcome_content);
    delwin(this->log_window);
    delwin(this->log_content);
    delwin(this->door_window);
    delwin(this->door_content);
}

void Renderer::add_log_message(std::string message) {
    wprintw(this->log_content, message.c_str());
}

void Renderer::draw_door(Door& door) {
    Door::Position current_position = door.get_current_door_position();
    if (current_position != Door::Position::OPENING && current_position != Door::Position::CLOSING) {
        draw_sprite((current_position == Door::Position::OPEN) ? (100) : (0));
        return;
    }

    std::chrono::steady_clock::time_point movement_start_time = door.get_movement_start_time();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<float>(now - movement_start_time);

    if (current_position == Door::Position::OPENING) {
        draw_sprite(elapsed.count() * 100);
    }
    else {
        draw_sprite(100 - (elapsed.count() * 100));
    }
}

void Renderer::draw_sprite(int percent_open) {
    if (percent_open == 0) {
        wprintw(this->door_content, "%s", DOOR_OPEN_0);
    }
    else if (percent_open > 0 && percent_open < 33) {
        wprintw(this->door_content, "%s", DOOR_OPEN_25);
    }
    else if (percent_open >= 33 && percent_open < 66) {
        wprintw(this->door_content, "%s", DOOR_OPEN_50);
    }
    else if (percent_open >= 66 && percent_open < 100) {
        wprintw(this->door_content, "%s", DOOR_OPEN_75);
    }
    else if (percent_open == 100) {
        wprintw(this->door_content, "%s", DOOR_OPEN_100);
    }
}