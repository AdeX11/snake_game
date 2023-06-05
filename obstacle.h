#include <ncurses.h>

//direction of obstical
enum Dir {vertical = 'v', horizontal = 'h'}; 

//Structure to hold properties of obstacle
struct Obs {
  int x;
  int y;
  char direction;
  int size;
  struct Obs* next;
};

typedef struct Obs Obs; 

//Function prototypes 
void add_new_obs(Obs* obstacles, Obs* new_obs);
Obs* remove_eaten_obs(Obs* obstacles, int x, int y);
Obs* changeobsCoordinates(Obs* obstacles, int xchange, int ychange, int xmin, int ymin, int xmax, int ymax);

Obs* create_obs(int x, int y, char direction, int size);
void draw_obs (Obs* obstacles, int xmax, int ymax);
bool obs_exists(Obs* obstacles, int x, int y, int xmax, int ymax);
