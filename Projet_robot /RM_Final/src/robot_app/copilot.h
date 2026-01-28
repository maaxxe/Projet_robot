#ifndef COPILOT_H
#define COPILOT_H

#include <stdbool.h>
#include "pilot.h" // Defines move_t structure

/**
 * @brief Enum representing the status of a movement path.
 */
typedef enum {
    PATH_NOT_STARTED,  // The path has not started yet
    PATH_IN_PROGRESS,  // The path is currently executing
    PATH_COMPLETED     // The path execution is finished
} path_status_t;

/**
 * @brief Starts the movement along the predefined path.
 * The function ensures a valid path is set before execution.
 */
void copilot_start_path(void);

/**
 * @brief Stops movement upon reaching the target step.
 * @return The current status of the path execution.
 */
path_status_t copilot_stop_at_step_completion(void);

/**
 * @brief Checks if the path execution is complete.
 * @return true if the path is completed, false otherwise.
 */
bool copilot_is_path_completed(void);

/**
 * @brief Sets a movement path for the copilot to follow.
 * @param path Pointer to the movement sequence.
 * @param steps Number of steps in the path.
 */
void copilot_set_path(move_t *path, int steps);

#endif // COPILOT_H
