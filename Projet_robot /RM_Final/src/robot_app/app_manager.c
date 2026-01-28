#include "app_manager.h"
#include <stdio.h>
#include <unistd.h>

// Arrays to store different paths
move_t path1[STEPS_NUMBER];
move_t path2[STEPS_NUMBER];
move_t path3[1];
move_t path4[1];
move_t path5[1];
move_t path6[1];

// Function to initialize paths with given speed
void initialize_paths(int speed) {
    
    for (int i = 0; i < STEPS_NUMBER; i++) {
        path1[i] = (move_t){FORWARD, {DISTANCE, 0}, speed};
        path2[i] = (move_t){FORWARD, {DISTANCE, 0}, speed};
        if (i % 2 == 1) {
            
            path1[i].direction = ROTATION;
            path2[i].direction = ROTATION;
            path1[i].parameters[0] = (i % 4 == 1) ? RIGHT : LEFT;
            path2[i].parameters[0] = (i % 4 == 1) ? RIGHT : LEFT;
        }
    }
    path3[0] = (move_t){FORWARD, {DISTANCE, 0}, speed};
    path4[0] = (move_t){ROTATION, {RIGHT, 0}, speed};
    path5[0] = (move_t){ROTATION, {LEFT, 0}, speed};
    path6[0] = (move_t){ROTATION, {U_TURN, 0}, speed};
}

// Function to get the path based on the user's choice
move_t* get_path(int path_choice, int *steps, int speed) {

    robot_status_t status = robot_get_status();  // Get the current status of the robot

    // Check if the path is clear based on sensor readings
    int front_clear = status.center_sensor > OBSTACLE_DISTANCE_THRESHOLD;

    initialize_paths(speed);  // Initialize paths with the given speed
    switch (path_choice) {
        case 7:
            *steps = STEPS_NUMBER;
            return path1;
        case 9:
            *steps = STEPS_NUMBER;
            return path2;
        case 8:
            if (front_clear ) {
                *steps = 1;
                return path3;
            } else {
                return NULL;
            }
        case 6:
            *steps = 1;
            return path4;
        case 4:
            *steps = 1;
            return path5;
        case 2:
            *steps = 1;
            return path6;
        default:
            return NULL;
    }
}

// Function to display the robot's status
void display_robot_status(robot_status_t status) {
    fprintf(stdout, "Encoders: left = %d, right = %d\n", status.left_encoder, status.right_encoder);
    fprintf(stdout, "Proximity sensors: left = %d, center = %d, right = %d\n", status.left_sensor, status.center_sensor, status.right_sensor);
    fprintf(stdout, "Battery: %d%%\n", status.battery);
}

// Function to check if the path execution is completed
int check_path_completion(void) {
    for (int i = 0; i < ENCODERS_SCAN_NB; i++) {
        usleep(DELAY);  // Wait for a specified delay
        if (copilot_stop_at_step_completion() == PATH_COMPLETED) {
            printf("All steps completed.\n");
            return 1;
        }
    }
    return copilot_is_path_completed();
}