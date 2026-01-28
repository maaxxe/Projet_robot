#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include "pilot.h"
#include "robot.h"
#include "copilot.h"

/** @brief Number of steps (or moves) in the path. */
#define STEPS_NUMBER 12
/** @brief Waiting time between two encoder scans (in microseconds). So 10 000 for 2D and 1 000 000 for 3D */
#define DELAY 100000
/** @brief Maximum number of attempts to check if a move is finished. */
#define ENCODERS_SCAN_NB 1000
/** @brief Distance for each move. */
#define DISTANCE 100
/** @brief Obstacle distance threshold. */
#define OBSTACLE_DISTANCE_THRESHOLD 50

/**
 * @enum app_state_t
 * @brief Enumeration of the application states.
 */
typedef enum {
    STATE_SELECT_PATH,     /**< State for selecting the path. */
    STATE_EXECUTE_PATH,    /**< State for executing the path. */
    STATE_CHECK_COMPLETION,/**< State for checking path completion. */
    STATE_FOLLOW_WALL      /**< State for following the wall. */
} app_state_t;

/**
 * @brief Initializes the paths with the given speed.
 * 
 * @param speed The speed to set for the paths.
 */
void initialize_paths(int speed);

/**
 * @brief Gets the path based on the user's choice.
 * 
 * @param path_choice The chosen path.
 * @param steps Pointer to store the number of steps in the path.
 * @param speed The speed to set for the path.
 * @return Pointer to the chosen path.
 */
move_t* get_path(int path_choice, int *steps, int speed);

/**
 * @brief Displays the robot's status.
 * 
 * @param status The current status of the robot.
 */
void display_robot_status(robot_status_t status);

/**
 * @brief Checks if the path execution is completed.
 * 
 * @return 1 if the path is completed, 0 otherwise.
 */
int check_path_completion(void);

#endif // APP_MANAGER_H