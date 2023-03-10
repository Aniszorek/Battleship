#include "battle.c"

void main (){
	srand(time(0)); // to create different sequence of random numbers on every program run

	char x[2];

    Stats players[2] = {{0, 0, 0}, {0, 0, 0}}; // hits|miss|total

    char gameBoard1[ROWS][COLS];  // game board player one
	char gameBoard2[ROWS][COLS];  // game board player two

	int dataBoard[ROWS][COLS];   //for data from log

    Coordinate target;              // x,y   
	

    int   shot    = 0; // temporary value to check if ship has been shot
    int   option  = 0; // play ship manually or randomly
    int   player = 0;  // 1 - player1, 2 - player 2
    int   difficulty = 0; // 1- easy, 2-normal, 3-hard

    char shipSymbol = '\0'; // temporary save symbol of ship

    int modeop=0;
    Boolean    mode;                                      // FALSE - player vs player, TRUE - player vs bot

    //bot mode
    Coordinate   targetBot;           // x,y for bot
    Coordinate   targetOrigin;
    Coordinate   targetTemp;

    Boolean    directions[4]   = {TRUE, TRUE, TRUE, TRUE};      //  North, South, West, East
    Boolean    huntMode       = TRUE;                           // mode of randomly selecting a target 
	Boolean    targetMode     = FALSE;                          // mode when there is a hit 
    Boolean    shipDestroyed = FALSE;                           // if ship destroyed
    Boolean    flipper        = TRUE;	                        //flips values of boolean 

	// hard bot
	Coordinate firstTarget; 									
	int maxShot = 0;

    int   i       = 0,             
    	  counter = 1;        

    int   north   = 0, // holds change of value when going north 
	      south   = 0, //                                  south 
	      east    = 0, //                                   east 
	      west    = 0; //                                   west 


    Unit ship[NUM_OF_SHIPS] = {{'c', 4, "Carrier1"}, 
                                {'a', 3, "Battleship1"},
                                {'t', 3, "Battleship2"},
                                {'u', 2, "Cruiser1"},
                                {'i', 2, "Cruiser2"},
                                {'e', 2, "Cruiser3"},
                                {'s', 1, "Submarine1"},
                                {'s', 1, "Submarine2"},
                                {'s', 1, "Submarine3"},
                                {'s', 1, "Submarine4"}};


    int destroyedShip[2][NUM_OF_SHIPS] = {{4, 3, 3, 2, 2, 2, 1, 1, 1, 1},    // controls damage 
	                                       {4, 3, 3, 2, 2, 2, 1, 1, 1, 1}};

    // start program
    FILE *outStream;  
	FILE *inStream;  
    outStream = fopen (LOG_FILE, "a");
	inStream = fopen (LOG_FILE, "r");

    welcomeScreen ();
	printMessage ("Nacisnij ENTER, żeby kontynuować\n> ");
	system("clear");



    createGameBoard(gameBoard1);
    createGameBoard(gameBoard2);  // creating boards without ships for player

	/*
	*/
    //game mode
    printf("> Proszę o wybranie opcji:\n");
    printf("> 1) Chcę zagrać gracz vs gracz\n");
    printf("> 2) Chcę zagrać gracz vs bot\n");
    while(modeop != 1 && modeop != 2){
        printf("> ");
        scanf("%d",&modeop);
        mode = modeop - 1;
    }

    
    if (mode){
        //difficulty level if player vs bot
        printf("> Proszę o wybranie poziomu trudności bota:\n");
        printf("> 1) Łatwy\n");
        printf("> 2) Normalny\n");
        printf("> 3) Trudny\n");
        while(difficulty!= 1 && difficulty != 2 && difficulty != 3){
            printf("> ");
            scanf("%d",&difficulty);
        }

		if(difficulty == 3){
			createDataBoard(dataBoard);
			loadData(inStream,dataBoard);
			fclose (inStream);
		}

        //manually put ships on game board only if player vs bot
        printf("> Proszę o wybranie opcji:\n");
        printf("> 1) Chcę manualnie wybrać rozmieszczenie statków\n");
        printf("> 2) Chcę, żeby statki zostały rozmieszczone losowo\n");
        while(option != 1 && option != 2){
            printf("> ");
            scanf("%d",&option);
            }

        switch(option){
        case 1:
        manuallyPlaceShips(gameBoard1,ship);
		break;
        case 2:
        randomlyPlaceShips(gameBoard1,ship);
		break;
        }
        // place ships for player 2
        randomlyPlaceShips(gameBoard2,ship);
        printf("\n> Plansza gracza numer dwa została wygenerowana.\n");
    }
    else {
        randomlyPlaceShips(gameBoard1,ship);
        printf("\n> Plansza gracza numer jeden została wygenerowana.\n");
        randomlyPlaceShips(gameBoard2,ship);
        printf("> Plansza gracza numer dwa została wygenerowana.\n\n");
    }
	//printGameBoard(gameBoard1,TRUE);
	//printGameBoard(gameBoard2,TRUE);

	// for hard bot
	for (int a = 0; a<10; a++){
		for (int b = 0; b<10;b++){
			if (gameBoard1[a][b] != WATER && gameBoard1[a][b] != FIELD){
				fprintf (outStream, "%d\n%d\n", a, b);
			} 
		}
	}

    player = getRandomNumber(1,2);
    printf ("> Gracz %d został losowo wybrany do rozpoczęcia rozgrywki.\n", player);

	fflush(stdout);
    printMessage("\n> Naciśnij ENTER, żeby kontynuować\n");
	fgets(x,2,stdin);
	system("clear");

	//printGameBoard(gameBoard2,TRUE);

    while(TRUE){

        switch (player) {
            case PLAYER_ONE:
            printf("\n> Plansza gracza numer dwa: \n");
            printGameBoard(gameBoard2,FALSE);
            printf("> Tura gracza numer jeden \n");

            do{
                target = getTarget (); 
				shot = checkShot (gameBoard2, target);
					
				// check if shot is possible
				if (shot == -1) 
					printf ("> Niemożliwy ruch, proszę wprowadzić dane ponownie \n");
            } while (shot == -1);
			system("clear");

            shipSymbol = gameBoard2[target.row][target.column];

            updateGameBoard(gameBoard2,target);
            printf("\nPlansza gracza 2 po zaktualizowaniu: \n");
            printGameBoard(gameBoard2,FALSE);

            break;

            case PLAYER_TWO:
            //if player vs player
            if(!mode){
                printf("\n> Plansza gracza numer jeden: \n");
                printGameBoard(gameBoard1,FALSE);
                printf("> Tura gracza numer dwa \n");

                do{
                    target = getTarget (); 
                    shot = checkShot (gameBoard1, target);
                        
                    // check if shot is possible
                    if (shot == -1) 
                        printf ("> Niemożliwy ruch, proszę wprowadzić dane ponownie \n");
                } while (shot == -1);
				system("clear");

                shipSymbol = gameBoard1[target.row][target.column];

                updateGameBoard(gameBoard1,target);
                printf("\nPlansza gracza 1 po zaktualizowaniu: \n");
                printGameBoard(gameBoard1,FALSE);
                break;
            }
            //if player vs bot
            switch (difficulty)
            {
            case EASY:
                printf("\n> Plansza gracza numer jeden: \n");
                printGameBoard(gameBoard1,FALSE);
                printf("> Tura komputera \n");

                target = moveEasyBot(gameBoard1);
                shipSymbol = gameBoard1[target.row][target.column];

                if (shipSymbol != WATER) shot = 1;
                updateGameBoard(gameBoard1,target);
				fflush(stdin);
        		printMessage("> Naciśnij ENTER, żeby kontynuować\n");
				system("clear");
                printf("\nPlansza gracza 1 po zaktualizowaniu: \n");
                printGameBoard(gameBoard1,FALSE);
                break;
            case NORMAL:
                printf("\n> Plansza gracza numer jeden: \n");
                printGameBoard(gameBoard1,FALSE);
                printf("> Tura komputera \n");

                /*
				  check from a previous cycle if a ship destroyed,
				  if yes, bot back randomly selecting a target
				 */
				if (shipDestroyed) {
					shipDestroyed = FALSE;
					targetMode = FALSE;
					huntMode = TRUE;
				}

                /*
				  TARGET MODE 
				  This mode is true when the computer randomly selects a target,
				  and it happens to be a hit, this mode is set to true.
				 */
				if (targetMode) {
					// retains value of targetBot
					target = targetBot;

					do {
						if (directions[NORTH]) {        
							target.row = north;
						} else if (directions[SOUTH]) { 
							target.row = south;
						} else if (directions[WEST]) {  
							target.column = west;
						} else if (directions[EAST]) {  
							target.column = east;
						} else if (!directions[NORTH] && !directions[SOUTH] && 
						           !directions[WEST]  && !directions[EAST]  && 
								   !shipDestroyed) {
							// All cardinals are FALSE but has not sunk a ship 
							
							// reinitiliazes target to the original target value */
							target = targetOrigin;
							targetTemp = target;

							/*
							  Counter increments by one, when the loop cycles again and
							  all cardinals are still FALSE. This ensures that we are checking
							  one cell over the adjacent cells 
							 */
							north = target.row - counter;
							targetTemp.row = north;

							// checks if the value NORTH of the target is a hit or miss 
							if (checkShot (gameBoard1, targetTemp) != -1 && north >= 0) {
								directions[NORTH] = TRUE;
							}

							targetTemp = target;
							south = target.row + counter;
							targetTemp.row = south;

							// checks if the value SOUTH of the target is a hit or miss 
							if (checkShot (gameBoard1, targetTemp) != -1 && south <= 9) {
								directions[SOUTH] = TRUE;
							}

							targetTemp = target;
							west = target.column - counter;
							targetTemp.column = west;

							// checks if the value WEST of the target is a hit or miss
							if (checkShot (gameBoard1, targetTemp) != -1 && west >= 0) {
								directions[WEST] = TRUE;
							}

							targetTemp = target;
							east = target.column + counter;
							targetTemp.column = east;

							// checks if the value EAST of the target is a hit or miss 
							if (checkShot (gameBoard1, targetTemp) != -1 && east <= 9) {
								directions[EAST] = TRUE;
							}

							/*
							  increments counter every cycle, serves as a addend to how
							  many cells to check from the target cell
							 */
							counter++;

						} else  {
							// when all conditions are not met, it reverts back to Hunt mode
							targetMode = FALSE;
							huntMode = TRUE;
							break;
						}
						
						// checks if the shot is a hit or miss
						shot = checkShot (gameBoard1, target);

					} while (shot == -1 && targetMode == TRUE);

					/*
					  this loop flips the values of the directions when it is not needed
					  takes the cardinal off the stack for directions to check 
					 */
					if (shot == 1 && huntMode == FALSE) {
						for (i = 0; i < 4; i++) {
							if (flipper == FALSE)
								directions[i] = FALSE;

							if (directions[i] == flipper) 
								flipper = FALSE;
						}
					} else {
						for (i = 0; i < 4; i++) {
							if (flipper == TRUE && directions[i] != FALSE) {
								directions[i] = FALSE;
								break;
							}
						}
					}

					// reinitializes the value of flipper
					flipper = TRUE;
				}

				/*
				  HUNT MODE 
				  This is starting mode of Player 2's turn. This pertains to the 
				  computer randomly selecting cells as targets and checking if they are
				  just plain water. When a cell equates to a Hit, then hunt mode is
				  set to false and target mode is set to true
				 */
				if (huntMode) {	

					// reinitializes important values
					counter = 1;
					flipper = TRUE;
					for (i = 0; i < 4; i++)
						directions[i] = TRUE;

					// loop that gets a random number of a cell as target 
					do {
						target.row = getRandomNumber (0, 9);
						target.column = getRandomNumber (0, 9);

						// checks target if it is a miss or hit cell
						shot = checkShot (gameBoard1, target);
					} while (shot == -1);

					// if it is a Hit, this saves the value of the original target */
					if (shot == 1) targetOrigin = target;
				}

				/*
				  shot values (1 means craft is hit, 0 means missed and -1 means 
				  it is already a hit or miss cell
				 */
				if (shot == 1) {

					/*
					  if all cardinals are false and no ship was sunk,
					  reintializes target from value of original target 
					 */
					if (!directions[NORTH] && !directions[SOUTH] && 
						!directions[WEST]  && !directions[EAST]  && 
						!shipDestroyed) { target = targetOrigin; }

					// hunt mode is false during a target mode
					huntMode = FALSE;
					targetMode = TRUE;
					targetBot = target;

					/*
					  get values of adjacent cells and ensuring that
					  that it is withing the bounds of gameboard for 
					  NORTH, SOUTH, WEST & EAST
					 */
					if (directions[NORTH] == TRUE) {  
						north = (targetBot.row - 1);
						checkBoundsDirections (directions, north, NORTH);
						targetTemp = target;
						targetTemp.row = north;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[NORTH] = FALSE;
					}
					
					if (directions[SOUTH] == TRUE) { 
						south = targetBot.row + 1;
						checkBoundsDirections (directions, south, SOUTH);
						targetTemp = target;
						targetTemp.row = south;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[SOUTH] = FALSE;
					}

					if (directions[WEST] == TRUE) {  
						west  = targetBot.column - 1;
						checkBoundsDirections (directions, west, WEST);
						targetTemp = target;
						targetTemp.column = west;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[WEST] = FALSE;
					}

					if (directions[EAST] == TRUE) {  
						east  = targetBot.column + 1;
						checkBoundsDirections (directions, east, EAST);
						targetTemp = target;
						targetTemp.column = east;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[EAST] = FALSE;
					}
				}
				
				// saves the character of the targeted cell
				shipSymbol = gameBoard1[target.row][target.column];

                updateGameBoard(gameBoard1,target);
				fflush(stdin);
				printMessage ("Nacisnij ENTER, żeby kontynuować\n> ");
				system("clear");
                printf("\nPlansza gracza 1 po zaktualizowaniu: \n");
                printGameBoard(gameBoard1,FALSE);
                break;
            case HARD:
				printf("\n> Plansza gracza numer jeden: \n");
                printGameBoard(gameBoard1,FALSE);
                printf("> Tura komputera \n");

                /*
				  check from a previous cycle if a ship destroyed,
				  if yes, bot back randomly selecting a target
				 */
				if (shipDestroyed) {
					shipDestroyed = FALSE;
					targetMode = FALSE;
					huntMode = TRUE;
				}

                /*
				  TARGET MODE 
				  This mode is true when the computer randomly selects a target,
				  and it happens to be a hit, this mode is set to true.
				 */
				if (targetMode) {
					// retains value of targetBot
					target = targetBot;

					do {
						if (directions[NORTH]) {        
							target.row = north;
						} else if (directions[SOUTH]) { 
							target.row = south;
						} else if (directions[WEST]) {  
							target.column = west;
						} else if (directions[EAST]) {  
							target.column = east;
						} else if (!directions[NORTH] && !directions[SOUTH] && 
						           !directions[WEST]  && !directions[EAST]  && 
								   !shipDestroyed) {
							// All cardinals are FALSE but has not sunk a ship 
							
							// reinitiliazes target to the original target value */
							target = targetOrigin;
							targetTemp = target;

							/*
							  Counter increments by one, when the loop cycles again and
							  all cardinals are still FALSE. This ensures that we are checking
							  one cell over the adjacent cells 
							 */
							north = target.row - counter;
							targetTemp.row = north;

							// checks if the value NORTH of the target is a hit or miss 
							if (checkShot (gameBoard1, targetTemp) != -1 && north >= 0) {
								directions[NORTH] = TRUE;
							}

							targetTemp = target;
							south = target.row + counter;
							targetTemp.row = south;

							// checks if the value SOUTH of the target is a hit or miss 
							if (checkShot (gameBoard1, targetTemp) != -1 && south <= 9) {
								directions[SOUTH] = TRUE;
							}

							targetTemp = target;
							west = target.column - counter;
							targetTemp.column = west;

							// checks if the value WEST of the target is a hit or miss
							if (checkShot (gameBoard1, targetTemp) != -1 && west >= 0) {
								directions[WEST] = TRUE;
							}

							targetTemp = target;
							east = target.column + counter;
							targetTemp.column = east;

							// checks if the value EAST of the target is a hit or miss 
							if (checkShot (gameBoard1, targetTemp) != -1 && east <= 9) {
								directions[EAST] = TRUE;
							}

							
							counter++;

						} else  {
							// when all conditions are not met, it reverts back to Hunt mode
							targetMode = FALSE;
							huntMode = TRUE;
							break;
						}
						
						// checks if the shot is a hit or miss
						shot = checkShot (gameBoard1, target);

					} while (shot == -1 && targetMode == TRUE);

					/*
					  this loop flips the values of the directions when it is not needed
					  takes the cardinal off the stack for directions to check 
					 */
					if (shot == 1 && huntMode == FALSE) {
						for (i = 0; i < 4; i++) {
							if (flipper == FALSE)
								directions[i] = FALSE;

							if (directions[i] == flipper) 
								flipper = FALSE;
						}
					} else {
						for (i = 0; i < 4; i++) {
							if (flipper == TRUE && directions[i] != FALSE) {
								directions[i] = FALSE;
								break;
							}
						}
					}

					// reinitializes the value of flipper
					flipper = TRUE;
				}

				/*
				  HUNT MODE 
				  This is starting mode of Player 2's turn. This pertains to the 
				  computer randomly selecting cells as targets and checking if they are
				  just plain water. When a cell equates to a Hit, then hunt mode is
				  set to false and target mode is set to true
				 */
				if (huntMode) {	
					maxShot = 0;
					// reinitializes important values
					counter = 1;
					flipper = TRUE;
					for (i = 0; i < 4; i++)
						directions[i] = TRUE;

					
					for (int a = 0; a<10; a++){
						for (int b = 0; b<10; b++){
							if (dataBoard[a][b] > maxShot) {
								maxShot = dataBoard[a][b];
								firstTarget.row = a; firstTarget.column = b;
							}
						}
					}
					dataBoard[firstTarget.row][firstTarget.column] = 0;
					if (firstTarget.row -1 >= 0) dataBoard[firstTarget.row - 1][firstTarget.column] = 0;
					if (firstTarget.row +1 <= 9) dataBoard[firstTarget.row + 1][firstTarget.column] = 0;
					if (firstTarget.column - 1 >= 0) dataBoard[firstTarget.row][firstTarget.column-1] = 0;
					if (firstTarget.column + 1 <= 9) dataBoard[firstTarget.row][firstTarget.column+1] = 0;

					if (maxShot > 2){
						target.row = firstTarget.row;
						target.column = firstTarget.column;

						shot = checkShot (gameBoard1, target);
					}
					// loop that gets a random number of a cell as target 
					else{
						do {
							target.row = getRandomNumber (0, 9);
							target.column = getRandomNumber (0, 9);

							// checks target if it is a miss or hit cell
							shot = checkShot (gameBoard1, target);
						} while (shot == -1);
					}

					// if it is a Hit, this saves the value of the original target */
					if (shot == 1) targetOrigin = target;
				}

				/*
				  shot values (1 means craft is hit, 0 means missed and -1 means 
				  it is already a hit or miss cell
				 */
				if (shot == 1) {

					/*
					  if all cardinals are false and no ship was sunk,
					  reintializes target from value of original target 
					 */
					if (!directions[NORTH] && !directions[SOUTH] && 
						!directions[WEST]  && !directions[EAST]  && 
						!shipDestroyed) { target = targetOrigin; }

					// hunt mode is false during a target mode
					huntMode = FALSE;
					targetMode = TRUE;
					targetBot = target;

					/*
					  get values of adjacent cells and ensuring that
					  that it is withing the bounds of gameboard for 
					  NORTH, SOUTH, WEST & EAST
					 */
					if (directions[NORTH] == TRUE) {  
						north = (targetBot.row - 1);
						checkBoundsDirections (directions, north, NORTH);
						targetTemp = target;
						targetTemp.row = north;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[NORTH] = FALSE;
					}
					
					if (directions[SOUTH] == TRUE) { 
						south = targetBot.row + 1;
						checkBoundsDirections (directions, south, SOUTH);
						targetTemp = target;
						targetTemp.row = south;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[SOUTH] = FALSE;
					}

					if (directions[WEST] == TRUE) {  
						west  = targetBot.column - 1;
						checkBoundsDirections (directions, west, WEST);
						targetTemp = target;
						targetTemp.column = west;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[WEST] = FALSE;
					}

					if (directions[EAST] == TRUE) {  
						east  = targetBot.column + 1;
						checkBoundsDirections (directions, east, EAST);
						targetTemp = target;
						targetTemp.column = east;
						if (checkShot (gameBoard1, targetTemp) == -1)
							directions[EAST] = FALSE;
					}
				}
				
				// saves the character of the targeted cell
				shipSymbol = gameBoard1[target.row][target.column];

                updateGameBoard(gameBoard1,target);
				fflush(stdin);
				printMessage ("Nacisnij ENTER, żeby kontynuować\n> ");
				system("clear");
                printf("\nPlansza gracza 1 po zaktualizowaniu: \n");
                printGameBoard(gameBoard1,FALSE);
                break;
            }
        }
        if (shot == 1){ //hit
            printf("> Trafiono w statek!\n");

            players[player-1].numHits++;

			// Checks if the ship destroyed
			if (player == 2)  
				shipDestroyed = checkDestructionShip (destroyedShip, PLAYER_ONE-1, shipSymbol, outStream);
			else
				checkDestructionShip  (destroyedShip, PLAYER_TWO-1, shipSymbol, outStream);

        }else {        // miss
                printf ("> Chybiono!\n");

                players[player-1].numMisses++;
            }

        if (isWinner(players,player)){
            printf ("\n> Gracz %d wygrał!\n", player);

                break;
        }

		if (player==2){
			if (mode) {
				fflush(stdin);
				printMessage ("Nacisnij ENTER, żeby kontynuować\n> ");
				system("clear");
			}
			else{
				fflush(stdin);
				printMessage ("Nacisnij ENTER, żeby kontynuować\n> ");
				fgets(x,2,stdin);
				system("clear");
			}
		}
		else{
			fflush(stdin);
			printMessage ("Nacisnij ENTER, żeby kontynuować\n> ");
			fgets(x,2,stdin);
			system("clear");
		}

        

        //change player
        if (player == 1) player = 2;
        else player = 1;
        //system ("clear");
    }

	players[0].totalShots = players[0].numHits + players[0].numMisses;
	players[1].totalShots = players[1].numHits + players[1].numMisses;

    printf("\n==============================\n");
	printf("             STATS            \n");
	printf("==============================\n");
	printf(" PLAYER 1:  %d hits           \n",players[0].numHits);
	printf("	    %d misses         \n",players[0].numMisses);
	printf("	    %d total shots    \n\n",players[0].totalShots);
	printf(" PLAYER 2:  %d hits           \n",players[1].numHits);
	printf("	    %d misses         \n",players[1].numMisses);
	printf("	    %d total shots    \n",players[1].totalShots);



	fclose (outStream);

}