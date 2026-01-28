// robot.c

#include "robot.h"
#include "mrpiz.h"
#include <errno.h>
#include <stdio.h>

// Initializes the robot
int robot_start(void) {
  int result = 0;

  // Initialize the mrpiz library and check for errors
  if (mrpiz_init() != 0) {
      mrpiz_error_print("Problème d'initialisation"); // Display initialization error
  }

  return result;
}

// Sets the speed of the left and right wheels
void robot_set_speed(speed_pct_t left, speed_pct_t right) {
  mrpiz_motor_set(MRPIZ_MOTOR_LEFT, left); // Set left wheel speed
  mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, right); // Set right wheel speed
}

// Retrieves the encoder position of a given wheel
int robot_get_wheel_position(wheel_t wheel_id) {
   return mrpiz_motor_encoder_get(wheel_id);
}

// Resets the encoder positions for both wheels
void robot_reset_wheel_pos(void) {
  mrpiz_motor_encoder_reset(MRPIZ_MOTOR_BOTH);
}

// Retrieves the current status of the robot
robot_status_t robot_get_status(void) {
    robot_status_t status;

    // Get sensor readings
    status.left_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT);
    status.right_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT);
    status.center_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER);
    
    // Get encoder positions
    status.left_encoder = mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
    status.right_encoder = mrpiz_motor_encoder_get(MRPIZ_MOTOR_RIGHT);
    
    // Get battery level
    status.battery = mrpiz_battery_level();
    
    return status;
}

// Controls the LED signal based on the robot's status
void robot_signal_event(notification_t event) {
  switch (event) {
  case ROBOT_OK:
    mrpiz_led_rgb_set(MRPIZ_LED_OFF); // Turn off LED for normal status
    break;
  case ROBOT_OBSTACLE:
    mrpiz_led_rgb_set(MRPIZ_LED_RED); // Red LED indicates an obstacle detected
    break;
  case ROBOT_PROBLEM:
    mrpiz_led_rgb_set(MRPIZ_LED_GREEN); // Green LED signals a problem
    break;
  case ROBOT_IDLE:
    mrpiz_led_rgb_set(MRPIZ_LED_BLUE); // Blue LED indicates idle mode
    break;
  default:
    mrpiz_led_rgb_set(MRPIZ_LED_OFF); // Default case turns LED off
    break;
  }
}

// Stops the robot and closes the mrpiz library
void robot_close(void) {
  robot_set_speed(0, 0); // Stop the robot
  mrpiz_close(); // Close the mrpiz library
}
