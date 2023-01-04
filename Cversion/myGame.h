#ifndef MY_GAME
#define MY_GAME

#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
    key_escape = 27,
    key_enter = '\n',
    delay = 100
};

// Menu item names
enum item_names {
    //start_item = 0,
    //controls_item = 1,
    //exit_item = 2,
    //maxMenuItemsCount = 3,

    again_item = 4,
    to_menu_item = 5,
    maxGameOverItemsCount = 2,

    back_item = 7,

    maxItemNames = 6,
};

typedef enum item_names itemNames_t;

enum mainMenuItems {
    start_item = 0,
    controls_item = 1,
    exit_item = 2,
    maxMainMenuItemsCount = 3
};

typedef enum mainMenuItems mainMenuItems_t;

// General purpose coordinates
struct coordinates {
    int x;
    int y;
};

typedef struct coordinates position_t;

struct selectionPoint
{
    itemNames_t currentItem;
    char selectionSymbol;
};

typedef struct selectionPoint selectionPoint_t;

// Snake's body
struct body {
    int number;
    struct coordinates pos;
    struct coordinates dir;
    struct body *next;
};

// Snake's head
struct head {
    struct coordinates pos;
    struct coordinates dir;
    struct coordinates goal_cords;
    struct body *first;
    struct body *last;
    int length;
    char head_symbol;
};

void screenInit();
void drawMainMenu();
void print_menu(position_t cur, const char *str);
void move_menu(position_t dest, position_t src);
void game_init();
void check_coords(position_t *pos);
void time_delay(char sy);
void snake_grow(struct head *snake);
position_t rand_goal(struct head snake);
int set_goal(struct head *snake);
void mv_snake(struct head *snake);
void game_start();
void blinking_arrow(struct head* snake);
void direction_normalization(struct head* snake, struct body* tmp);
void game_over(struct head* snake);
void game_over_screen();
void game_over_screen_init();
int goal_inside_snake(struct head snake, position_t pos);
int lose_condition(struct head* snake);
void movement(struct head* snake);
void mv(position_t* pos, position_t dpos);
void print_snake(struct head snake);
void rem_last_char(struct head snake);
void update_data();

#endif
