/**
 * ENGG1110 Problem Solving by Programming
 *
 * Course Project
 *
 * I declare that the project here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   https://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name  : Paisal, Pavat Zhang
 * Student ID    : 1155280121
 * Class/Section : A
 * Date          : 26/11/2026
 */


// [DO NOT MODIFY]
// DO NOT ADD new header files

#include <stdio.h>

#define MAX_ROWS 20
#define MAX_COLS 40
#define INITIAL_RANGE 1

// In Part 2, BOMB_FUSE must be changed.
#define BOMB_FUSE 4

/* Square states specified in the project document. */
#define CORRIDOR 7
#define WALL 9
#define BRICK_EMPTY 8
#define BRICK_BOOST 6
#define BOOST 5
#define PLAYER 1
#define PLAYER_ON_BOMB 3
#define BOMB 2
#define EXPLODE 4
#define EXPLODE_BOOST -1
// END OF [DO NOT MODIFY]

/* Indexes in the state[] array specified in the project document. */
#define STATE_FIELDS 15     // no. of fields
#define PLAYER_ROW 0		// player location
#define PLAYER_COL 1		// player location
#define PLAYER_ALIVE 2 		// player status (for game end checks)
#define EXPLOSION_RANGE 3 	// current bomb explosion range
#define BOMB_ACTIVE 4 		// current bomb state
#define BOMB_ROW 5			// current bomb row index
#define BOMB_COL 6			// current bomb column index
#define BOMB_TIMER 7		// current bomb timer value
#define ACTION_COUNT 8		// number of user valid actions (WASD + PB)
#define MOVE_COUNT 9		// number of user valid movement (WASD)
#define BOMB_COUNT 10		// total bombs dropped
#define BRICKS_REMAINS 11	// game statistics
#define BRICKS_DESTROYED 12	// game statistics
#define BOOSTS_DROPPED 13	// game statistics
#define BOOSTS_COLLECTED 14	// game statistics

// The GAME STATUS constants
#define GAME_RUNNING 0
#define GAME_WON 1
#define GAME_LOST 2

// [DO NOT MODIFY]
void initialise(int board[][MAX_COLS], int size[], int state[]);
void displayBoard(int board[][MAX_COLS], int size[], int state[]);
void displayLegend(int state[]);
int movePlayer(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange);
int dropBomb(int board[][MAX_COLS], int state[]);
void advanceTime(int board[][MAX_COLS], int size[], int state[]);
void explodeBomb(int board[][MAX_COLS], int size[], int state[]);
void endGameMessage(int state[], int win); // e.g., win can take GAME_WON or GAME_LOST or otherwise
// END OF [DO NOT MODIFY]

/* Define your helper functions prototype below. You can implement it somewhere else in this file.*/
int handleInput(int board[][MAX_COLS], int size[], int state[]);
int rayCheck(int board[][MAX_COLS], int state[], int rowChange, int colChange);


/* DO NOT DECLARE EXTRA GLOBAL VARIABLES */

int main(void) {
    // Suggested starter variables
    int board[MAX_ROWS][MAX_COLS];
    int size[2];
    int state[STATE_FIELDS];


    // Initialize the game board
    initialise(board, size, state);

    //return value of 1 in the functions means unsuccessful :)
    while (1) {
        if (!state[BRICKS_REMAINS] || !state[PLAYER_ALIVE])
            break;
        displayBoard(board, size, state);
        displayLegend(state);
        int inp = handleInput(board, size, state);
        if (inp == 1) {
            continue;
        } else if (inp == 2) {
            break; //if you quit the game
        }
        advanceTime(board, size, state);
    }

    displayBoard(board, size, state);
    displayLegend(state);
    if (!state[PLAYER_ALIVE]) {
        endGameMessage(state, GAME_LOST);
    }
    else if (!state[BRICKS_REMAINS]) {
        endGameMessage(state, GAME_WON);
    } else {
        endGameMessage(state, GAME_RUNNING); //quit
    }

    return 0;
}

int handleInput (int board[][MAX_COLS], int size[], int state[]) {
    printf("Command [W/A/S/D = move, B = bomb, P = pause, Q = quit]: ");
    char command;
    if (scanf(" %c", &command) != 1)
        return 2; //basically quit because scanf doesnt read anything
    if (command >= 'a' && command <= 'z')
        command -= 'a' - 'A';
    if (command == 'B') {
        if (dropBomb(board, state)) {
            return 1;
        }
    }
    else if (command == 'W') {
        if (movePlayer(board, size, state, -1, 0)) {
            return 1;
        }
        state[MOVE_COUNT]++;
    }
    else if (command == 'S') {
        if (movePlayer(board, size, state, 1, 0)) {
            return 1;
        }
        state[MOVE_COUNT]++;
    }
    else if (command == 'A') {
        if (movePlayer(board, size, state, 0, -1)) {
            return 1;
        }
        state[MOVE_COUNT]++;
    }
    else if (command == 'D') {
        if (movePlayer(board, size, state, 0, 1)) {
            return 1;
        }
        state[MOVE_COUNT]++;
    } else if (command == 'Q') {
        return 2;
    } else if (command == 'P') {
        //nothing
    }
    else {
        printf("Error: enter W, A, S, D, B, P, or Q.\n");
        return 1;
    }
    return 0;
}

