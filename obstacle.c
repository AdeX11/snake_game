#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "obstacle.h"

//Create new obstacle
Obs* create_obs(int x, int y, char direction, int size) {
    Obs* new_obs = malloc(sizeof(Obs));
    new_obs->x = x;
    new_obs->y = y;
    new_obs->direction = direction;
    new_obs->size = size;
    new_obs->next = NULL;

    return new_obs;
}

//Check if obs exists at coordinates
bool obs_exists(Obs* obstacles, int x, int y, int xmax, int ymax) {
    Obs* current = obstacles;
    int i = 0, Y, X, j;
    while(current) {
        X = current->x;
        Y = current->y;
        if(current->direction == vertical) {
            // for obs corrdinates up to size and/or board y-range
            for(j = 0; j < current->size && Y < ymax; j++) {
                if (X == x && Y == y) return true;
                Y++;
            }
        }
        else if(current->direction == horizontal) {
            // for obs corrdinates up to size and/or board x-range
            for(j = 0; j < current->size && X < xmax; j++) {
                if (X == x && Y == y) return true;
                X++;
            }
        }
        else printf("Direction is invalid\n");
        current = current->next;
    }
    return false;
}

// Display all the obs
void draw_obs (Obs* obstacles, int xmax, int ymax)
{   
    Obs* current = obstacles;
    int i = 0, Y, X, j;
    while(current) {
        X = current->x;
        Y = current->y;
        if(current->direction == vertical) {
            // for obs corrdinates up to size and/or board y-range
            for(j = 0; j < current->size && Y < ymax; j++) {
                mvprintw(Y, X, "H");
                Y++;
            }
        }
        else if(current->direction == horizontal) {
            // for obs corrdinates up to size and/or board x-range
            for(j = 0; j < current->size && X < xmax; j++) {
                mvprintw(Y, X, "H");
                X++;
            }
        }
        else printf("Direction is invalid\n");
        current = current->next;
    }
}
