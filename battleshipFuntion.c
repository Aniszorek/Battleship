#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>



// CONSTANT MACROS 

#define ROWS 10
#define COLS 10

#define LOG_FILE "battleship.log"

#define NUM_OF_SHIPS  10

#define HORIZONTAL    0
#define VERTICAL      1

#define PLAYER_ONE    1
#define PLAYER_TWO    2

#define EASY 1
#define NORMAL 2
#define HARD 3


#define WATER         '~'
#define FIELD         'x'
#define HIT           '*'
#define MISS          'M'

#define CARRIER       'c' // 4 spaces - 1 ship
#define BATTLESHIP    'b' // 3 spaces - 2 ship
	#define BATTLESHIP1   'a' 
	#define BATTLESHIP2   't' 
#define CRUISER       'r' // 2 spaces - 3 ship
	#define CRUISER1      'u' 	
	#define CRUISER2      'i' 		
	#define CRUISER3      'e' 	
#define SUBMARINE     's' // 1 spaces - 4 ship


#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

//

typedef enum {
	FALSE, TRUE
} Boolean;

typedef struct unit {
	char   symbol;
	short  length;
	char   *name;
} Unit;

typedef struct stats {
	int        numHits;
	int        numMisses;
	int        totalShots;
} Stats;

typedef struct coordinate {
	int        row;
	int        column;
} Coordinate;


// FUNCTION 
int          getRandomNumber                (int lowest, int highest);											
int          checkShot                      (char GameBoard[][COLS], Coordinate target);

void         welcomeScreen                  (void);                                                                     
void         createGameBoard                (char gameBoard[][COLS]);                                                   
void         printGameBoard                 (char gameBoard [][COLS],Boolean showAll);                                
void         putShipOnGameBoard             (char gameBoard[][COLS], Unit ship, Coordinate position, int direction);    
void         manuallyPlaceShips             (char gameBoard[][COLS], Unit ship[]);										
void         randomlyPlaceShips             (char gameBoard[][COLS], Unit ship[]);																			
void         updateGameBoard                (char gameBoard[][COLS], Coordinate target);														
void         printMessage                   (char *message);															
void         loadData                       (FILE *stream, int dataBoard[][COLS]);
void         createDataBoard                (int dataBoard[][COLS]);
void         checkBoundsDirections          (Boolean directions[], int bound, int direction) ;	

Boolean      isValidLocation                (char gameBoard[][COLS], Coordinate position, int direction, int length);   //done
Boolean      checkDestructionShip           (int destroyedShip[][NUM_OF_SHIPS], int player, char shipSymbol, FILE *stream);
Boolean      isWinner                       (Stats players[], int player);		

Coordinate   moveEasyBot                    (char gameBoard[][COLS]);			
Coordinate   generatePosition               (int direction, int length);												//done
Coordinate   getTarget                      (void);																		//done


typedef enum {
	CARRIER_L    = 4, 
	BATTLESHIP_L = 3,
	CRUISER_L    = 2, 
	SUBMARINE_L  = 1,
} ShipType;