void initialise(int board[][MAX_COLS], int size[], int state[]) {
    // The hardcoded game board
    int start[9][15] = {
        {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
        {9,1,7,7,8,7,7,7,7,6,7,7,7,7,9},
        {9,7,9,7,9,7,9,7,9,7,9,7,9,7,9},
        {9,7,7,7,8,7,7,7,7,7,8,7,7,7,9},
        {9,7,9,7,9,7,9,7,9,7,9,7,9,7,9},
        {9,6,7,7,7,7,8,7,7,7,7,8,7,7,9},
        {9,7,9,7,9,7,9,7,9,7,9,7,9,7,9},
        {9,7,7,7,7,8,7,7,7,7,7,7,7,7,9},
        {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9}
    };
    int row = 9;
    int col = 15;
    // Initialize the game board, size, and state
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int cur = start[i][j];
            board[i][j] = cur;
            if (cur == PLAYER) {
                state[PLAYER_ROW] = i;
                state[PLAYER_COL] = j;
                state[PLAYER_ALIVE] = 1;
                state[EXPLOSION_RANGE] = INITIAL_RANGE;
                state[BOMB_ACTIVE] = 0;
                state[BOMB_TIMER] = 1; //just not 0 so it doesn't explode at first
                //BOMB_ROW, BOMB_COL
                state[ACTION_COUNT] = 0;
                state[MOVE_COUNT] = 0;
                state[BOMB_COUNT] = 0;
                state[BRICKS_DESTROYED] = 0;
                state[BOOSTS_DROPPED] = 0;
                state[BOOSTS_COLLECTED] = 0;
            } else if (cur == BRICK_EMPTY || cur == BRICK_BOOST)
                state[BRICKS_REMAINS]++;
        }
    }

    size[0] = row;
    size[1] = col;
}

void displayBoard(int board[][MAX_COLS], int size[], int state[]) {
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            if (board[i][j] == CORRIDOR)
                printf("  ");
            else if (board[i][j] == WALL)
                printf("[]");
            else if (board[i][j] == EXPLODE || board[i][j] == EXPLODE_BOOST)
                printf("##");
            else if (board[i][j] == PLAYER)
                printf("@@");
            else if (board[i][j] == PLAYER_ON_BOMB)
                printf("@B");
            else if (board[i][j] == BRICK_EMPTY || board[i][j] == BRICK_BOOST)
                printf("()");
            else if (board[i][j] == BOOST)
                printf("EX");
            else
                printf("BB");
        }
        putchar('\n');
    }

    //Player / game info
    printf("Actions=%d Moves=%d Bombs=%d Range=%d Soft bricks=%d\n", state[ACTION_COUNT], state[MOVE_COUNT], state[BOMB_COUNT], state[EXPLOSION_RANGE], state[BRICKS_REMAINS]);
    if (state[BOMB_ACTIVE]) {
        printf("Bomb %d: timer=%d at (%d,%d)\n", state[BOMB_ACTIVE], state[BOMB_TIMER], state[BOMB_ROW], state[BOMB_COL]);
    }
    printf("Destroyed=%d Boosts dropped=%d Boosts collected=%d\n", state[BRICKS_DESTROYED], state[BOOSTS_DROPPED], state[BOOSTS_COLLECTED]);
}

void displayLegend(int state[]) {
    printf("Legend: [] wall, () soft brick, @@ player, @B player on bomb, BB bomb, EX boost, ## explosion\n");
}



int movePlayer(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange) {
    // Process a "move" command from user.
    // return value indicates success or fail
    int potential = board[state[PLAYER_ROW]+rowChange][state[PLAYER_COL]+colChange];
    if (potential == BRICK_EMPTY || potential == BRICK_BOOST || potential == BOMB || potential == WALL ) {
        printf("Error: a wall, soft brick, or bomb blocks the movement.\n");
        return 1;
    }
    if (board[state[PLAYER_ROW]][state[PLAYER_COL]] != PLAYER_ON_BOMB)
        board[state[PLAYER_ROW]][state[PLAYER_COL]] = CORRIDOR;
    else
        board[state[PLAYER_ROW]][state[PLAYER_COL]] = BOMB;
    board[state[PLAYER_ROW]+rowChange][state[PLAYER_COL]+colChange] = PLAYER;
    state[PLAYER_ROW] += rowChange;
    state[PLAYER_COL] += colChange;
    if (potential == BOOST || potential == EXPLODE_BOOST) {
        state[EXPLOSION_RANGE]++;
        state[BOOSTS_COLLECTED]++;
    }
    return 0;

}

