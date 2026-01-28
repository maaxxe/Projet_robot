

#include "IHM.h"
#include <stdio.h>

// Function to clear the console screen
void clear_screen() {
    printf("\033[H\033[J");
}

// Function to display the main menu of the robot application
void display_menu() {
    printf("\n");
    printf("************************************\n");
    printf("*        Application  Robot        *\n");
    printf("************************************\n");
    printf("* 1. avancer (8)                   *\n");  // Move forward
    printf("* 2. droite (6)                    *\n");  // Turn right
    printf("* 3. gauche (4)                    *\n");  // Turn left
    printf("* 4. demie tour (2)                *\n");  // U-turn
    printf("* 5. Path definie (7)              *\n");  // Predefined path 1
    printf("* 6. Path definie (9)              *\n");  // Predefined path 2
    printf("* 7 - Suivi du mur droit(1)        *\n");  // Follow right wall
    printf("* 0. Quitter                       *\n");  // Quit the application
    printf("************************************\n");
    printf("Choisissez une option : ");  // Prompt user to choose an option
}
