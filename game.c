#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"
#include "food.h"
#include "game_window.h"
#include "key.h"
#include "game.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "obstacle.h"
#include <menu.h>
   
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}
Food* readNextFood(Food * head,FILE *pFile)
{
  size_t returnValue;
  if(head == NULL)
  {
    head = malloc(sizeof(Food));
    returnValue = fread(head,sizeof(Food),1,pFile);
    head->next=NULL;

  }
  else
  {
   Food* indexFood = head;
    Food* newFood = malloc(sizeof(Food));
    while(indexFood->next != NULL)
    {
      indexFood = indexFood->next;

    }
    returnValue = fread(newFood,sizeof(Food),1,pFile);
    indexFood->next = newFood;
    newFood->next = NULL;


  }
  return head;
}

Snake* readNext(Snake * head,FILE *pFile)
{
  size_t returnValue;
  if(head == NULL)
  {
    head = malloc(sizeof(Snake));
    returnValue = fread(head,sizeof(Snake),1,pFile);
    head->next=NULL;

  }
  else
  {
    Snake* indexSnake = head;
    Snake* newSnake = malloc(sizeof(Snake));
    while(indexSnake->next != NULL)
    {
      indexSnake = indexSnake->next;

    }
    returnValue = fread(newSnake,sizeof(Snake),1,pFile);
    indexSnake->next = newSnake;
    newSnake->next = NULL;


  }
  return head;
}

Obs* readNextObstacle(Obs * head,FILE *pFile)
{
  size_t returnValue;
  if(head == NULL)
  {
    head = malloc(sizeof(Obs));
    returnValue = fread(head,sizeof(Obs),1,pFile);
    head->next=NULL;

  }
  else
  {
    Obs* indexObs = head;
    Obs* newObs = malloc(sizeof(Obs));
    while(indexObs->next != NULL)
    {
      indexObs = indexObs->next;

    }
    returnValue = fread(newObs,sizeof(Obs),1,pFile);
    indexObs->next = newObs;
    newObs->next = NULL;
    
    
  }
  return head;
}

