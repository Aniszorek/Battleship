#include "battleshipFuntion.c"

//display welcome message
void welcomeScreen (void) {
	printf ("BATTLESHIP");
	printf ("\n\n");
	printf ("ZASADY GRY:\n");
    printf ("1. W grze uczestniczy dwóch graczy\n\n"); 
	printf ("2. Istnieją 4 typy statków, ktore nalezy stawiać na planszy od największego do najmniejszego\n"); 
	printf ("   jeden [c] czteromasztowiec (ma 4 kratki)\n");
    printf ("   dwa [a][t] trójmasztowce (3 kratki)\n");
    printf ("   trzy [u][i][e] dwumasztowce (2 kratki)\n");
    printf ("   cztery [s] jednomasztowce (1 kratka)\n\n");
	printf ("3. Komputer losowo wybierze który gracz zaczyna\n\n");
	printf ("4. Kazdy z graczy próbuje trafic w statki przeciwnika; [*] - trafiony i [M] - chybiony \n\n");
	printf ("5. Pierwszy gracz, który zgadnie pozycje wszystkich statkow przeciwnika, wygrywa.\n\n");
}

void mag () {
  printf("\x1B[35m");
}
void blue() {
  printf("\033[0;34m");
}
void reset () {
  printf("\033[0m");
}
//create empty game board
void createGameBoard (char gameBoard[][COLS]) {
	int i = 0, j = 0;

	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) {
			gameBoard[i][j]          = WATER;
		}
}   

//print game board on console
void printGameBoard(char gameBoard[][COLS],Boolean showAll){
    int i,j;
    printf ("\n   0  1  2  3  4  5  6  7  8  9");

    for (i = 0; i< ROWS;i++){
        printf("\n%d ", i);

        for (j=0;j<COLS;j++){
            if (showAll) printf("%2c ",gameBoard[i][j]);
            else{
                switch(gameBoard[i][j]){
                    case HIT:   
					printf ("%2c ", HIT);   
					break;
					case MISS:  printf ("%2c ", MISS);  break;
					case WATER: 
					default:    
					blue();
					printf ("%2c ", WATER); 
					reset();
					break;
                }
                
            }
        }
    }
	printf("\n");
}

// places ship on the game board using x,y position, and 0,1 direction (horizontal,vertical)
void putShipOnGameBoard (char gameBoard[][COLS], Unit ship, Coordinate position, int direction){
	if (direction == HORIZONTAL) {
		if (position.column - 1>=0){
			gameBoard [position.row][position.column-1] = FIELD;
			if (position.row - 1 >= 0) gameBoard [position.row-1][position.column-1] = FIELD;
			if (position.row + 1 <= 9) gameBoard [position.row+1][position.column-1] = FIELD;
		}
		if (position.column+ship.length <=9){
			gameBoard [position.row][position.column + ship.length] = FIELD;
			if (position.row+1 <= 9) gameBoard [position.row+1][position.column+ship.length] = FIELD;
			if (position.row-1 >=0) gameBoard [position.row-1][position.column+ship.length] = FIELD;
		}
	}

	if (direction == VERTICAL) {
		if (position.row - 1>=0){
			gameBoard [position.row-1][position.column] = FIELD;
			if (position.column - 1 >= 0) gameBoard [position.row-1][position.column-1] = FIELD;
			if (position.column + 1 <= 9) gameBoard [position.row-1][position.column+1] = FIELD;	
		}
		if (position.row+ship.length <=9) {
			gameBoard [position.row + ship.length][position.column] = FIELD;
			if (position.column+1 <=9) gameBoard [position.row+ship.length][position.column+1] = FIELD;
			if (position.column-1 >=0) gameBoard [position.row+ship.length][position.column-1] = FIELD;
			}
	}
    for (int i = 0; i < ship.length; i++) {
		if (direction == HORIZONTAL) {
			gameBoard [position.row][position.column + i] = ship.symbol;
			if (position.row - 1>=0) gameBoard [position.row - 1][position.column+i] = FIELD;
			if (position.row + 1<=9) gameBoard [position.row + 1][position.column+i] = FIELD;}
		else // VERTICAL 
		{
			gameBoard [position.row + i][position.column] = ship.symbol;
			if (position.column - 1>=0) gameBoard [position.row + i ][position.column-1] = FIELD;
			if (position.column + 1<=9) gameBoard [position.row + i ][position.column+1] = FIELD;}
	}

}

