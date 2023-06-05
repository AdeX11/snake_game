/* snake.c -------
 *
 * Filename: snake.c
 * Description:
 * Author: Rishabh Agrawal
 * Maintainer:
 * Created: Sun Sep 13 9:12:30 2020
 * Last-Updated: September 13 22:40 2020
 *           By: Rishabh Agrawal
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

/* Copyright (c) 2016 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "snake.h"
#include "key.h"
#include <time.h>
#include "game_window.h"
#include <ncurses.h>
typedef struct gamewindow_t gamewindow_t;

// Initialize snake
Snake* init_snake(int x, int y){
  Snake* head = create_tail(x, y);
  Snake* tail1 = create_tail(x-1, y);
  Snake* tail2 = create_tail(x-2, y);
  tail1->next = tail2;
  head->next = tail1;
  head->symbol = '@';
  return head;
}

Snake* init_enemy_snake(int x, int y){
  Snake* head = create_tail(x, y);
  Snake* tail1 = create_tail(x-1, y);
  Snake* tail2 = create_tail(x-2, y);
  Snake* tail3 = create_tail(x-3, y);
  Snake* tail4 = create_tail(x-4, y);
  tail3->next = tail4;
  tail2->next = tail3;
  tail1->next = tail2;
  head->next = tail1;
  head->symbol = '@';
  return head;
}

// Creates one tail
Snake* create_tail(int x, int y){
  Snake* snake = malloc(sizeof(snake));
  snake->color[0] = 0;
  snake->color[1] = 0;
  snake->color[2] = 255;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

int change_direction(int direction)
{
  int newDirection;
  srand(time(NULL));
  while(direction == newDirection)
  {
    int random = rand() % 4;
    if(random == 0)
    {
      newDirection = UP;
    }
    if(random == 1)
    {
      newDirection = DOWN;
    }
    if(random == 2)
    {
      newDirection = LEFT;
    }
    if(random == 3)
    {
      newDirection = RIGHT;
    }

  }
  return newDirection;



}
Snake* move_enemy_snake(Snake* snake,int direction, struct gamewindow_t *window){
  Snake* new_head = malloc(sizeof(snake));
  double comp = 5 ;
  new_head->x = snake->x;
  new_head->y = snake->y;
  if((window->upper_left_y+comp) >=snake->y)
  {
    direction = DOWN;
  }
  if((window->upper_left_y+window->height-(5*window->height/100)) <=snake->y)
  {
    direction =UP;
  }

  if(window->upper_left_x==snake->x) {
    snake = reverseSnake(snake);
    direction = RIGHT;move_enemy_snake(snake,direction,window);
    
  }
  if(window->upper_left_x+window->width == snake->x) {
    new_head= reverseSnake(snake);
    direction=LEFT;
  }
  switch(direction) {
    case UP:
        //Write code to make the new head go up by 1 cell
        new_head->y--;

      break;
    case DOWN:
        //Write code to make the new head go down by 1 cell
        new_head->y++;
      break;
    case RIGHT:
        //Write code to make the new head go right by 1 cell
        new_head->x++;
      break;
    case LEFT:
        //Write code to make the new head go left by 1 cell
        new_head->x--;
      break;
  }

  //Set new head as the new head of the entire snake
  new_head->next = snake;
  //Add all the features (color and symbol) to the new cell
  new_head->speed = snake->speed;
  int i;
  for (i=0; i<3; i++) { //color coordinates
    new_head->color[i] = snake->color[i];
  }
  new_head->symbol = snake->symbol;
  //make previous head the body symbol
  snake->symbol = '#';
  // Delete the tail from the snake: HINT - there is a remove tail function below
  remove_tail(new_head);

  return new_head;
}


// Moves the snake in the input direction
Snake* move_snake(Snake* snake, int direction, struct gamewindow_t *window){
  Snake* new_head = malloc(sizeof(snake));

  // Set the new head to have the x and y coordinates as the existing head of the snake
  new_head->x = snake->x;
  new_head->y = snake->y;
  if(window->upper_left_y==snake->y) {
    snake->y = window->upper_left_y + window->height-1;
    return snake;
  }
  else if(window->upper_left_y+window->height ==snake->y) {
    snake->y = window->upper_left_y+1;return snake;
  }
  else if(window->upper_left_x==snake->x) {
    snake->x = window->upper_left_x+window->width-1;return snake;
  }
  else if(window->upper_left_x+window->width == snake->x) {
    snake->x = window->upper_left_x+1;return snake;
  }
  switch(direction) {
    case UP:
        //Write code to make the new head go up by 1 cell
        new_head->y--;

      break;
    case DOWN:
        //Write code to make the new head go down by 1 cell
        new_head->y++;
      break;
    case RIGHT:
        //Write code to make the new head go right by 1 cell
        new_head->x++;
      break;
    case LEFT:
        //Write code to make the new head go left by 1 cell
        new_head->x--;
      break;
  }

  //Set new head as the new head of the entire snake
  new_head->next = snake;
  //Add all the features (color and symbol) to the new cell
  new_head->speed = snake->speed;
  int i;
  for (i=0; i<3; i++) { //color coordinates
    new_head->color[i] = snake->color[i];
  }
  new_head->symbol = snake->symbol;
  //make previous head the body symbol
  snake->symbol = '#';
  // Delete the tail from the snake: HINT - there is a remove tail function below
  remove_tail(new_head);

  return new_head;
}

Snake* remove_tail(Snake* snake){
  if(snake != NULL)
  {
    Snake* end = snake;
      while(end->next->next)
          end = end->next;
      free(end->next);
      end->next = NULL;
      return snake;
  }
}

Snake* grow(Snake* snake, char direction) {
  Snake* secondToLast = snake;
  int x;
  int y;

  // check if snake is only a head
  if (secondToLast->next == NULL) {
    if(direction == LEFT) {
      x = secondToLast->x + 1;
      y = secondToLast->y;
    }
    if(direction == RIGHT) {
      x = secondToLast->x - 1;
      y = secondToLast->y;
    }
    if(direction == UP) {
      x = secondToLast->x;
      y = secondToLast->y - 1;
    }
    if(direction == DOWN) {
      x = secondToLast->x;
      y = secondToLast->y + 1;
    }
    secondToLast->next = create_tail(x,y);
    return snake;

  }

  //snake has tail
  while(secondToLast->next->next)
      secondToLast = secondToLast->next;

  // if vertical end
  if (secondToLast->next->x == secondToLast->x) {
    x = secondToLast->next->x;
    // horizontal direction of end
    if (secondToLast->next->y < secondToLast->y) y = secondToLast->next->y - 1;
    else y = secondToLast->next->y + 1;
  }
  else { // horizontal end
    y = secondToLast->next->y;
    // vertical direction of end
    if (secondToLast->next->x < secondToLast->x) x = secondToLast->next->x - 1;
    else x = secondToLast->next->x + 1;
  }
  secondToLast->next->next = create_tail(x,y);
  return snake;
}

// reverse snake
Snake* reverseSnake(Snake* snake) {
  Snake* previous = NULL;
  Snake* current = snake;
  Snake* next = NULL;

  //head is now tail
  current->symbol = '#';

  //until previous points to last
  while (current) {
    //store next, reverse current to point at previous
    next = current->next;
    current->next = previous;
    //move up/back
    previous = current;
    current = next;
  }
  // previous is the head
  previous->symbol = '@';
  
  return previous;
}

// draws the snake on the board
void draw_snake(Snake* snake){
  while(snake){
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    snake = snake->next;
  }
}

// checks if it eats itself, if it does, then return true
bool eat_itself(Snake* snake){
    int x = snake->x;
    int y = snake->y;
    Snake * temp = malloc(sizeof(Snake));
    temp = snake->next;
    while(temp)
    {
      if(temp->x == x && temp->y == y)
      {
        return true;
      }
    temp = temp->next;
    }
    return false;
}

bool contact(Snake * snake1, Snake* snake2)
{
  Snake * temp1 = snake2;
  while(temp1)
  {
    if(snake1->x == temp1->x && snake1->y == temp1->y)
    {

      return true;
    }
    temp1 =temp1->next;
  }

 

  
  return false;

}
int len_snake(Snake * snake)
{
  Snake * temp = snake;
  int i=0;
  while(temp)
  {
    i++;
    temp = temp->next;
  }
  return i;
}

