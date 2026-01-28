#ifndef ROBOT_H
#define ROBOT_H

/**
 * @file robot.h
 * @brief Declaration of the Robot class.
 */

/**
 * @typedef speed_pct_t
 * @brief Defines a type for speed percentage values.
 */
typedef int speed_pct_t;

/**
 * @enum wheel_t
 * @brief Enumeration of wheel identifiers.
 */
typedef enum {
    LEFT_WHEEL    = 0,   /**< Left wheel identifier */
    RIGHT_WHEEL   = 1,   /**< Right wheel identifier */
    BOTH_WHEEL    = 2    /**< Both wheels identifier */
} wheel_t;

/**
 * @struct robot_status_t
 * @brief Structure to hold robot status information.
 */
typedef struct {
    int left_encoder;   /**< Position of the left wheel encoder */
    int right_encoder;  /**< Position of the right wheel encoder */
    int left_sensor;    /**< Value of the left proximity sensor */
    int center_sensor;  /**< Value of the center proximity sensor */
    int right_sensor;   /**< Value of the right proximity sensor */
    int battery;        /**< Battery level */
} robot_status_t;

/**
 * @enum notification_t
 * @brief Enumeration of robot notification events.
 */
typedef enum {
    ROBOT_IDLE,      /**< Robot is idle */
    ROBOT_OK,        /**< Robot is functioning normally */
    ROBOT_OBSTACLE,  /**< Robot has detected an obstacle */
    ROBOT_PROBLEM    /**< Robot encountered a problem */
} notification_t;

/**
 * @brief Initializes and starts the robot.
 *
 * @return The initialization status.
 */
int robot_start(void);

/**
 * @brief Sets the speed of the robot's wheels.
 *
 * @param left Speed percentage for the left wheel.
 * @param right Speed percentage for the right wheel.
 */
void robot_set_speed(speed_pct_t left, speed_pct_t right);

/**
 * @brief Gets the position of a specific wheel.
 *
 * @param wheel_id The wheel identifier.
 * @return The position of the specified wheel.
 */
int robot_get_wheel_position(wheel_t wheel_id);

/**
 * @brief Resets the position of the robot's wheels.
 */
void robot_reset_wheel_pos(void);

/**
 * @brief Gets the current status of the robot.
 *
 * @return The status of the robot containing encoder, sensor, and battery information.
 */
robot_status_t robot_get_status(void);

/**
 * @brief Signals an event to external users.
 *
 * @param event The event notification type.
 */
void robot_signal_event(notification_t event);

/**
 * @brief Stops the robot and closes the link to the robot.
 */
void robot_close(void);

#endif // ROBOT_H
