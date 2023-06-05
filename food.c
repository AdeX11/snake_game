/*food.c -------
*
* Filename: food.c
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:16:12 2020
* Last-Updated: September 12 16:51 2020
*     By: Manisha Suresh Kumar
*
*/

/* Commentary:
*
*
*
*/

/* Change log:
*
*
*/

/*Copyright (c) 2016 The Trustees of Indiana University and
* Indiana University Research and Technology Corporation.
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "food.h"


//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        char randomType = (rand() > RAND_MAX/2) ? 'O' : '+';
        new_food->type = randomType;
    }
    else if(type == Decrease){
        char randomType = (rand() > RAND_MAX/2) ? 'X' : '!';
        new_food->type = randomType;
    }
    new_food->next = NULL;

    return new_food;
}

//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}


enum Type food_type(Food* foods, int x, int y){
    //Implement the code to return the type of the food 
    //present at position (x, y)    
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return temp->type;
        temp = temp->next;
    }
    
}


// Display all the food
void draw_food (Food *foods)
{   Food* temp = foods;
    while(temp) {
        mvprintw(temp->y, temp->x, "%c", temp->type);
        temp = temp->next;
    }
}

Food* changeFoodCoordinates(Food * foods, int xchange, int ychange, int xmin, int ymin, int xmax, int ymax) {
    Food* temp = foods;
    Food* head = temp;
    while(temp) {
        temp->x += xchange;
        temp->y += ychange;
        temp = temp->next;
    }
    
    return head;
}


Food *remove_eaten_food(Food *foods, int x, int y) {

    Food *currentFood;
    Food *previousFood = NULL;

    // check if first is eaten
    if (foods->x == x && foods->y == y) {
        currentFood = foods;
        foods = foods->next;
        free(currentFood);
        return foods;
    }

    currentFood = foods;
    //until last
    while (currentFood->next) {
        // check if the temp is pointing to the food in question
        if (currentFood->x == x && currentFood->y == y) {   // current food gets freed and previous food skips over it to the nect
            previousFood->next = currentFood->next;
            free(currentFood);
        }

        // check for the first valueo of PFood
        if (previousFood != NULL) {
            previousFood = previousFood->next;
        }
        else {
            previousFood = currentFood;
        }
        currentFood = currentFood->next;
    }
    if (currentFood->x == x && currentFood->y == y) {
        free(currentFood);
        previousFood->next = NULL;
    }

    return foods;
}
