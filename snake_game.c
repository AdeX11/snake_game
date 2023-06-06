#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <sys/stat.h>
int main(void){
    printf("Welcome to snake game!\n" 
    "* Use the arrows on your keyboard to move left, right, up, and down, respectively.\n"
    "* The snake can only move forwards, it cannot backtrack.\n"
    "* Eating good food 'O' and '+' will make the snake grow, eating junk food 'X' and '!' will make the snake shrink.\n"
    "* Avoid Red enemy snakes or you will lose a life.\n"
    "* You can bounce off the obstacles 'H'\n"
    "* Every good food eaten is worth 20 points, you lose 10 points for every junk food eaten.\n"
    "* While playing you can change the window size using '+' and '-'\n"
    "* You can press 'P' to pause the game.\n"
    "* You can save your current game in the pause menu and load it up when you restart the game.\n"
    "* You can also just press 'L' during game play to load up a previously saved game.\n"
    "* When saving the game, ensure to use a .game extension. For example, 'play1.game'.\n"
    "* For Multiplayer mode(2 players), Player 1 can use the 'WASD' keys and Player 2 can use the arrow keys.\n"
    "* You can press 'Q' at any time to quit the game, enjoy :)\n");
    printf("Enter 's' to start:\n");
    char option;
    bool check =false;
    int i = 0;
    for (i; i<1; i++) {
        scanf("%c",&option);
        //option = getchar() - '0'; //to int
        getchar();mkdir("saves",0777);
	FILE *fp = fopen("./saves/save_best_5.game", "r");
        if(fp == NULL)
        {
          check = true;fp = fopen("./saves/save_best_5.game", "w");
        }
	switch(option) {
            case 's':
		//mkdir("saves",0777);
   		if(check)
                {
       		  fprintf(fp,"%d %d %d %d %d\n",0,0,0,0,0);fclose(fp);		
         	}
	 	game();printf("started");
           
                break;
            case 'S':
		//mkdir("saves",0777);
                
                if(check)
                {
                  fprintf(fp,"%d %d %d %d %d\n",0,0,0,0,0);fclose(fp);
                }

                game();printf("started");
                // option = 'Q';
                break;
            case 'Q': //quit
                break;
            case 'q': //quit
                break;
            default: printf("That was not a valid option, try again.\n");
                i--;
                break;
        }
    }
    printf("\nLeft Game.\n");
    return(0);
}
