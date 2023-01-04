#include "myGame.h"

const char* gameTitle = "Snake!";
const char* developedByMsg = "developed by German";
const char* gameOverText = "Game over";
const char* tryAgainText = "try again";
const char* returnToMainMenuText = "main menu";
const char* scoreText = "score: ";
const char* timeText = "time: ";
const char* startButtonText = "start";
const char* controlsButtonText = "how to play";
const char* exitButtonText = "exit";
const char* returnBackText = "back";

const char* controls_title = "Controls";
const char* controls_uparrow = "Up arrow - moving up";
const char* controls_downarrow = "Down arrow - moving down";
const char* controls_leftarrow = "Left arrow - moving left";
const char* controls_rightarrow = "Right arrow - moving right";
const char* controls_boost = "press and hold move key to boost up your snake";
const char* controls_esc = "pressing ESC returns you to the main menu";

const char menu_symb = '>';
const char goal_symb = 'x';
const char body_symbol = 'x';
const char left_arrow = '<';
const char right_arrow = '>';
const char up_arrow = '^';
const char down_arrow = 'v';

int key, score, game_time, abs_time, process = 0;

// User Interface items' coordinates on screen
position_t start_coords;
position_t controls_coords;
position_t exit_coords;
position_t again_coords;
position_t to_menu_coords;
position_t back_cords;
position_t score_coords;
position_t time_coords;

WINDOW *score_win, *game_win;

// Useful for transition of important variables from "enum item_names" to "position_t" type
position_t menuEnumToPositionTable[maxItemNames];


void screenInit()
{                                         //PDCURSES routine
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	resize_term(18, 50);
}

void drawMainMenu()                                          //init of game's menu
{
	position_t max_coords, cur_coords;

	erase();
	getmaxyx(stdscr, max_coords.y, max_coords.x);
	cur_coords.x = max_coords.x/2;
	cur_coords.y = max_coords.y * 1/6;
	print_menu(cur_coords, gameTitle);

	cur_coords.y += 2;
	print_menu(cur_coords, developedByMsg);

	cur_coords.y = max_coords.y/2 - 1;
	start_coords.x = cur_coords.x - strlen(startButtonText)/2;
	start_coords.y = cur_coords.y;
	print_menu(cur_coords, startButtonText);

	cur_coords.y += 1;
	controls_coords.x = cur_coords.x - strlen(controlsButtonText)/2;
	controls_coords.y = cur_coords.y;
	print_menu(cur_coords, controlsButtonText);

	cur_coords.y += 1;
	exit_coords.x = cur_coords.x - strlen(exitButtonText) / 2;
	exit_coords.y = cur_coords.y;
	print_menu(cur_coords, exitButtonText);

	menuEnumToPositionTable[start_item] = start_coords;
	menuEnumToPositionTable[controls_item] = controls_coords;
	menuEnumToPositionTable[exit_item] = exit_coords;

	move(start_coords.y, start_coords.x - 2);
	addch(menu_symb);
}

void game_over_screen_init()
{
	position_t max_coords, cur_coords;

	erase();
	getmaxyx(stdscr, max_coords.y, max_coords.x);
	cur_coords.x = max_coords.x / 2;
	cur_coords.y = max_coords.y * 1 / 6;
	print_menu(cur_coords, gameOverText);

	cur_coords.y = max_coords.y / 2 - 1;
	again_coords.x = cur_coords.x - strlen(tryAgainText) / 2;
	again_coords.y = cur_coords.y;
	print_menu(cur_coords, tryAgainText);

	cur_coords.y += 1;
	to_menu_coords.x = cur_coords.x - strlen(returnToMainMenuText) / 2;
	to_menu_coords.y = cur_coords.y;
	print_menu(cur_coords, returnToMainMenuText);
	move(again_coords.y, again_coords.x - 2);
	addch(menu_symb);
}

void print_menu(position_t cur, const char *str)
{
	int temp;
	temp = strlen(str)/2;
	cur.x = cur.x - temp;
	mvaddstr(cur.y, cur.x, str);
}

void move_menu(position_t dest, position_t src)                //from where to from
{                                               //the menu_symb should be moved
	int spacingFromMenuText = 2;

	move(src.y, src.x - spacingFromMenuText);
	addch(' ');
	move(dest.y, dest.x - spacingFromMenuText);
	addch(menu_symb);
}

