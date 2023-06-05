/* snake.h -------
 *
 * Filename: snake.h
 * Description:
 * Author: Rishabh Agrawal
 * Maintainer:
 * Created: Sun Sep 13 21:01:02 2020
 * Last-Updated: September 13 22:43 2020
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


#include <stdbool.h>
typedef struct gamewindow_t gamewindow_t;
struct Snake {
  int x;
  int y;
  int speed;
  char color[3];
  char symbol;
  struct Snake* next;
};

typedef struct Snake Snake;

Snake* init_snake(int x, int y);
Snake* init_enemy_snake(int x, int y);
int change_direction(int direction);
Snake* create_tail(int x, int y);
Snake* move_snake(Snake* snake, int direction, gamewindow_t *window);
void draw_snake(Snake* snake);
bool contact(Snake * snake1, Snake* snake2);
bool eat_itself(Snake* snake);
Snake* remove_tail(Snake* snake);
Snake* grow(Snake* snake, char direction);
int len(Snake* snake);
int len_snake(Snake * snake);
Snake* reverseSnake(Snake* snake);
