#include "copilot.h"
#include "pilot.h"
#include <stdio.h>
#include <stdbool.h>

// Global variables to manage the path
static move_t *path = NULL;  // Pointer to the current path
static path_status_t path_status = PATH_NOT_STARTED; // Current path execution status
static int current_step = 0; // Current step index in the path
static int path_steps = 0;   // Total number of steps in the path

// Start executing the path
void copilot_start_path(void) {
    if (path == NULL || path_steps == 0) {
        fprintf(stderr, "Erreur : Chemin non défini.\n");
        return;
    }

    current_step = 0;
    path_status = PATH_IN_PROGRESS;

    printf("Démarrage du chemin. Premier déplacement : direction=%d, vitesse=%d\n",
           path[current_step].direction, path[current_step].speed);

    // Start the first move in the path
    pilot_start_move(path[current_step]);
}

// Monitor the progress of the path, stopping at each step completion
path_status_t copilot_stop_at_step_completion(void) {
    if (path_status != PATH_IN_PROGRESS) {
        return path_status;
    }

    move_status_t move_status = pilot_stop_at_target();
    if (move_status == MOVE_DONE) {
        current_step++;

        if (current_step >= path_steps) {
            path_status = PATH_COMPLETED;
            printf("Chemin terminé.\n");
        } else {
            // Move to the next step in the path
            printf("Déplacement terminé. Prochain mouvement : direction=%d, vitesse=%d\n",
                   path[current_step].direction, path[current_step].speed);
            pilot_start_move(path[current_step]);
        }
    }

    return path_status;
}

// Check if the path execution is completed
bool copilot_is_path_completed(void) {
    return path_status == PATH_COMPLETED;
}

// Set the path to be followed
void copilot_set_path(move_t *new_path, int steps) {
    printf("Configuration du chemin\n");

    path = new_path;
    path_steps = steps;
    path_status = PATH_NOT_STARTED;
}