void mv_menu(mainMenuItems_t currentItem, const selectionPoint_t* selectionPoint)
{
	int spacingFromMenuText = 2;
	position_t previousScreenCoords = menuEnumToPositionTable[selectionPoint->currentItem];
	position_t newScreenCoords = menuEnumToPositionTable[currentItem];

	move(previousScreenCoords.y, previousScreenCoords.x - spacingFromMenuText);
	addch(' ');
	move(newScreenCoords.y, newScreenCoords.x - spacingFromMenuText);
	addch(selectionPoint->selectionSymbol);
}

void game_init()
{
	position_t max_coords;

	erase();
	getmaxyx(stdscr, max_coords.y, max_coords.x);
	game_win = subwin(stdscr, (max_coords.y - 3), max_coords.x, 0, 0);
	score_win = subwin(stdscr, 3, max_coords.x, (max_coords.y - 3), 0);
	wborder(game_win, 0, 0, 0, 0, 0, 0, 0, 0);
	mvwaddstr(score_win, 1, 2, scoreText);
	mvwaddstr(score_win, 1, (max_coords.x - 7 - strlen(timeText)), timeText);
	score_coords.x = 2 + strlen(scoreText);
	score_coords.y = 1;
	time_coords.x = max_coords.x - 7;
	time_coords.y = 1;
	abs_time = time(NULL);
	game_time = 0;
	score = 0;
}

 void check_coords(position_t *pos)
 {
	 int x, y;
	 getmaxyx(game_win, y, x);

	 if(pos->x == x - 1) {
		 pos->x = 1;
	 } else if(pos->x < 1) {
		 pos->x = x - 2;
	 }

	 if(pos->y == y - 1) {
		 pos->y = 1;
	 } else if(pos->y < 1) {
		 pos->y = y - 2;
	 }
 }

void time_delay(char symbol)
{
	int del;
	if(symbol == up_arrow || symbol == down_arrow) {
		del = delay + 50;
	} else {
		del = delay;
	}
	timeout(del);
}

 struct body* create_body(struct head *snake)
 {
	 struct body *tmp;

	 tmp = malloc(sizeof(struct body));
	 tmp->number = 1;
	 tmp->pos = snake->pos;
	 tmp->dir = snake->dir;
	 tmp->next = snake->first;
	 return tmp;
 }

 void snake_grow(struct head *snake)
 {
	 struct body* tmp;

	 snake->first = create_body(snake);
	 if (snake->length == 0) {
		 snake->last = snake->first;
	 }
	 else {
		 tmp = snake->first;
		 while (tmp) {
			 tmp->number++;
			 tmp = tmp->next;
		 }
	 }
	 snake->length++;
 }

 int goal_inside_snake(struct head snake, position_t pos)
 {
	 int is_it;
	 struct body* tmp;

	 is_it = (pos.x == snake.pos.x) && (pos.y == snake.pos.y);
	 tmp = snake.first;
	 while (tmp) {
		 is_it = is_it || ((pos.x == tmp->pos.x) && (pos.y == tmp->pos.y));
		 tmp = tmp->next;
	 }

	 return is_it;
 }

 position_t rand_goal(struct head snake)
 {
	 position_t max, cords;

	 srand(time(NULL));
	 rand();
	 getmaxyx(game_win, max.y, max.x);
	 do {
		 cords.x = 1 + (int)((max.x - 2) * rand() / (RAND_MAX + 1));
		 cords.y = 1 + (int)((max.y - 2) * rand() / (RAND_MAX + 1));
	 } while (goal_inside_snake(snake, cords));
	 
	 mvwaddch(game_win, cords.y, cords.x, goal_symb);
	 wrefresh(game_win);

	 return cords;
 }

 int set_goal(struct head *snake)
 {
	 int is_goal;

	 is_goal = ((snake->pos.x == snake->goal_cords.x) && (snake->pos.y == snake->goal_cords.y));
	 if(is_goal) {
		 snake->goal_cords = rand_goal(*snake);
	 }
	 return is_goal;
 }

void rem_last_char(struct head snake)
{
	if(snake.length == 0) {
		mvwaddch(game_win, snake.pos.y, snake.pos.x, ' ');
		return;
	}
	mvwaddch(game_win, snake.last->pos.y, snake.last->pos.x, ' ');
}

void mv(position_t *pos, position_t dpos)
{
	pos->x += dpos.x;
	pos->y += dpos.y;
	check_coords(pos);
}