void game(){
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    // top of board ^
    gamewindow_t *window; // Name of the board
    static int windowSize = 0;
    Snake *snake,*snakeB,*snake1,*snake2;
    char snake1Dir=UP,snake2Dir = DOWN;
    Food *foods,*new_food; // List of foods (Not an array)
    int choice = 0;
    static int points=0,points2 = 0;
    static int foodEaten = 0;
    int array[6];
    const int height = 35; 
    const int width = 70;
    char ch, currentDirection = RIGHT,direction=LEFT;
    char cantGoInDirection = LEFT;
    char vertEdge = NOCHAR;
    char horiEdge = NOCHAR;
    bool isMulti = false;
    int mode;
    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;
    static int speed = 4;
   // THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //obstacles
    srand(time(NULL));
    int obstacleCt = 5; //between 3 and 7
    int foodCt = 10;
    static int obsSize, obs_x, obs_y;
    enum Dir obsDirection;
    Obs *obstacles,*newObs;
    static int lives = 3;static int livesB = 3;
    time_t previousTime,previousTime2;
    int iterations=0;;
    int opt=0;
        
    while(state != EXIT){
        switch(state){
        case INIT:
            initscr();
            start_color();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // m
            curs_set(0); // hide cursor
            timeout(100);
            char ch = getch();
	    init_pair(1, COLOR_GREEN, COLOR_BLACK); /* create foreground / background combination */
            init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
            init_pair(3, COLOR_YELLOW, COLOR_BLACK);
            init_pair(4, COLOR_RED, COLOR_BLACK);
            

            while(ch != 'q')
            {
              mvprintw(20,80,"Choose an option");
              mvprintw(22,95, "Single Player");
              mvprintw(25,95, "Multiplayer");
              mvprintw(28,95, "Load Game");
            // print underline of choice
              if (opt == 0){
                 attron(COLOR_PAIR(1));
		 mvprintw(23, 95, "______");
		 mvprintw(22,95,"Single Player");
		 attroff(COLOR_PAIR(1));
              }
              else if (opt == 1){
		 attron(COLOR_PAIR(2));
		 mvprintw(26, 95, "____");
		 mvprintw(25,95, "Multiplayer");
		 attroff(COLOR_PAIR(2));
	      }
              else if (opt == 2){
		 attron(COLOR_PAIR(3));
		 mvprintw(29, 95, "____");
		 mvprintw(28,95, "Load Game");
		 attroff(COLOR_PAIR(3));
	      }
              ch = get_char();

            // movement to change choice
              switch (ch)
              {
              case UP:
                  if (opt > 0) opt--;
                  break;
              case DOWN:
                  if(opt < 2) opt++;
                  break;
              case 10: //10 is enter key
                  if (opt == 0){
		    state = ALIVE;ch ='q';break;
                  }
                  else if (opt == 1)
                  {

                   state=ALIVE2;ch = 'q';isMulti = true;
                  // mvprintw(20,80, "What File Name would you like to save the file: ?");
                   break;
                   
                  }// save game state   NOT DONE
                  else if (opt == 2) {
                      state = LOADING;ch = 'q';
                      //mvprintw(20,80,"What is the fileName(Press Enter when finished): ?\n");
                      break;
                  }
              default:
                  break;
	      }	    
              
       	    }
	    clear();refresh();
	    if(state == LOADING)
            {
		break;
            }
            	    
	    char c;
            while(c != 'a'  && c != 'A' && c != 'h' && c != 'H' && c != 'e' && c != 'E')
            {
              mvprintw(20,80,"Please choose a difficulty level");
              mvprintw(30,80,"(E)Easy");mvprintw(40,80,"(H)Hard");
              mvprintw(50,80,"(A)Advanced");
              c=getch();

              if(c == 'e' || c == 'E')
              {
	         timeret.tv_nsec = 999999999/4;obstacleCt = 5;foodCt = 10;
		 
              }

	      else if(c == 'h' || c == 'H')
              {
		  timeret.tv_nsec = 999999999/8;
		  obstacleCt = 10;foodCt = 20;
              }
	      else if(c == 'a' || c == 'A')
              {
                  timeret.tv_nsec = 999999999/16;
		  obstacleCt = 15;foodCt = 30;
              }
	     
            }
            clear();
            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
            
            //Init board
            window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);
            refresh();
            // Init snake
            previousTime = time(NULL);previousTime2 = time(NULL);
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
	    snakeB = init_snake(x_offset + (width / 2)+1, y_offset + (height / 2)+1);
            snake1 = init_enemy_snake(x_offset + (width / 2)+10, y_offset + (height / 2)+10);
	    snake2 = init_enemy_snake(x_offset + (width / 2)+10, y_offset + (height / 2)+10);
            // Init foods
            int food_x, food_y, i;
            enum Type type, eatenType,eatenTypeB,eatenType1,eatenType2;
            srand(time(NULL));
            //Generate 10 foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            for(i = 1; i < foodCt; i++){
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }
           // THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            int j;
            // generate obstacles starting away from edge
            generate_points(&obs_x, &obs_y, width - 7, height - 7, x_offset + 7, y_offset + 7);
            obsDirection = (rand() > RAND_MAX/2) ? vertical : horizontal; //random direction

            // determine random size in window
            if (obsDirection == vertical) obsSize = (rand() % (height - 1)) + 1; //at least 1 or 2 less than the height
            else obsSize = (rand() % (width - 1)) + 1; //at least 1 or 2 less than the width

            obstacles = create_obs(obs_x, obs_y, obsDirection, obsSize);
            newObs = obstacles;

            for(i = 1; i < obstacleCt; i++) {
                
                // generate obstacle
                generate_points(&obs_x, &obs_y, width - 7, height - 7, x_offset + 7, y_offset + 7);
                obsDirection = (rand() > RAND_MAX/2) ? vertical : horizontal; //random direction

                // determine random size in window
                if (obsDirection == vertical) obsSize = (rand() % (height - 1)) + 1; //at least 1 or 2 less than the height
                else obsSize = (rand() % (width - 1)) + 1; //at least 1 or 2 less than the width

                newObs->next = create_obs(obs_x, obs_y, obsDirection, obsSize);
                newObs = newObs->next;
            }

            
            break;

        case ALIVE:
	   // mvprintw(20,50,"FAstet");  
            ch = get_char();
            
            // check for quit or pause
            if (ch == 'Q' || ch == 'q') {
                state = EXIT;;
            }
            if (ch == 'p' || ch == 'P') {
                state = PAUSE;
            }
            if(ch == 'l' || ch == 'L')
	    {
		clear();
		state = LOADING;
	        //mvprintw(50,20,"What is the file name: ?\n");
	        mvprintw(20,80,"What is the fileName(Press Enter when finished): ?\n");
		break;
            }

            //check for window size change
            if (ch == '+' && windowSize < 25) {
                windowSize += 5;
                x_max += 5;
                y_max += 5;
                // increase both sides by 5 to expand
                window = changeGameWindow(x_offset, y_offset, width + windowSize, height + windowSize, window);
                // change coordinates of board elements/food respectively
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                foods = create_food(food_x, food_y, type);
                for(i = 1; i < foodCt; i++) {
                    generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    while (food_exists(foods,food_x, food_y))
                        generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food);
                }
            }
            if (ch == '-' && windowSize > -25) {
                windowSize -= 5;
                x_max -= 5;
                y_max -= 5;
                // decrease both sides by 5 to shrink
                window = changeGameWindow(x_offset, y_offset, width + windowSize, height + windowSize, window);
                // change coordinates of board elements/food respectively
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                foods = create_food(food_x, food_y, type);
                for(i = 1; i < foodCt; i++) {
                    generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    while (food_exists(foods,food_x, food_y))
                        generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food);
                }
            }

            //check for direction change/movement keys
            else if(ch >= LEFT && ch <= DOWN) { 
                

                // change direction if can
                if (ch != cantGoInDirection && ch != horiEdge && ch != vertEdge) {
                    // set forward direction and set as current direction
                    if (ch == LEFT || ch == UP) {
                        //cant go in opposite direction
                        cantGoInDirection = ch + 1;
                    }
                    else {
                        cantGoInDirection = ch - 1;
                    }
                    // snake = move_snake(snake, ch);
                    //change current direction
                    currentDirection = ch;
                }
            }
                // 
            // move snake
            snake = move_snake(snake, currentDirection,window);
            // snake1 = move_snake(snake1,snake1Dir,window);
            //snake2 = move_enemy_snake(snake2,snake2Dir,window);
 
           
	    int new1Dir;
	    int new2Dir;
            double restr = 5  * (double)window->height /100;
            int restrint = (int) restr;
            if(iterations % 5 == 0)
            {
		new1Dir =change_direction(snake1Dir);
		
		
            }
            if(new1Dir == UP ||new1Dir == DOWN ||new1Dir == LEFT ||new1Dir == RIGHT)
            {
		snake1Dir = new1Dir;
            }

            if(window->upper_left_x+window->width==snake1->x) {
 		snake1 = reverseSnake(snake1);
                snake1Dir = LEFT;
            }
            if(window->upper_left_x==snake1->x) {
                snake1 = reverseSnake(snake1);
                snake1Dir = RIGHT;
            }
            if(window->upper_left_y+5>=snake1->y) {
                snake1 = reverseSnake(snake1);
                snake1Dir = DOWN;
            }
            if(window->upper_left_y+window->height-5<=snake1->y) {
                snake1 = reverseSnake(snake1);
                snake1Dir = UP;
            }



            if(iterations % 7 == 0)
	    {
		new2Dir =change_direction(snake2Dir);

               

            }
            if(new2Dir == UP ||new2Dir == DOWN ||new2Dir == LEFT ||new2Dir == RIGHT)
            {
                snake2Dir = new2Dir;
            }
            if(window->upper_left_x+window->width==snake2->x) {
                snake2 = reverseSnake(snake2);
                snake2Dir = LEFT;
            }
            if(window->upper_left_x==snake2->x) {
                snake2 = reverseSnake(snake2);
                snake2Dir = RIGHT;
            }
            if(window->upper_left_y+5>=snake2->y) {
                snake2 = reverseSnake(snake2);
                snake2Dir = DOWN;
            }
            if(window->upper_left_y+window->height-5<=snake2->y) {
                snake2 = reverseSnake(snake2);
                snake2Dir = UP;
            }

            snake1 = move_snake(snake1,snake1Dir,window);
            snake2 = move_snake(snake2,snake2Dir,window);
            iterations++;
	    refresh();
                                        
	    if(eat_itself(snake))
            {
		remove_tail(snake);


            }
           /* if(eat_itself(snake1))
            {
                remove_tail(snake1);


            }
            if(eat_itself(snake2))
            {
                remove_tail(snake2);


            }*/

            
	    // check for obstacles
            if (obs_exists(obstacles, snake->x, snake->y, x_offset + width, y_offset + height)) 
	    {
                // reverse snake
                snake = reverseSnake(snake);

                // go in opposite direction
                if (currentDirection == LEFT || currentDirection == UP) {
                    currentDirection += 1;
                }
                else {
                    currentDirection -= 1;
                }
                snake = move_snake(snake, currentDirection, window);
            }
            if(contact(snake,snake1) || contact(snake1,snake))
            {

                state = DEAD;lives--;continue;
            }
            if(contact(snake,snake2)|| contact(snake2,snake))
            {

                state = DEAD;lives--;continue;
            }


            if (obs_exists(obstacles, snake1->x, snake1->y, x_offset + width, y_offset + height))
            {
                // reverse snake
                snake1 = reverseSnake(snake1);

                // go in opposite direction
                if (snake1Dir == LEFT || snake1Dir == UP) {
                    snake1Dir += 1;
                }
                else {
                    snake1Dir -= 1;
                }
                snake1 = move_snake(snake1, snake1Dir, window);
            }
            if (obs_exists(obstacles, snake2->x, snake2->y, x_offset + width, y_offset + height))
            {
                // reverse snake
                snake2 = reverseSnake(snake2);

                // go in opposite direction
                if (snake2Dir == LEFT || snake2Dir == UP) {
                    snake2Dir += 1;
                }
                else {
                    snake2Dir -= 1;
                }
                snake2 = move_snake(snake2, snake2Dir, window);
            }

            // check for food
            if (food_exists(foods, snake->x, snake->y)) {
                // collect food data
                foodEaten++;
                eatenType = food_type(foods, snake->x, snake->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snake->x, snake->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenType == 'O' || eatenType == '+') {
                    points += 20;
                    grow(snake, currentDirection);
                }
                else {
                    points -= 10;
		    if(snake->next == NULL)
                    {
                      snake = NULL;state=DEAD;lives--;
                    }
		    else  remove_tail(snake);
                }

                //modify speed
                if ((foodEaten % 10) == 0) {
                    speed = speed * 1.5;
                    timeret.tv_nsec = 999999999/speed;
                }
            }
            if (food_exists(foods, snake1->x, snake1->y)) {
                
                eatenType1 = food_type(foods, snake1->x, snake1->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snake1->x, snake1->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenType1 == 'O' || eatenType1 == '+') {
                    
                    grow(snake1, snake1Dir);
                }
                else {
                    
                    if(snake->next == NULL)
                    {
                      snake = NULL;
                    }
                    else  remove_tail(snake1);
                }

            
                
            }
            if (food_exists(foods, snake2->x, snake2->y)) {
                // collect food data
                
                eatenType2 = food_type(foods, snake2->x, snake2->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snake2->x, snake2->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenType2 == 'O' || eatenType2 == '+') {
                    
                    grow(snake2, snake2Dir);
                }
                else {
                    
                    if(snake2->next == NULL)
                    {
                      snake2 = NULL;
                    }
                    else  remove_tail(snake2);
                }

                
            }


            // Draw everything on the screen
            clear();
            mvprintw(10,20, "Your Score: %d", points);
            mvprintw(15,20, "Food eaten: %d", foodEaten);
            mvprintw(20,20, "Key entered: %c", ch);
            mvprintw(30, 20, "Window size: %d", windowSize);
            // mvprintw(15,20, "you ate: %d", eatenType);
            init_pair(1, COLOR_GREEN, COLOR_BLACK); /* create foreground / background combination */
            init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
            init_pair(3, COLOR_YELLOW, COLOR_BLACK);
            init_pair(4, COLOR_RED, COLOR_BLACK);
	    attron(COLOR_PAIR(4));
            draw_Gamewindow(window);attroff(COLOR_PAIR(4));
	    attron(COLOR_PAIR(2));
	    draw_obs(obstacles,x_offset + width, y_offset + height);attroff(COLOR_PAIR(2));
	    attron(COLOR_PAIR(1));

	    draw_snake(snake);attroff(COLOR_PAIR(1));attron(COLOR_PAIR(4));
	    draw_snake(snake1);draw_snake(snake2);attroff(COLOR_PAIR(4));
	    attron(COLOR_PAIR(3));
            draw_food(foods);attroff(COLOR_PAIR(3));
            break;
        case ALIVE2:
            //snakeB = init_snake(x_offset + (width / 2)+10, y_offset + (height / 2)+10);
            ch = get_char();

            // check for quit or pause
            if (ch == 'Q' || ch == 'q') {
                state = EXIT;
            }
            if (ch == 'p' || ch == 'P') {
                state = PAUSE;
            }
            if(ch == 'l' || ch == 'L')
            {
                clear();
                state = LOADING;
                //mvprintw(50,20,"What is the file name: ?\n");
                mvprintw(20,80,"What is the fileName(Press Enter when finished): ?\n");
                break;
            }
            
             //mvprintw(20,50,"FAstet");
            //check for window size change
            if (ch == '+' && windowSize < 25) {
                windowSize += 5;
                x_max += 5;
                y_max += 5;
                // increase both sides by 5 to expand
                window = changeGameWindow(x_offset, y_offset, width + windowSize, height + windowSize, window);
                // change coordinates of board elements/food respectively
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                foods = create_food(food_x, food_y, type);
                for(i = 1; i < foodCt; i++) {
                    generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    while (food_exists(foods,food_x, food_y))
                        generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food);
                }
            }
            if (ch == '-' && windowSize > -25) {
                windowSize -= 5;
                x_max -= 5;
                y_max -= 5;
                // decrease both sides by 5 to shrink
                window = changeGameWindow(x_offset, y_offset, width + windowSize, height + windowSize, window);
                // change coordinates of board elements/food respectively
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                foods = create_food(food_x, food_y, type);
                for(i = 1; i < foodCt; i++) {
                    generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    while (food_exists(foods,food_x, food_y))
                        generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                    type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food);
                }
            }

            //check for direction change/movement keys
            else if(ch >= LEFT && ch <= DOWN) {


                // change direction if can
                if (ch != cantGoInDirection && ch != horiEdge && ch != vertEdge) {
                    // set forward direction and set as current direction
                    if (ch == LEFT || ch == UP) {
                        //cant go in opposite direction
                        cantGoInDirection = ch + 1;
                    }
                    else {
                        cantGoInDirection = ch - 1;
                    }
                    // snake = move_snake(snake, ch);
                    //change current direction
                    currentDirection = ch;
                }
            }
           
            else if(ch == 'W' || ch =='A' || ch == 'S' || ch == 'D')
            {
		if((direction == UP || direction ==DOWN) && ch != 'W' && ch != 'S')
                {
		  if(ch =='A')
                    direction = LEFT;
                  else direction = RIGHT;
                } 
                if((direction == LEFT|| direction ==RIGHT) && ch != 'A' && ch != 'D')
                {
                  if(ch =='W')
                    direction = UP;
                  else direction = DOWN;

                }

            }
	    //printf("AAAAAARARARARARRA");
            // move snake
            snake = move_snake(snake, currentDirection,window);
            snakeB = move_snake(snakeB, direction,window);
            // snake1 = move_snake(snake1,snake1Dir,window);
            //snake2 = move_enemy_snake(snake2,snake2Dir,window);
            if(iterations % 5 == 0)
            {
                snake1Dir =change_direction(snake1Dir);


            }
           

            if(window->upper_left_x+window->width==snake1->x) {
                snake1 = reverseSnake(snake1);
                snake1Dir = LEFT;
            }
            if(window->upper_left_x==snake1->x) {
                snake1 = reverseSnake(snake1);
                snake1Dir = RIGHT;
            }
            if(window->upper_left_y+5>=snake1->y) {
                snake1 = reverseSnake(snake1);
                snake1Dir = DOWN;
            }
            if(window->upper_left_y+window->height-5<=snake1->y) {
                snake1 = reverseSnake(snake1);
                snake1Dir = UP;
            }



            if(iterations % 7 == 0)
            {
                snake2Dir =change_direction(snake2Dir);



            }
           
            if(window->upper_left_x+window->width==snake2->x) {
                snake2 = reverseSnake(snake2);
                snake2Dir = LEFT;
            }
            if(window->upper_left_x==snake2->x) {
                snake2 = reverseSnake(snake2);
                snake2Dir = RIGHT;
            }
            if(window->upper_left_y+5>=snake2->y) {
                snake2 = reverseSnake(snake2);
                snake2Dir = DOWN;
            }
            if(window->upper_left_y+window->height-5<=snake2->y) {
                snake2 = reverseSnake(snake2);
                snake2Dir = UP;
            }

            snake1 = move_snake(snake1,snake1Dir,window);
            snake2 = move_snake(snake2,snake2Dir,window);
            iterations++;
            refresh();
	    if(contact(snake,snake1)|| contact(snake1,snake))
            {

		state = DEAD;lives--;break;
            }
            if(contact(snake,snake2) || contact(snake2,snake))
            {

                state = DEAD;lives--;break;
            }
            if(contact(snakeB,snake1) || contact(snake1,snakeB))
            {

                state = DEAD;livesB--;break;
            }
            if(contact(snakeB,snake2)|| contact(snake,snakeB))
            {

                state = DEAD;livesB--;break;
            }

            if(eat_itself(snake))
            {
                remove_tail(snake);


            }
            if(eat_itself(snakeB))
            {
                remove_tail(snakeB);


            }
           


            // check for obstacles
            if (obs_exists(obstacles, snake->x, snake->y, x_offset + width, y_offset + height))
            {
                // reverse snake
                snake = reverseSnake(snake);

                // go in opposite direction
                if (currentDirection == LEFT || currentDirection == UP) {
                    currentDirection += 1;
                }
                else {
                    currentDirection -= 1;
                }
                snake = move_snake(snake, currentDirection, window);
            }

            if (obs_exists(obstacles, snakeB->x, snakeB->y, x_offset + width, y_offset + height))
            {
                // reverse snake
                snakeB = reverseSnake(snakeB);

                // go in opposite direction
                if (direction == LEFT || direction == UP) {
                    direction += 1;
                }
                else {
                    direction -= 1;
                }
                snakeB = move_snake(snakeB, direction, window);
            }

            if (obs_exists(obstacles, snake1->x, snake1->y, x_offset + width, y_offset + height))
            {
                // reverse snake
                snake1 = reverseSnake(snake1);

                // go in opposite direction
                if (snake1Dir == LEFT || snake1Dir == UP) {
                    snake1Dir += 1;
                }
                else {
                    snake1Dir -= 1;
                }
                snake1 = move_snake(snake1, snake1Dir, window);
            }
            if (obs_exists(obstacles, snake2->x, snake2->y, x_offset + width, y_offset + height))
            {
                // reverse snake
                snake2 = reverseSnake(snake2);

                // go in opposite direction
                if (snake2Dir == LEFT || snake2Dir == UP) {
                    snake2Dir += 1;
                }
                else {
                    snake2Dir -= 1;
                }
                snake2 = move_snake(snake2, snake2Dir, window);
            }

            // check for food
            if (food_exists(foods, snake->x, snake->y)) {
                // collect food data
                foodEaten++;
                eatenType = food_type(foods, snake->x, snake->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snake->x, snake->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenType == 'O' || eatenType == '+') {
                    points += 20;
                    grow(snake, currentDirection);
                }
                else {
                    points -= 10;
                    if(snake->next == NULL)
                    {
                      snake = NULL;state=DEAD;lives--;
                    }
                    else  remove_tail(snake);
                }
	    }
            if (food_exists(foods, snakeB->x, snakeB->y)) {
                // collect food data
                foodEaten++;
                eatenTypeB = food_type(foods, snakeB->x, snakeB->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snakeB->x, snakeB->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenTypeB == 'O' || eatenTypeB == '+') {
                    points2 += 20;
                    grow(snakeB, direction);
                }
                else {
                    points2 -= 10;
                    if(snakeB->next == NULL)
                    {
                      snakeB = NULL;state=DEAD;livesB--;
                    }
                    else  remove_tail(snakeB);
                }

                
            }
            if (food_exists(foods, snake1->x, snake1->y)) {

                eatenType1 = food_type(foods, snake1->x, snake1->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snake1->x, snake1->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenType1 == 'O' || eatenType1 == '+') {

                    grow(snake1, snake1Dir);
                }
                else {

                    if(snake->next == NULL)
                    {
                      snake = NULL;
                    }
                    else  remove_tail(snake1);
                }



            }
            if (food_exists(foods, snake2->x, snake2->y)) {
                // collect food data

                eatenType2 = food_type(foods, snake2->x, snake2->y);

                // remove food and replace
                foods = remove_eaten_food(foods, snake2->x, snake2->y);
                generate_points(&food_x, &food_y, width + windowSize, height + windowSize, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);

                //add to points and modify snake size
                if (eatenType2 == 'O' || eatenType2 == '+') {

                    grow(snake2, snake2Dir);
                }
                else {

                    if(snake2->next == NULL)
                    {
                      snake2 = NULL;
                    }
                    else  remove_tail(snake2);
                }


            }
	    if ((foodEaten % 20) == 0) {
              speed = speed * 1.5;
              timeret.tv_nsec = 999999999/speed;
            }


            // Draw everything on the screen
            clear();
            mvprintw(10,20, "Player1: %d", points);
            mvprintw(12.5,20, "Player2: %d", points2);
            mvprintw(15,20, "Food eaten: %d", foodEaten);
            mvprintw(20,20, "Key entered: %c", ch);
            mvprintw(30, 20, "Window size: %d", windowSize);
            // mvprintw(15,20, "you ate: %d", eatenType);
            init_pair(1, COLOR_GREEN, COLOR_BLACK); /* create foreground / background combination */
            init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
            init_pair(3, COLOR_YELLOW, COLOR_BLACK);
            init_pair(4, COLOR_RED, COLOR_BLACK);
	    init_pair(5, COLOR_BLUE, COLOR_BLACK);
            attron(COLOR_PAIR(4));
            draw_Gamewindow(window);attroff(COLOR_PAIR(4));
            attron(COLOR_PAIR(2));
            draw_obs(obstacles,x_offset + width, y_offset + height);attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(1));

            draw_snake(snake);attroff(COLOR_PAIR(1));attron(COLOR_PAIR(5));
            draw_snake(snakeB);attroff(COLOR_PAIR(5));attron(COLOR_PAIR(4));
            draw_snake(snake1);draw_snake(snake2);attron(COLOR_PAIR(4));
            attron(COLOR_PAIR(3));

            draw_food(foods);attroff(COLOR_PAIR(3));
            break;

	case LOADING:
		mvprintw(20,80,"What is the fileName(Press Enter when finished): ?\n");
		DIR *dir;
        	struct dirent *nextDir;
  	        dir = opendir("saves");
		int x = 30;
		mvprintw(25,80,"list of files in save folder");refresh();
    		if (dir)
    		{
        	  while ((nextDir = readdir(dir)) != NULL)
        	  {
		    if(strcmp(nextDir->d_name,"save_best_5.game") != 0)
		    {
            	      mvprintw(x,80,"%s\n", nextDir->d_name);x+=2;
		    }
        	  }
        	  closedir(dir);
    		}
		refresh();                
                char store[20]  = "saves/";
                FILE * fh;char * string = (char *)malloc(100*sizeof(char));
                scanf("%s",string);strcat(store,string);
		int length = strlen(store);
		const char *last_five = &store[length-5];
		fh = fopen(store,"rb");
                if(fh ==NULL)
                {
                  clear();
                  mvprintw(30,80,"This file doesnt exist,returning to game...");
                  refresh();
                  sleep(3);
                  state = ALIVE;
                  break;


                }
		
                if(strcmp(".game",last_five)!=0)
                {
		  clear();//nodelay(stdscr, TRUE);timeout(100);refresh();
                  mvprintw(30,80,"This file doesnt contain a .game extension, returning to game...");
                  refresh();sleep(2);
                  state = ALIVE;
                  
                  break;


                }
	
		clear();
                if(fh ==NULL)
                {
		  clear();
        	  mvprintw(30,80,"This file doesnt exist");
		  refresh();
                  sleep(1);
                  break;
        
	
		}
                Snake *head,*headB,*head1,*head2;Food * firstFood;Obs *firstObstacle;
               
                fread(&mode,sizeof(int),1,fh);
		fread(&foodCt,sizeof(int),1,fh);
		fread(&obstacleCt,sizeof(int),1,fh);
		int len,lenB,len1,len2;
                fread(&len,sizeof(int),1,fh);fread(&lenB,sizeof(int),1,fh);
		fread(&len1,sizeof(int),1,fh);fread(&len2,sizeof(int),1,fh);
		fread(&timeret.tv_nsec,sizeof(long),1,fh);
                head= NULL;headB=NULL;head1=NULL;head2=NULL;firstFood=NULL;firstObstacle =NULL;
		int loop = 0;
		for(loop = 0; loop < len;++loop)
		{
		 
		  head = readNext(head,fh);

		}
		snake = head;
		
	        if(mode == 1)
		{
                  for(loop = 0; loop < lenB;++loop)
                  {
  
                    headB = readNext(headB,fh);
  
                  }
 	          snakeB = headB;
		}

                
                for(loop = 0; loop < len1;++loop)
                {

                  head1 = readNext(head1,fh);

                }
                snake1 = head1;
                for(loop = 0; loop < len2;++loop)
                {

                  head2 = readNext(head2,fh);

                }
                snake2 = head2;

                gamewindow_t * windowt= malloc(sizeof(gamewindow_t));
                fread(windowt,sizeof(gamewindow_t),1,fh);
		window = windowt;
                for(loop = 0; loop < obstacleCt;++loop)
                {
                 // fseek(fh,(sizeof(Obs) * loop),SEEK_SET);
                  firstObstacle = readNextObstacle(firstObstacle,fh);

                }
                obstacles = firstObstacle;

                for(loop = 0; loop < foodCt;++loop)
                {
                  //fseek(fh,(sizeof(Food) * loop),SEEK_SET);
                  firstFood = readNextFood(firstFood,fh);

                }
                foods = firstFood;
                fread(&points,sizeof(points),1,fh);fread(&points2,sizeof(points2),1,fh);
                fread(&lives,sizeof(int),1,fh);fread(&livesB,sizeof(int),1,fh);
                fread(&foodEaten,sizeof(int),1,fh);


                fclose(fh);
                state = ALIVE;
                if(mode == 1)
		{
                  state = ALIVE2;
                } 
                break;

        case PAUSE:
            clear();
            
            mvprintw(22,95, "Resume");
            mvprintw(25,95, "Save");
            mvprintw(28,95, "Quit");
            // print underline of choice
            if (choice == 0) mvprintw(23, 95, "______");
            else if (choice == 1) mvprintw(26, 95, "____");
            else if (choice == 2) mvprintw(29, 95, "____");
            ch = get_char();

            // movement to change choice
            switch (ch)
            {
            case UP:
                if (choice > 0) choice--;
                break;
            case DOWN:
                if(choice < 2) choice++;
                break;
            case 10: //10 is enter key
                if (choice == 0) state = ALIVE;
                else if (choice == 1) 
                {
                
                 state=SAVING;
                 mvprintw(20,80, "What File Name would you like to save the file: ?");
		 break;
		}// save game state   NOT DONE
                else if (choice == 2) {
                    state = EXIT;
                    lives--;
		    break;
                }
            default:
                break;
            }
            break;
	case SAVING:
	    clear();
	    char file[20]="saves/";
            char*str = (char *)malloc(100*sizeof(char));
            scanf("%s",str);strcat(file,str);
            int length2 = strlen(file);
            const char *last_five2 = &file[length2-5];
	    if(strcmp(".game",last_five2) != 0)
            {
		mvprintw(30,80,"Files without a .game extension can't be loaded in,saving anyways...");
                refresh();sleep(2);
            }
            FILE *fp;
            fp = fopen(file,"wb");
	    Snake * temp=snake;Snake *tempB = snakeB;Snake *temp4=snake1;Snake *temp5=snake2;Food * temp2 = foods;Obs * temp3 = obstacles;
            mode = 0;
  	    if(fp != NULL)
            {
	      if(isMulti)
	      {
                mode = 1;
              }
	      fwrite(&mode,sizeof(int),1,fp);  
  	      fwrite(&foodCt,sizeof(int),1,fp);
              fwrite(&obstacleCt,sizeof(int),1,fp);
              int len = len_snake(snake);
              int lenB = len_snake(snakeB);
              int len1 = len_snake(snake1);
              int len2 = len_snake(snake2);
             
             
              fwrite(&len,sizeof(int),1,fp);fwrite(&lenB,sizeof(int),1,fp);
	      fwrite(&len1,sizeof(int),1,fp);fwrite(&len2,sizeof(int),1,fp);
              fwrite(&timeret.tv_nsec,sizeof(long),1,fp);

	      Snake * next=NULL;
	      while(temp != NULL)
	      {
	 	next = temp->next;
                temp->next = NULL;
		fseek(fp,0,SEEK_END);
		fwrite(temp,sizeof(Snake),1,fp);
		temp->next = next;
		next = NULL;
		temp = temp->next;	
	      }
              
              if(isMulti)
              {
                Snake * nextB=NULL;
                while(tempB != NULL)
                {
                  nextB = tempB->next;
                  tempB->next = NULL;
                  fseek(fp,0,SEEK_END);
                  fwrite(tempB,sizeof(Snake),1,fp);
                  tempB->next = nextB;
                  nextB = NULL;
                  tempB = tempB->next;
                }
	      }


              
              
              Snake * next4=NULL;
              while(temp4 != NULL)
              {
                next4 = temp4->next;
                temp4->next = NULL;
                fseek(fp,0,SEEK_END);
                fwrite(temp4,sizeof(Snake),1,fp);
                temp4->next = next4;
                next4 = NULL;
                temp4 = temp4->next;
              }
              

              Snake * next5=NULL;
              while(temp5 != NULL)
              {
                next5 = temp5->next;
                temp5->next = NULL;
                fseek(fp,0,SEEK_END);
                fwrite(temp5,sizeof(Snake),1,fp);
                temp5->next = next5;
                next5 = NULL;
                temp5 = temp5->next;
              }
             
	      fwrite(window,sizeof(gamewindow_t),1,fp);
              Obs * next3=NULL;
              while(temp3 != NULL)
              {
                next3 = temp3->next;
                temp3->next = NULL;
                fseek(fp,0,SEEK_END);
                fwrite(temp3,sizeof(Obs),1,fp);
                temp3->next = next3;
                next3 = NULL;
                temp3 = temp3->next;
              }
              Food * next2=NULL;
              while(temp2 != NULL)
              {
                next2 = temp2->next;
                temp2->next = NULL;
                fseek(fp,0,SEEK_END);
                fwrite(temp2,sizeof(Food),1,fp);
                temp2->next = next2;
                next2 = NULL;
                temp2 = temp2->next;
              }
	      fwrite(&points,sizeof(points),1,fp);fwrite(&points2,sizeof(points2),1,fp);
              fwrite(&lives,sizeof(lives),1,fp);fread(&livesB,sizeof(livesB),1,fp);
	      fwrite(&foodEaten,sizeof(foodEaten),1,fp);
 	      fclose(fp);fp = NULL;
            }
	    else
            {
	      printf("FILE DOESNT EXIST!\n");
            }
	    mvprintw(28,95, "Saved");
            state = PAUSE;
            break;	  
        case DEAD:
            clear();
            mvprintw(20,95, "GAME OVER");
            if(isMulti)
            {
              mvprintw(25,95, "Player 1: %d", points);
              mvprintw(27,95, "Player 2: %d", points2);
              mvprintw(29,95, "Player 1 Lives left: %d", lives);
              mvprintw(31,95, "Player 2 Lives left: %d", livesB);
 

            }
            else
            {
              mvprintw(25,95, "Your Score: %d", points);
              mvprintw(27,95, "Lives left: %d", lives);
	    }
            //check lives
            if (lives > 0&& livesB >0) 
	    {
		mvprintw(30,95, "'P' to play again, 'Q' to quit");
                while(ch!= 'q' && ch != 'p')
		{
		  ch = getch();
	
		}
                if (ch == 'Q' || 'q') {
                    state = EXIT;
                }
                if (ch == 'p' || ch == 'P') {
		    clear();
                    state = ALIVE;
                    if(snake->next == NULL)
                      state = INIT;
                }
                
            }
            else {
		while(ch!= 'q' && ch != 'p')
                {
                  ch = getch();

                }

                
                if (ch == 'Q'|| ch == 'q' ) {
                    state = EXIT;
                }
                if(lives == 0)
                {
		   mvprintw(33,95, "Player 2 Wins!");
                }
                else  mvprintw(35,95,"Player 1 Wins!");
                mvprintw(35,95, "Press 'Q' to quit");
            }
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
    //end
    undraw_Gamewindow(window);
    endwin();
    //Mix_FreeMusic(backgroundSound);
	
	// quit SDL_mixer
    //Mix_CloseAudio();
    FILE *fp;
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    
    int i=0;
    if(true)
    {
    
      fp = fopen("./saves/save_best_5.game", "r");
      if((read = getline(&line, &len, fp)) != -1)
      {
      
        char* token = strtok(line, " ");
        while (token != NULL) 
        {
          array[i++] = atoi(token);
          token = strtok(NULL, " ");
        }
        array[i++] = points;
        int j; int k,tmp;
        for(j=0; j<i; ++j)
        {
          for(k=j+1; k<i; ++k)
          {
              if(array[j] <array[k])
              {
                tmp = array[j];
                array[j] = array[k];
                array[k] = tmp;
              }
          }
            
        }
      }     
      int l;
      fclose(fp);
      fp = fopen("./saves/save_best_5.game", "w");
      if(fp != NULL)
      {
        for(l = 0; l <i&& l<5; l++)
        {
          fprintf(fp,"%d ", array[l]);
        }
        fclose(fp);
      }
                     

    } 
   
    

}
