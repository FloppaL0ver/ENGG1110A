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
 * Date          : 19/11/2026
 */

// [DO NOT MODIFY]
// DO NOT ADD new header files

#include <stdio.h>

#define MAX_ROWS 20
#define MAX_COLS 40
#define INITIAL_RANGE 1

// In Part 2, BOMB_FUSE must be changed.
#define BOMB_FUSE 6

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
#define KICK 10                 //I added this
#define EXPLODE_BOOST -1
// END OF [DO NOT MODIFY]

/* Indexes in the state[] array specified in the project document. */
#define STATE_FIELDS 18     // no. of fields
#define PLAYER_ROW 0		// player location
#define PLAYER_COL 1		// player location
#define PLAYER_ALIVE 2 		// player status (for game end checks)
#define EXPLOSION_RANGE 3 	// current bomb explosion range
#define BOMB_ACTIVE 4 		// current bomb state
#define BOMB1_ROW 5			// current bomb row index
#define BOMB1_COL 6			// current bomb column index
#define BOMB1_TIMER 7		// current bomb timer value
#define BOMB2_ROW 8			// current bomb row index
#define BOMB2_COL 9			// current bomb column index
#define BOMB2_TIMER 10		// current bomb timer value
#define ACTION_COUNT 11		// number of user valid actions (WASD + PB)
#define MOVE_COUNT 12		// number of user valid movement (WASD)
#define BOMB_COUNT 13		// total bombs dropped
#define BRICKS_REMAINS 14	// game statistics
#define BRICKS_DESTROYED 15	// game statistics
#define BOOSTS_DROPPED 16	// game statistics
#define BOOSTS_COLLECTED 17	// game statistics

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
void explodeAllDirs (int board[][MAX_COLS], int size[], int state[], int range, int row, int col, int bombtimer);
int rayCheck(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange, int row, int col, int bombtimer);
void saveGame(int board[][MAX_COLS], int size[], int state[]);
void kickBomb(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange);
void moveBomb(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange, int row, int col, int bombtimer);
void endSpacePrinter(int state[], int field);


/* DO NOT DECLARE EXTRA GLOBAL VARIABLES */

int main(void) {
    // Suggested starter variables
    int board[MAX_ROWS][MAX_COLS];
    int size[2];
    int state[STATE_FIELDS];
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
        }
        if (inp == 2) {
            break; //if you quit the game
        }
        advanceTime(board, size, state); //clears explosion/kick, advances timer, explodes bomb
        if (inp == 3) //w
            kickBomb(board, size, state, -1, 0);
        else if (inp == 4) //s
            kickBomb(board, size, state, 1, 0);
        else if (inp == 5) //a
            kickBomb(board, size, state, 0, -1);
        else if (inp == 6) //d
            kickBomb(board, size, state, 0, 1);
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

