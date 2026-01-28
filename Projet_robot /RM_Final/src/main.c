#include <signal.h>   // Gérer les interruptions (ex: Ctrl+C)
#include <stdio.h>    // Fonctions d'affichage et de lecture (printf, scanf)
#include <stdlib.h>   // Fonctions utilitaires (exit, malloc)
#include <unistd.h>   // Fonctions POSIX (sleep, usleep, STDIN_FILENO)
#include <termios.h>  // Contrôle du terminal (désactiver le mode canonique)
#include <fcntl.h>    // Manipulation des fichiers et des entrées/sorties
#include "robot_app/pilot.h"
#include "robot_app/robot.h"
#include "utils.h"
#include "robot_app/copilot.h"
#include "robot_app/app_manager.h"
#include "robot_app/IHM.h"

// Definition of process states (active or stopped)
typedef enum {
    STOPPED = 0, // The process is stopped
    ALIVE        // The process is running
} process_state_t;

// Global variable to track the program state
static process_state_t running = ALIVE;

// Save initial terminal settings
static struct termios saved_tattr;

// SIGINT (Ctrl+C) signal handler to gracefully stop the program
static void sigint_handler(int dummy) {
    running = STOPPED;
}

// Configure the terminal in non-canonical mode for user input
void set_input_mode(void) {
    struct termios tattr;

    // Save current terminal attributes
    tcgetattr(STDIN_FILENO, &saved_tattr);

    // Disable canonical mode and input echo
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);

    // Apply the new terminal configuration
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

// Restore initial terminal settings before exiting
void restore_input_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_tattr);
}

// Read user input and convert to integer
int get_input() {
    char c;
    c = getchar();
    if (c < '0' || c > '9') return -1; // Return an error for invalid input
    return c - '0';// Convert character to integer
}

int is_t_pressed(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Set stdin to non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Try to read a character
    ch = getchar();

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == 't' || ch == 'T') {
        return 1; // 't' key was pressed
    }
    return 0; // No 't' key was pressed
}

// Main application loop managing different states
void app_loop() {
    robot_status_t my_status;
    app_state_t state = STATE_SELECT_PATH;
    int path_choice, speed;
    move_t *selected_path = NULL;
    int selected_steps = 0;

    set_input_mode(); // Configure terminal

    while (running) {
        switch (state) {
            case STATE_SELECT_PATH:
                display_menu(); // Display user menu
               
                path_choice = get_input();
                if (path_choice == 0) { // Exit application
                    running = STOPPED;
                    break;
                }
                else if (path_choice == 1) { // Wall-following mode
                    printf("Mode suivi du mur droit activé.\n");
                    state = STATE_FOLLOW_WALL;
                } else {
                    printf("Choisissez la vitesse (1-10) : ");
                    speed = get_input() * 10;
                    clear_screen();
                    // Retrieve selected path
                    selected_path = get_path(path_choice, &selected_steps, speed);
                    if (selected_path == NULL) {
                        printf("Choix de chemin invalide.\n");
                        continue;
                    }
                    
                    // Start the selected path
                    copilot_set_path(selected_path, selected_steps);
                    copilot_start_path();
                    state = STATE_EXECUTE_PATH;
                }
                break;

            case STATE_EXECUTE_PATH:
                my_status = robot_get_status(); // Get robot status
                display_robot_status(my_status);
                
                // Check if movement is completed
                if (check_path_completion() || pilot_get_status() == MOVE_DONE) {
                    state = STATE_CHECK_COMPLETION;
                }
                break;
            
            case STATE_CHECK_COMPLETION:
                printf("Chemin terminé. Choisissez un autre chemin ou quittez.\n");
                state = STATE_SELECT_PATH;
                break;

            case STATE_FOLLOW_WALL:
                printf("Mode suivi du mur droit activé. Appuyez sur 't' pour arrêter.\n");
                while (running && !is_t_pressed()) {
                    follow_right_wall();
                    usleep(100000); // Pause pour éviter une surcharge du CPU
                }
                printf("Vous passez en mode manuel.\n");
                robot_set_speed(0, 0);  // Stop the robot         /*ajout*/
                state = STATE_SELECT_PATH;
            break;

            default:
                fprintf(stderr, "Erreur inconnue.\n");
                break;
            }
    }
}

// Main function of the program
int main(void) {
    if (robot_start()) { // Initialize robot
        printf("Erreur lors du démarrage du simulateur de robot.\n");
        fflush(stdout);
        return EXIT_FAILURE;
    }
    
    printf("**** Application Robot ****\n");
    printf("Ctrl+C pour quitter\n");
    fflush(stdout);

    // Associate SIGINT signal with sigint_handler
    signal(SIGINT, sigint_handler);

    app_loop(); // Start main loop

    restore_input_mode(); // Restore terminal settings

    robot_close(); // Properly shut down the robot
    return EXIT_SUCCESS;
}