void mv_snake(struct head *snake)       //moves current abstract state
{                                       //changes abstract state
	snake->pos.x += snake->dir.x;
	snake->pos.y += snake->dir.y;
	check_coords(&(snake->pos));
	if(snake->length == 0) {
		return;
	}
	struct body *tmp;
	tmp = snake->first;
	while(tmp) {
		tmp->pos.x += tmp->dir.x;
		tmp->pos.y += tmp->dir.y;
		check_coords(&(tmp->pos));
		tmp = tmp->next;
	}
}

void direction_normalization(struct head* snake, struct body *tmp)
{
	if (tmp == NULL) {
		return;
	}
	if (snake->length == 1) {
		tmp->dir = snake->dir;
		return;
	}
	if (tmp->next == NULL) {
		return;
	}
	direction_normalization(snake, tmp->next);
	tmp->next->dir = tmp->dir;
	if (tmp == snake->first) {
		tmp->dir = snake->dir;
	}
}

void print_snake(struct head snake)         //prints current abstract state
{                                           //changes visual state
	mvwaddch(game_win, snake.pos.y, snake.pos.x, snake.head_symbol);
	struct body *tmp;
	tmp = snake.first;
	while(tmp) {
		mvwaddch(game_win, tmp->pos.y, tmp->pos.x, body_symbol);
		tmp = tmp->next;
	}
	wrefresh(game_win);
	time_delay(snake.head_symbol);
}

int lose_condition(struct head* snake)
{
	int is_lose = 0;
	struct body* tmp;

	tmp = snake->first;

	while (tmp) {
		is_lose = is_lose || ((snake->pos.x == tmp->pos.x) && (snake->pos.y == tmp->pos.y));
		tmp = tmp->next;
	}


	return is_lose;
}

void blinking_arrow(struct head* snake)
{
	int i;
	int sec = 3;

	for (i = 0; i < sec; i++) {
		mvwaddch(game_win, snake->pos.y, snake->pos.x, ' ');
		wrefresh(game_win);
		timeout(500);
		key = getch();

		mvwaddch(game_win, snake->pos.y, snake->pos.x, snake->head_symbol);
		wrefresh(game_win);
		timeout(500);
		key = getch();
	}
}

void game_over(struct head* snake)
{
	blinking_arrow(snake);
	game_over_screen();
}

void update_data()
{
	game_time = time(NULL) - abs_time;
	mvwprintw(score_win, score_coords.y, score_coords.x, "%d", score);
	mvwprintw(score_win, time_coords.y, time_coords.x, "%d", game_time);
	wrefresh(score_win);
}

void movement(struct head *snake)
{
	static int grow_flag = 0;

	if (set_goal(snake)) {
		snake_grow(snake);
		mv(&snake->pos, snake->dir);
		grow_flag = 1;
		score++;
	}
	if (grow_flag == 0) {
		rem_last_char(*snake);
		mv_snake(snake);
		direction_normalization(snake, snake->first);
		grow_flag = 0;
	}
	print_snake(*snake);
	update_data();
	grow_flag = 0;
}

void game_start()
{
	//TODO [Place the loop here for GameOver screen's "try again"]
	//TODO [Make more clear and convinient connecting between functions main(), game_start() and game_over()]
	struct head snake;
	position_t gwin_max;
	int last_key;

	game_init();
	nodelay(game_win, TRUE);
	getmaxyx(game_win, gwin_max.y, gwin_max.x);
	snake.first = NULL;
	snake.last = NULL;
	snake.length = 0;
	snake.pos.x = gwin_max.x/2;
	snake.pos.y = gwin_max.y/2;
	snake.dir.x = -1;
	snake.dir.y = 0;
	snake.head_symbol = left_arrow;
	snake.goal_cords = rand_goal(snake);
	last_key = KEY_LEFT;
	print_snake(snake);
	
	while((key = getch()) != key_escape) {
		switch(key) {
			case ERR:
just_move:
				movement(&snake);
				if (lose_condition(&snake)) {
					process = 2;
					goto game_over;
				}
				break;
			 case KEY_UP:
				 if(last_key == KEY_DOWN) {
					 goto just_move;
				 }
				 last_key = KEY_UP;
				 snake.head_symbol = up_arrow;
				 snake.dir.x = 0;
				 snake.dir.y = -1;
				 movement(&snake);
				 if (lose_condition(&snake)) {
					 process = 2;
					 goto game_over;
				 }
				 break;
			 case KEY_DOWN:
				 if(last_key == KEY_UP) {
					 goto just_move;
				 }
				 last_key = KEY_DOWN;
				 snake.head_symbol = down_arrow;
				 snake.dir.x = 0;
				 snake.dir.y = 1;
				 movement(&snake);
				 if (lose_condition(&snake)) {
					 process = 2;
					 goto game_over;
				 }
				 break;
			 case KEY_LEFT:
				 if(last_key == KEY_RIGHT) {
					 goto just_move;
				 }
				 last_key = KEY_LEFT;
				 snake.head_symbol = left_arrow;
				 snake.dir.x = -1;
				 snake.dir.y = 0;
				 movement(&snake);
				 if (lose_condition(&snake)) {
					 process = 2;
					 goto game_over;
				 }
				 break;
			 case KEY_RIGHT:
				 if(last_key == KEY_LEFT) {
					 goto just_move;
				 }
				 last_key = KEY_RIGHT;
				 snake.head_symbol = right_arrow;
				 snake.dir.x = 1;
				 snake.dir.y = 0;
				 movement(&snake);
				 if (lose_condition(&snake)) {
					 process = 2;
					 goto game_over;
				 }
				 break;
		}
	}
	process = 0;
game_over:
	if (process == 2) {
		game_over(&snake);

		return;
	}
	
}