//function allows user to manually place ship on the game board
void manuallyPlaceShips (char gameBoard[][COLS], Unit ship[]) {
	int x=-1,y=-1,direction;
	int i, j;

	Coordinate position[NUM_OF_SHIPS];
	Boolean isValid = FALSE;

	fflush (stdin);

	for (i = 0; i < NUM_OF_SHIPS; i++) {

		while (TRUE) {
            system("clear");
			printGameBoard (gameBoard, TRUE);
			printf ("\n> Prosze wprowadzic koordynaty pozycji od ktorej chcesz zaczac umieszczac statek\n");
            printf ("> (mozesz go umiescic od tej pozycji, w dol lub w prawo) %s o dlugosci %d:\n", ship[i].name, ship[i].length);
			printf("> ");
            scanf (" %d", &x); if (x>9 || x<0) {printf("Nie mozna umiescic w ten sposob tego statku"); continue;}
            position[i].row = x; 
            scanf (" %d", &y); if (y>9 || y<0) {printf("Nie mozna umiescic w ten sposob tego statku"); continue;}
            position[i].column = y; 
			if (ship[i].symbol!='s'){
				printf ("> Prosze wprowadzic \n> 0 jesli chcesz zeby statek skierowany byl w prawo \n> 1 jesli chcesz zeby statek skierowany byl w dol\n");
				printf ("> ");
				scanf (" %d", &direction); if (direction!=0 && direction!= 1) {printf("Nie mozna umiescic w ten sposob tego statku"); continue;}
			}
			else direction = 0;

            isValid = TRUE;
            int cnt = 0;
            for (j=0;j<ship[i].length;j++){
                if (direction == HORIZONTAL){
                    if (position[i].row + position[i].column + j > 18) continue;
                    if (gameBoard[position[i].row][position[i].column+j] == WATER) cnt++;}
                // VERTICAL
                else {
                    if (position[i].row + j + position[i].column > 18) continue;
                    if (gameBoard[position[i].row+j][position[i].column] == WATER) cnt++;}
                }
            if (cnt == ship[i].length) putShipOnGameBoard(gameBoard,ship[i],position[i],direction);
            else {
                isValid = FALSE;
                printf("Nie mozna umiescic w ten sposob tego statku");
                }
			if (isValid == TRUE) break;
        }
	}
}

// function to randomly place ships on the game board
void randomlyPlaceShips(char gameBoard[][COLS], Unit ship[]) {
	Coordinate position;
	int direction = -1;
	int i = 0;

	for (i = 0; i < NUM_OF_SHIPS; i++) {
		while (TRUE) {
			direction = getRandomNumber (0, 1); // 0 -> horizontal, 1 -> vertical 
			position = generatePosition (direction, ship[i].length);

			if (isValidLocation (gameBoard, position, direction, ship[i].length)) break;
		}

		putShipOnGameBoard (gameBoard, ship[i], position, direction);
	}
}

// function to update game board if target is hit or miss
void updateGameBoard (char gameBoard[][COLS], Coordinate target) {
	switch (gameBoard[target.row][target.column]) {
		//miss 
		case WATER: 
		case FIELD:
			gameBoard[target.row][target.column] = MISS;
			break;
		//hit
		case CARRIER: 
		case BATTLESHIP1:
		case BATTLESHIP2:
		case CRUISER1:
		case CRUISER2:
		case CRUISER3:
		case SUBMARINE: gameBoard[target.row][target.column] = HIT; break;
		case HIT: break;
		case MISS: break;
		default: break;
	}	
}

// function to get random target
Coordinate moveEasyBot (char gameBoard[][COLS]){
    Coordinate target;
    Boolean isValid = TRUE;
    while(isValid){
        target.row = getRandomNumber(0,ROWS-1);
        target.column = getRandomNumber(0,COLS-1);
		switch(gameBoard[target.row][target.column]){
			case WATER: 	 
			case CARRIER: 	 
			case BATTLESHIP1: 
			case BATTLESHIP2: 
			case CRUISER1: 	 
			case CRUISER2: 
			case CRUISER3: 
			case SUBMARINE:  isValid = FALSE; break;
			case HIT:		 isValid = TRUE; break;
			case MISS:       isValid = TRUE; break;
			default:		 isValid = FALSE; break;
		}
    }
    //updateGameBoard(gameBoard,position);

	return target;
}

// function checks if placing a ship in a certain place is possible
Boolean isValidLocation (char gameBoard[][COLS], Coordinate position, 
				         int direction, int length) {
	int i;
	Boolean isValid = TRUE;

	for (i = 0; i < length; i++) {
		if (direction == HORIZONTAL) {
			if (gameBoard [position.row][position.column + i] != WATER &&
				(position.column + i) < COLS) {isValid = FALSE; break;}
		} else { /* VERTICAL */
			if (gameBoard [position.row + i][position.column] != WATER &&
				(position.row + i) < ROWS) {isValid = FALSE; break;}
		}
	}

	return isValid;
}