void initialise(int board[][MAX_COLS], int size[], int state[]) {
    int row,col;
    FILE *fptr = fopen("save.txt", "r");
    if (fptr != NULL) {
        printf("A save.txt exists. Reload? [Y/N]: ");
        char reload;
        scanf("%c", &reload);
        if (reload == 'Y' || reload == 'y') { //assuming if not Y or y then it means no
            fscanf(fptr, "%d%d", &row, &col);
            // Initialize the game board, size, and state
            for (int i = 0; i < STATE_FIELDS; i++) {
                fscanf(fptr, "%d", &state[i]);
            }
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    int cur;
                    if (fscanf(fptr, "%d", &cur) != 1) //because we'll scan a lot of stuff it might break
                        break;
                    board[i][j] = cur;
                }
            }
            size[0] = row;
            size[1] = col;
            fclose(fptr);
            return;
        }
        fclose(fptr);
    }

    // new game from board.txt
    int brick = 0;
    FILE *fptr2 = fopen("board.txt", "r"); //assuming that there will always be board.txt available
    fscanf(fptr2, "%d%d", &row, &col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int cur;
            if (fscanf(fptr2, "%d", &cur) != 1)
                break;
            board[i][j] = cur;
            if (cur == PLAYER) {
                state[PLAYER_ROW] = i;
                state[PLAYER_COL] = j;
                state[PLAYER_ALIVE] = 1;
                state[EXPLOSION_RANGE] = INITIAL_RANGE;
                state[BOMB_ACTIVE] = 0;
                state[BOMB1_TIMER] = -1;
                state[BOMB2_TIMER] = -1;
                //BOMB_ROW, BOMB_COL, BRICKS_REMAIN will be done later
                state[ACTION_COUNT] = 0;
                state[MOVE_COUNT] = 0;
                state[BOMB_COUNT] = 0;
                state[BRICKS_DESTROYED] = 0;
                state[BOOSTS_DROPPED] = 0;
                state[BOOSTS_COLLECTED] = 0;
            } else if (cur == BRICK_EMPTY || cur == BRICK_BOOST)
                brick++;
        }
    }
    size[0] = row;
    size[1] = col;
    state[BRICKS_REMAINS] = brick;
    fclose(fptr2);
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
            else if (board[i][j] == KICK)
                printf("++");
            else
                printf("BB");
        }
        putchar('\n');
    }

    //Player / game info
    printf("Actions=%d Moves=%d Bombs=%d Range=%d Soft bricks=%d\n", state[ACTION_COUNT], state[MOVE_COUNT], state[BOMB_COUNT], state[EXPLOSION_RANGE], state[BRICKS_REMAINS]);
    if (state[BOMB_ACTIVE]) {
        if (state[BOMB1_TIMER] > 0)
            printf("Bomb 1: timer=%d at (%d,%d)\n", state[BOMB1_TIMER], state[BOMB1_ROW], state[BOMB1_COL]);
        if (state[BOMB2_TIMER] > 0)
            printf("Bomb 2: timer=%d at (%d,%d)\n", state[BOMB2_TIMER], state[BOMB2_ROW], state[BOMB2_COL]);
    }
    printf("Destroyed=%d Boosts dropped=%d Boosts collected=%d\n", state[BRICKS_DESTROYED], state[BOOSTS_DROPPED], state[BOOSTS_COLLECTED]);
}

void displayLegend(int state[]) {
    printf("Legend: [] wall, () soft brick, @@ player, @B player on bomb, BB bomb, EX boost, ++ kick path, ## explosion\n");
}