void controls_menu_init() {
	position_t maxcords, curcords;

	erase();
	getmaxyx(stdscr, maxcords.y, maxcords.x);
	curcords.x = maxcords.x / 2;
	curcords.y = 2;
	print_menu(curcords, controls_title);

	curcords.y += 2;
	print_menu(curcords, controls_uparrow);

	curcords.y += 1;
	print_menu(curcords, controls_downarrow);

	curcords.y += 1;
	print_menu(curcords, controls_leftarrow);

	curcords.y += 1;
	print_menu(curcords, controls_rightarrow);

	curcords.y += 2;
	print_menu(curcords, controls_boost);

	curcords.y += 2;
	print_menu(curcords, controls_esc);

	curcords.y = maxcords.y - 2;
	back_cords.y = curcords.y;
	back_cords.x = curcords.x - strlen(returnBackText) / 2;
	print_menu(curcords, returnBackText);

	move(back_cords.y, back_cords.x - 2);
	addch(menu_symb);
}

void controls_menu() {

	controls_menu_init();
	while ( (key = getch()) != key_enter) {

	}
}

int main()
{
	screenInit();
	drawMainMenu();

	selectionPoint_t* menuSelectionPoint;
	menuSelectionPoint->currentItem = start_item;
	menuSelectionPoint->selectionSymbol = menu_symb;

	mainMenuItems_t currentMenuItem = start_item;

	while((key = getch()) != key_escape) {
		switch(key) {
			case KEY_DOWN:
				currentMenuItem++;
				if(currentMenuItem >= maxMainMenuItemsCount)
					currentMenuItem = currentMenuItem % maxMainMenuItemsCount;
				mv_menu(currentMenuItem, menuSelectionPoint);
				menuSelectionPoint->currentItem = currentMenuItem;
				break;
			case KEY_UP:
				currentMenuItem--;
				if(currentMenuItem >= maxMainMenuItemsCount)
					currentMenuItem = maxMainMenuItemsCount - 1;
				mv_menu(currentMenuItem, menuSelectionPoint);
				menuSelectionPoint->currentItem = currentMenuItem;
				break;
			case key_enter:
				if (currentMenuItem == start_item) {
					do
					{
						game_start();
					} while (process == 1);
					
					drawMainMenu();
				} 
				else if (currentMenuItem == exit_item) {
					goto end;
				}
				else if (currentMenuItem == controls_item) {
					controls_menu();
					drawMainMenu();
				}
				break;
		}


	}
end:
	endwin();
	return 0;
}

void game_over_screen()
{
	game_over_screen_init();

	itemNames_t item = again_item;

	while ((key = getch()) != key_escape) {
		switch (key) {
		case KEY_DOWN:
			if (item == again_item) {
				move_menu(to_menu_coords, again_coords);
				item = to_menu_item;
			}
			else {
				move_menu(again_coords, to_menu_coords);
				item = again_item;
			}
			break;
		case KEY_UP:
			if (item == to_menu_item) {
				move_menu(again_coords, to_menu_coords);
				item = again_item;
			}
			else {
				move_menu(to_menu_coords, again_coords);
				item = to_menu_item;
			}
			break;
		case key_enter:
			if (item == again_item) {
				process = 1;
				return;
			}
			else if (item == to_menu_item) {
				process = 0;
				return;
			}
			break;
		}
	}
}