int dropBomb(int board[][MAX_COLS], int state[]) {
    // Process a dropBomb command from user.
    if (state[BOMB_ACTIVE]) {
        printf("Error: an unexploded bomb already exists.\n");
        return 1;
    }
    state[BOMB_TIMER] = BOMB_FUSE;
    state[BOMB_ACTIVE] = 1;
    state[BOMB_ROW] = state[PLAYER_ROW];
    state[BOMB_COL] = state[PLAYER_COL];
    board[state[BOMB_ROW]][state[BOMB_COL]] = PLAYER_ON_BOMB;
    state[BOMB_COUNT]++;
    return 0;

}



void advanceTime(int board[][MAX_COLS], int size[], int state[]) {
    // Advance game time; explode bomb if needed
    //clear explosion tiles
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            if (board[i][j] == EXPLODE) {
                board[i][j] = CORRIDOR;
            } else if (board[i][j] == EXPLODE_BOOST) {
                board[i][j] = BOOST;
            }
        }
    }
    state[ACTION_COUNT]++;
    if (state[BOMB_ACTIVE]){
        state[BOMB_TIMER]--;
        if (!state[BOMB_TIMER])
            explodeBomb(board, size, state);
    }
}

int rayCheck(int board[][MAX_COLS], int state[], int rowChange, int colChange) {
    int potential =  board[state[BOMB_ROW]+rowChange][state[BOMB_COL]+colChange];
    if (potential == WALL) {
        return 1; //if wall then stop because explosions might phase through wall
    }
    else if (potential == PLAYER) {
        state[PLAYER_ALIVE] = 0;
        board[state[BOMB_ROW]+rowChange][state[BOMB_COL]+colChange] = EXPLODE;
    }
    else if (potential == CORRIDOR)
        board[state[BOMB_ROW]+rowChange][state[BOMB_COL]+colChange] = EXPLODE;
    else if (potential == BRICK_EMPTY) {
        board[state[BOMB_ROW]+rowChange][state[BOMB_COL]+colChange] = EXPLODE;
        state[BRICKS_REMAINS]--;
        state[BRICKS_DESTROYED]++;
        return 1;
    }
    else if (potential == BRICK_BOOST) {
        board[state[BOMB_ROW]+rowChange][state[BOMB_COL]+colChange] = EXPLODE_BOOST;
        state[BOOSTS_DROPPED]++;
        state[BRICKS_REMAINS]--;
        state[BRICKS_DESTROYED]++;
        return 1;
    } else if (potential == BOOST) {
        board[state[BOMB_ROW]+rowChange][state[BOMB_COL]+colChange] = EXPLODE_BOOST;
    }
    return 0;
}

void explodeBomb(int board[][MAX_COLS], int size[], int state[]) {
    // Explode the bomb; read the data from state[]/board[][]
    if (board[state[BOMB_ROW]][state[BOMB_COL]] == PLAYER_ON_BOMB) {
        state[PLAYER_ALIVE] = 0;
    }
    board[state[BOMB_ROW]][state[BOMB_COL]] = EXPLODE;
    int range = state[EXPLOSION_RANGE];
    //thanks for idea of rowChange and colChange!
    //up
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, state, -i, 0)) {
            break; //stop checking that direction
        }
    }
    //down
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, state, i, 0)) {
            break;
        }
    }
    //left
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, state, 0, -i)) {
            break;
        }
    }
    //right
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, state, 0, i)) {
            break;
        }
    }
    state[BOMB_ACTIVE] = 0;
}



void endGameMessage(int state[], int win) {
    // Display End Game Message and Statistics
    //0 is QUIT, 1 is WIN, 2 is LOST
    if (win == 2) {
        printf("[GAME OVER]: the player was caught in an explosion.\n");
    } else if (win == 1) {
        printf("[YOU WIN]: all soft bricks were destroyed.\n");
    } else
        printf("[GAME OVER]: the player quits.\n");
    printf("+-----------------------+-----+\n");

    printf("| Successful moves      |");
    int numDigits = 0;
    int tmp = state[MOVE_COUNT];
    do {
        numDigits++;
        tmp/=10;
    } while (tmp);
    //do this so that if the user does nothing and just quits then the spacing is still correct; I know this wasn't taught in the course.sorry!

    for (int i = 0; i < (4-numDigits); i++)
        printf(" ");
    printf("%d |\n", state[MOVE_COUNT]);
    printf("+-----------------------+-----+\n");

    printf("| Bombs dropped         |");
    numDigits = 0;
    tmp = state[BOMB_COUNT];
    do {
        numDigits++;
        tmp/=10;
    } while (tmp);
    for (int i = 0; i < (4-numDigits); i++)
        printf(" ");
    printf("%d |\n", state[BOMB_COUNT]);

    //in part2 bricks will probably go over 10 but for the time being I'll leave this as is
    printf("+-----------------------+-----+\n");
    printf("| Soft bricks destroyed |   %d |\n", state[BRICKS_DESTROYED]);
    printf("+-----------------------+-----+\n");
    if (win != 1) {
        printf("| Soft bricks remaining |   %d |\n", state[BRICKS_REMAINS]);
        printf("+-----------------------+-----+\n");
    }
}