int handleInput (int board[][MAX_COLS], int size[], int state[]) {
    printf("Command [W/A/S/D = move, B = bomb, P = pause, Q = save and quit]: ");
    char command;
    int returnV;
    if (scanf(" %c", &command) != 1)
        return 2; //basically quit because scanf doesnt read anything
    if (command >= 'a' && command <= 'z')
        command -= 'a' - 'A';
    if (command == 'B') {
        if (dropBomb(board, state) == 1) {
            return 1;
        }
    }
    else if (command == 'W') {
        returnV = movePlayer(board, size, state, -1, 0);
        if (returnV == 1) {
            return 1;
        }
        state[MOVE_COUNT]++; //increment move count too if kick bomb
        if (returnV == 2) { //if kick bomb
            return 3; //up
        }
    }
    else if (command == 'S') {
        returnV = movePlayer(board, size, state, 1, 0);
        if (returnV == 1) {
            return 1;
        }
        state[MOVE_COUNT]++;
        if (returnV == 2) {
            return 4; //down
        }
    }
    else if (command == 'A') {
        returnV = movePlayer(board, size, state, 0, -1);
        if (returnV == 1) {
            return 1;
        }
        state[MOVE_COUNT]++;
        if (returnV == 2) {
            return 5; //left
        }
    }
    else if (command == 'D') {
        returnV = movePlayer(board, size, state, 0, 1);
        if (returnV == 1) {
            return 1;
        }
        state[MOVE_COUNT]++;
        if (returnV == 2) {
            return 6; //right
        }
    } else if (command == 'Q') {
        saveGame(board, size, state);
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


int movePlayer(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange) {
    // Process a "move" command from user.
    // return value indicates success or fail
    int potential = board[state[PLAYER_ROW]+rowChange][state[PLAYER_COL]+colChange];
    if (potential == BRICK_EMPTY || potential == BRICK_BOOST || potential == WALL ) {
        printf("Error: movement or kick is blocked.\n");
        return 1;
    } if (potential == BOMB) {
        if (board[state[PLAYER_ROW]+2*rowChange][state[PLAYER_COL]+2*colChange] == BOMB) {
            printf("Error: movement or kick is blocked.\n");
            return 1;
        }
        return 2; //kick bomb
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
    if (state[BOMB_ACTIVE] == 2) {
        printf("Error: no available bombs.\n");
        return 1;
    } if (board[state[PLAYER_ROW]][state[PLAYER_COL]] == PLAYER_ON_BOMB) {
        printf("Error: a bomb is already at this location.\n");
        return 1;
    }
    if (!state[BOMB_ACTIVE] || (state[BOMB_ACTIVE] == 1 && state[BOMB2_TIMER] > 0)) {
        state[BOMB1_TIMER] = BOMB_FUSE;
        state[BOMB_ACTIVE]++;
        state[BOMB1_ROW] = state[PLAYER_ROW];
        state[BOMB1_COL] = state[PLAYER_COL];
        board[state[BOMB1_ROW]][state[BOMB1_COL]] = PLAYER_ON_BOMB;
        state[BOMB_COUNT]++;
    } else {
    //BOMB_ACTIVE == 1
        state[BOMB2_TIMER] = BOMB_FUSE;
        state[BOMB_ACTIVE]++;
        state[BOMB2_ROW] = state[PLAYER_ROW];
        state[BOMB2_COL] = state[PLAYER_COL];
        board[state[BOMB2_ROW]][state[BOMB2_COL]] = PLAYER_ON_BOMB;
        state[BOMB_COUNT]++;
    }
    return 0;
}



void advanceTime(int board[][MAX_COLS], int size[], int state[]) {
    // Advance game time; explode bomb if needed
    //clear explosion tiles
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            if (board[i][j] == EXPLODE || board[i][j] == KICK) {
                board[i][j] = CORRIDOR;
            } else if (board[i][j] == EXPLODE_BOOST) {
                board[i][j] = BOOST;
            }
        }
    }
    state[ACTION_COUNT]++;
    if (state[BOMB_ACTIVE]){
        if (state[BOMB1_TIMER] > 0)
            state[BOMB1_TIMER]--;
        if (state[BOMB2_TIMER] > 0)
            state[BOMB2_TIMER]--;
        if (!state[BOMB1_TIMER] || !state[BOMB2_TIMER])
            //printf("Explosion\n\n");
            explodeBomb(board, size, state); //will explode one or both bombs
    }
}

int rayCheck(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange, int row, int col, int bombtimer) {
    int potential =  board[state[row]+rowChange][state[col]+colChange];
    if (potential == WALL) {
        return 1; //if wall then stop because explosions might phase through wall
    }
    if (potential == PLAYER) {
        state[PLAYER_ALIVE] = 0;
        board[state[row]+rowChange][state[col]+colChange] = EXPLODE;
    }
    else if (potential == CORRIDOR || potential == KICK)
        board[state[row]+rowChange][state[col]+colChange] = EXPLODE;
    else if (potential == BRICK_EMPTY) {
        board[state[row]+rowChange][state[col]+colChange] = EXPLODE;
        state[BRICKS_REMAINS]--;
        state[BRICKS_DESTROYED]++;
        return 1;
    }
    else if (potential == BRICK_BOOST) {
        board[state[row]+rowChange][state[col]+colChange] = EXPLODE_BOOST;
        state[BOOSTS_DROPPED]++;
        state[BRICKS_REMAINS]--;
        state[BRICKS_DESTROYED]++;
        return 1;
    } else if (potential == BOOST) {
        board[state[row]+rowChange][state[col]+colChange] = EXPLODE_BOOST;
    } else if (potential == BOMB || potential == PLAYER_ON_BOMB) {
        state[BOMB1_TIMER] = 0;
        state[BOMB2_TIMER] = 0; //set both to 0 first because I don't want to keep track of which bomb is currently exploding
        state[bombtimer] = 1; //does not re-explode
        explodeBomb(board, size, state);
    }
    return 0;
}

void explodeAllDirs (int board[][MAX_COLS], int size[], int state[], int range, int row, int col, int bombtimer) {
    if (board[state[row]][state[col]] == PLAYER_ON_BOMB) {
        state[PLAYER_ALIVE] = 0;
    }
    board[state[row]][state[col]] = EXPLODE;
    //up
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, size, state, -i, 0, row, col, bombtimer)) {
            break; //stop checking that direction
        }
    }
    //down
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, size, state, i, 0, row, col, bombtimer)) {
            break;
        }
    }
    //left
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, size, state, 0, -i, row, col, bombtimer)) {
            break;
        }
    }
    //right
    for (int i = 1; i <= range; i++) {
        if (rayCheck(board, size, state, 0, i, row, col, bombtimer)) {
            break;
        }
    }
    state[bombtimer] = -1;
    state[BOMB_ACTIVE]--;
}

