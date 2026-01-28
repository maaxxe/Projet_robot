#ifndef PILOT_H
#define PILOT_H

/**
 * @enum move_status_t
 * @brief Enumeration of the movement statuses.
 */
typedef enum {
    MOVE_FORWARDING,      /**< The robot is moving forward. */
    MOVE_TURNING,         /**< The robot is turning. */
    MOVE_DONE,            /**< The movement is completed. */
    MOVE_OBSTACLE_FORWARD /**< The robot is moving forward to avoid an obstacle. */
} move_status_t;

/**
 * @enum move_type_t
 * @brief Enumeration of the movement types.
 */
typedef enum {
    FORWARD,  /**< Move forward. */
    ROTATION, /**< Rotate. */
    U_TURN    /**< U-turn. */
} move_type_t;

/**
 * @enum rotation_direction_t
 * @brief Enumeration of the rotation directions.
 */
typedef enum {
    LEFT,  /**< Rotate left. */
    RIGHT  /**< Rotate right. */
} rotation_direction_t;

/**
 * @struct move_t
 * @brief Structure representing a movement.
 */
typedef struct {
    move_type_t direction; /**< The direction of the movement. */
    int parameters[2];     /**< Parameters for the movement. */
    int speed;             /**< Speed of the movement. */
} move_t;

/**
 * @brief Starts the robot's movement based on the given move.
 * 
 * @param move The move to be executed.
 */
void pilot_start_move(move_t move);

/**
 * @brief Stops the robot when it reaches the target position.
 * 
 * @return The current movement status.
 */
move_status_t pilot_stop_at_target(void);

/**
 * @brief Gets the current movement status of the robot.
 * 
 * @return The current movement status.
 */
move_status_t pilot_get_status(void);

/**
 * @brief Handles dead angles by forcing a left movement.
 */
void handle_dead_angle(void);

/**
 * @brief Follows the right wall based on sensor readings.
 */
void follow_right_wall(void);

#endif // PILOT_H