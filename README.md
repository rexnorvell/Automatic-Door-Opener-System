# Automatic-Door-Opener-System

## About
This is a C++ example implementation of a software solution for a case study involving developing an **Automatic Door Opener System**.

## User Interface
The application has three main sections: the **welcome**, **logging**, and **door** sections. Each section is separated by a line border. 

- `Welcome` - The **welcome** section at the top of the window simply introduces the user to the application. It features a legend for the keys the user can press to take different actions. 

- `Logging` - The **logging** section at the bottom left of the window describes the steps taken by the system as the user performs different actions. Each action traces back to one of the **seven system requirements** or is a stated logical assumption.

- `Door` - The **door** section at the bottom right of the window displays a simple ASCII art representation of the door's current state. It features sprite-based animations to represent the door opening and closing.

## System Requirements
There are seven explicit system requirements.

1. When the button is pressed, the Automatic Door Opener system shall cause the door to CLOSE if it is currently in the OPEN position.
2. When the button is pressed, the Automatic Door Opener system shall cause the door to OPEN if it is currently in the CLOSE position.
3. When the button is pressed, the Automatic Door Opener system shall ignore the button press if the door is currently MOVING.
4. The Automatic Door Opener system shall be able to monitor door position.
5. The Automatic Door Opener system shall be able to command the motor(s) to move the door to the OPEN position.
6. The Automatic Door Opener system shall be able to command the motor(s) to move the door to the CLOSE position.
7. At initialization, the Automatic Door Opener system shall cause the door to OPEN if the door is not in either CLOSE or OPEN position.

Any other implementation details regarding logical decisions appear as stated assumptions in the application's logging.