void explodeBomb(int board[][MAX_COLS], int size[], int state[]) {
    // Explode the bomb; read the data from state[]/board[][]
    int range = state[EXPLOSION_RANGE];
    if (state[BOMB1_TIMER] == 0) {
        //I didn't want to 'copy and paste' the function code twice but my implementation of the way I track bomb1 and bomb2 don't really allow me another method (at least that I can think of), so apologies for the inception of functions
        explodeAllDirs(board, size, state, range, BOMB1_ROW, BOMB1_COL, BOMB1_TIMER);
    }
    if (state[BOMB2_TIMER] == 0) { //so that both bombs can explode if their timers hit 0
        explodeAllDirs(board, size, state, range, BOMB2_ROW, BOMB2_COL, BOMB2_TIMER);
    }
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
    endSpacePrinter(state, MOVE_COUNT);
    printf("+-----------------------+-----+\n");
    printf("| Bombs dropped         |");
    endSpacePrinter(state, BOMB_COUNT);
    printf("+-----------------------+-----+\n");
    printf("| Soft bricks destroyed |");
    endSpacePrinter(state, BRICKS_DESTROYED);
    printf("+-----------------------+-----+\n");
    if (win != 1) {
        printf("| Soft bricks remaining |");
        endSpacePrinter(state, BRICKS_REMAINS);
        printf("+-----------------------+-----+\n");
    }
}

void endSpacePrinter(int state[], int field) {
    int numDigits = 0;
    int tmp = state[field];
    do { //do this so that if the user does nothing and just quits then the spacing is still correct; I know this wasn't taught in the course.sorry!
        numDigits++;
        tmp/=10;
    } while (tmp);
    for (int i = 0; i < (4-numDigits); i++)
        printf(" ");
    printf("%d |\n", state[field]);
}

void saveGame(int board[][MAX_COLS], int size[], int state[]) {
    FILE *fptr = fopen("save.txt", "w");
    fprintf(fptr, "%d %d\n", size[0], size[1]);
    for (int i = 0; i < STATE_FIELDS; i++)
        fprintf(fptr, "%d ", state[i]);
    fputc('\n', fptr); //found this in stdio.h documentation seems like putchar
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            fprintf(fptr, "%d ", board[i][j]); //initially I did this without space and wasted so much time debugging :(
        }
        fputc('\n', fptr);
    }
    printf("Game saved in save.txt.\n");
    fclose(fptr);
}

void kickBomb(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange) {
    if (state[BOMB1_ROW] == state[PLAYER_ROW]+rowChange && state[BOMB1_COL] == state[PLAYER_COL]+colChange && state[BOMB1_TIMER] > 0) {
        moveBomb(board, size, state, rowChange, colChange, BOMB1_ROW, BOMB1_COL, BOMB1_TIMER); //needs BOMB1 row and col so can set position after move
    } else if (state[BOMB2_ROW] == state[PLAYER_ROW]+rowChange && state[BOMB2_COL] == state[PLAYER_COL]+colChange && state[BOMB2_TIMER] > 0)
        moveBomb(board, size, state, rowChange, colChange, BOMB2_ROW, BOMB2_COL, BOMB2_TIMER);
}
//maybe I could've come up with more imaginative names
void moveBomb(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange, int row, int col, int bombtimer) {
    board[state[row]][state[col]] = KICK;
    for (int i = 1; i <= MAX_COLS-1; i++){ //doesn't matter what condition is could also just do while loop with i
        int potential = board[state[row]+ i * rowChange][state[col]+ i * colChange];
        if (potential == CORRIDOR) {
            board[state[row]+ i * rowChange][state[col]+ i * colChange] = KICK;
            continue;
        }
        if (potential == BRICK_EMPTY || potential == BRICK_BOOST || potential == BOOST || potential == WALL) {
            board[state[row]+ (i-1) * rowChange][state[col]+ (i-1) * colChange] = BOMB;
            state[row] += (i-1) * rowChange;
            state[col] += (i-1) * colChange;
        }
        else if (potential == EXPLODE || potential == EXPLODE_BOOST) {
            state[row] += i * rowChange; //explodes at ray
            state[col] += i * colChange;
            state[bombtimer] = 0;
            explodeBomb(board, size, state);
        }
        break; //not possible to be player, player_on_bomb, or kick
    }
}