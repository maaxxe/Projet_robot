#include "pilot.h"
#include "robot.h"
#include "mrpiz.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define U_TURN_TARGET_POS 456  // U-turn duration
#define DEFAULT_TARGET_POS 200  // Default target position
#define OBSTACLE_DISTANCE_THRESHOLD 150  // Obstacle distance threshold

static move_status_t robot_moving;  // Current movement status of the robot
static int target_pos;  // Movement duration 2D 228

// Function to start the robot's movement based on the given move direction and speed
void pilot_start_move(move_t a_move) {
    int speed_left = 0, speed_right = 0;

    // Determine the movement direction and set the speeds accordingly
    switch (a_move.direction) {
        case FORWARD:
            printf("FORWARD\n");
            speed_left = a_move.speed;
            speed_right = a_move.speed;
            robot_moving = MOVE_FORWARDING;
            break;

        case ROTATION:
            // Determine the rotation direction and set the speeds accordingly
            switch (a_move.parameters[0]) {
                case RIGHT:
                    printf("RIGHT\n");
                    speed_left = a_move.speed;
                    speed_right = -a_move.speed;
                    robot_moving = MOVE_TURNING;
                    target_pos=DEFAULT_TARGET_POS;

                    break;

                case LEFT:
                    printf("LEFT\n");
                    speed_left = -a_move.speed;
                    speed_right = a_move.speed;
                    robot_moving = MOVE_TURNING;
                    target_pos=DEFAULT_TARGET_POS;
                    break;

                case U_TURN:
                    printf("U_TURN\n");
                    speed_left = a_move.speed;
                    speed_right = -a_move.speed;
                    robot_moving = MOVE_TURNING;
                    target_pos = U_TURN_TARGET_POS;  // Set target position for U-turn
                    break;

                default:
                    printf("Unknown ROTATION parameter\n");
                    robot_moving = MOVE_DONE;
                    return;
            }
            break;

        default:
            printf("Unknown move direction\n");
            robot_moving = MOVE_DONE;
            return;
    }

    // Set the robot's speed based on the calculated values
    robot_set_speed(speed_left, speed_right);
}

// Function to stop the robot when it reaches the target position or detects an obstacle
move_status_t pilot_stop_at_target(void) {
    robot_status_t status = robot_get_status();  // Get the current status of the robot

    // Check if the robot has reached the target position
    if (status.left_encoder >= target_pos || status.right_encoder >= target_pos) {
        printf("Stopped\n");
        robot_moving = MOVE_DONE;
        robot_reset_wheel_pos();  // Reset the wheel positions
        robot_set_speed(0, 0);  // Stop the robot
        target_pos = DEFAULT_TARGET_POS;  // Reset the target position
    } else if (status.left_sensor < OBSTACLE_DISTANCE_THRESHOLD ||
               status.center_sensor < OBSTACLE_DISTANCE_THRESHOLD ||
               status.right_sensor < OBSTACLE_DISTANCE_THRESHOLD) {
        robot_moving = MOVE_OBSTACLE_FORWARD;  // Change status if an obstacle is detected
    }
    return robot_moving;
}

// Function to get the current movement status of the robot
move_status_t pilot_get_status(void) {
    return robot_moving;
}

// Function to handle dead angles by forcing a left movement
void handle_dead_angle(void) {
    printf("Angle mort détecté ! Forçage d'un déplacement vers la gauche.\n");
    robot_set_speed(-30, 30);  // Force a left turn
    usleep(500000);  // Wait for 500 milliseconds

    robot_status_t status = robot_get_status();  // Get the current status of the robot
    if (status.right_sensor < OBSTACLE_DISTANCE_THRESHOLD) {
        printf("Mur retrouvé à droite, reprise du suivi.\n");
        return;  // Resume following the wall if the right sensor detects an obstacle
    }

    // If still blocked, perform a forced U-turn
    printf("Toujours bloqué, demi-tour forcé.\n");
    robot_set_speed(-30, -30);  // Move backward
    usleep(500000);  // Wait for 500 milliseconds
    robot_set_speed(30, -30);  // Turn around
    usleep(1000000);  // Wait for 1 second
    robot_set_speed(30, 30);  // Move forward
}

// Function to follow the right wall based on sensor readings
void follow_right_wall(void) {
    robot_status_t status = robot_get_status();  // Get the current status of the robot

    // Print the sensor readings
    printf("Capteurs -> Gauche: %d, Devant: %d, Droite: %d\n",
           status.left_sensor, status.center_sensor, status.right_sensor);

    // Determine if the path is clear based on sensor readings
    int right_clear = status.right_sensor > OBSTACLE_DISTANCE_THRESHOLD;
    int front_clear = status.center_sensor > OBSTACLE_DISTANCE_THRESHOLD;
    int left_clear = status.left_sensor > OBSTACLE_DISTANCE_THRESHOLD;

    // Decide the movement based on the sensor readings
    if (right_clear) {
        printf("Tourne à droite\n");
        robot_set_speed(30, -30);  // Turn right
        usleep(500000);  // Wait for 500 milliseconds
        robot_set_speed(30, 30);  // Move forward
    } else if (front_clear) {
        printf("Avance tout droit\n");
        robot_set_speed(30, 30);  // Move forward
    } else if (left_clear) {
        printf("Tourne à gauche\n");
        robot_set_speed(-30, 30);  // Turn left
        usleep(500000);  // Wait for 500 milliseconds
        robot_set_speed(30, 30);  // Move forward
    } else {
        handle_dead_angle();  // Handle dead angle if all paths are blocked
    }
}