// generate random position based on the direction and length
Coordinate generatePosition (int direction, int length) {
	Coordinate position;

	if (direction == HORIZONTAL) {
		position.row    = getRandomNumber (0, ROWS - 1);
		position.column = getRandomNumber (0, COLS - 1 - length);
	} else { //vertical
		position.row    = getRandomNumber (0, ROWS - 1 - length);
		position.column = getRandomNumber (0, COLS - 1);
	}

	return position;
}

// draw random number 
int getRandomNumber (int lowest, int highest) {
	if (lowest == 0)
		return rand () % ++highest;
	
	if (lowest > 0)
		return rand () % highest + lowest;
}

//print message on the console and awaits for user to send any char 
void printMessage(char* string){
	printf ("%s", string);
    while(getchar() != '\n');
	/*
	fflush(stdin);
	char str[2];
	printf ("%s", string);
	fgets(str,2,stdin);
	*/
}
int checkShot (char gameBoard[][COLS], Coordinate target) {
	int hit = -2;
	if (target.row>9 || target.row<0 || target.column>9 || target.column <0 ){
		hit = -1;
		return -1;
	}
	switch (gameBoard[target.row][target.column]) {
		// miss
		case WATER: 
		case FIELD:
			hit = 0;
			break;

		// hit
		case CARRIER:
		case BATTLESHIP1: 
		case BATTLESHIP2: 
		case CRUISER1: 	 
		case CRUISER2: 
		case CRUISER3: 
		case SUBMARINE:  hit = 1; break;

		// bad shot
		case HIT:		 hit = -1; break;
		case MISS:       hit = -1; break;
		default:		 hit = -1; break;
	}	

	return hit;
}

// read row and column from user
Coordinate getTarget (void) {
	Coordinate target;

	printf ("> Proszę wprowadzić współrzędne, w które chcesz uderzyć:\n");
	printf ("> ");
	scanf ("%d %d", &target.row, &target.column);

	return target;
}

// checks if player is winner
Boolean isWinner(Stats players[], int player){
	Boolean isWin = FALSE;

	if (players[player-1].numHits == 20) 
		isWin = TRUE;

	return isWin;
}

void checkBoundsDirections (Boolean directions[], int bound, int direction) {
	switch (direction) {
		case NORTH: 
			if (bound < 0) 
				directions[0] = FALSE;
			else            
				directions[0] = TRUE;
			break;

		case SOUTH:
			if (bound > 9) 
				directions[1] = FALSE;
			else            
				directions[1] = TRUE;
			break;

		case WEST:
			if (bound < 0)  
				directions[2] = FALSE;
			else            
				directions[2] = TRUE;
			break;

		case EAST:
			if (bound > 9)  
				directions[3] = FALSE;
			else            
				directions[3] = TRUE;	
			break;
	}
}

Boolean checkDestructionShip (int destroyedShip[][NUM_OF_SHIPS], int player, char shipSymbol, FILE *stream) {
	Boolean sunked = FALSE;

	switch (shipSymbol) {
		case CARRIER:    
			if (--destroyedShip[player][0] == 0) {
				printf ("> Czteromasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
			}
			break;

		case BATTLESHIP1: 
			if (--destroyedShip[player][1] == 0) {
				printf ("> Trójmasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
			}
			break;

		case BATTLESHIP2: 
			if (--destroyedShip[player][2] == 0) {
				printf ("> Trójmasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
			}
			break;

		case CRUISER1:    
			if (--destroyedShip[player][3] == 0) {
				printf ("> Dwumasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
			}
			break;
		case CRUISER2:    
			if (--destroyedShip[player][4] == 0) {
				printf ("> Dwumasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
			}
			break;
		case CRUISER3:    
			if (--destroyedShip[player][5] == 0) {
				printf ("> Dwumasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
			}
			break;

		case SUBMARINE:  
			if (--destroyedShip[player][6] == 0) {
				printf ("> Jednomasztowiec gracza %d zatopiony!\n", player+1);

				sunked = TRUE;
				destroyedShip[player][6] = 1;
			}
			break;
	}

	return sunked;
}

void createDataBoard(int dataBoard[][COLS]){
	for (int i=0; i<ROWS; i++){
		for(int j = 0; j < COLS; j++){
			dataBoard[i][j]= 0;
		}
	}
}

void loadData(FILE* stream, int dataBoard[][COLS]){
	int x,y;
	while (fscanf(stream, "%d", &x) != EOF){
		if(fscanf(stream, "%d", &y) == EOF) break;
		dataBoard[x][y]++;
	}
}