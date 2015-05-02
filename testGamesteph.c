//Steph's part of testGame.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game-1.07.h"

//Possible movements via path:
#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B' 

//KPI Value of each move:
#define CAMPUS 10
#define G08 10    //Only 10 points added to previous 10 pts of campus
#define ARC 2
#define SPINOFF 0 //Will have effect only if pub/ip.
#define PUB 0     //Only prestige, no actual points
#define IP 10
#define RETRAIN 0 //No KPI points will be earned
#define MOSTARC 10
#define MOSTPUB 10    

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, 
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, 
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, 
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, 
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS }

#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

#define KPIVALUES {PASS, CAMPUS, G08, ARC, 0, 0, IP, 0, MOSTARC, MOSTPUB}


void testGetARC (void);  //contents of given edge, is ARC?
void testIsLegalAction (void);
void testGetKPIpoints (void);
void testGetARCs (void); //returns no grants player has
void testGetGO8s (void);

int main (int argc, char *argv []) {
	testIsLegalAction();
	return EXIT_SUCCESS;
}

// the contents of the given edge (ie ARC code or vacent ARC)
//int getARC(Game g, path pathToEdge);
void testGetARC (void) {

	printf ("Testing getARC()...\n");

	Game g = newGame (DEFAULT_DISCIPLINES, DEFAULT_DICE);


}

void testIsLegalAction (void) {
	//Tests if func tests if action and retraining is legal, TRUE/FALSE

	printf ("Testing isLegalAction()...\n");

	int plyr = UNI_A;
	int rate = 0;
	int counter = 0;
	int noCampus = 0;
	int noGO8s = 0;
	int noStudents = 0;
	int stuTypes[6];  //Stores number of students of each type

	Game g = newGame (DEFAULT_DISCIPLINES, DEFAULT_DICE);
	action a;
	a.actionCode = 0;
	a.destination = {0};
	a.disciplineFrom = 0; 
	a.disciplineTo = 0;

	//Ensure that the game has already started
	assert (isLegalAction (g, a) == FALSE);
	throwDice (g, 3);
	assert (isLegalAction (g, a) == TRUE); 
	
	//Ensure action codes are legal
	a.actionCode = -1;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = -124;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = 'f';
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = 34;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = 8;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = '?';
	assert (isLegalAction (g, a) == FALSE);

	noCampus = getCampuses (g, plyr);
	while (counter < 6) {
		stuTypes[counter] = getStudents (g, plyr, counter);
		counter ++;
	}

	a.actionCode = PASS; //Always true when game has began
	assert (isLegalAction (g, a) == TRUE);

	a.actionCode = BUILD_CAMPUS; //Needs 1 of each but THD/M$
	if ((stuTypes[1] > 0) && (stuTypes[2] > 0) && (stuTypes[3] > 0) && (stuTypes[4] > 0)){
		if (getCampus (g, a.destination) == VACANT_VERTEX) {
			assert (isLegalAction (g, a) == TRUE);
		}
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}

	a.actionCode = BUILD_G08; //Needs a campus, 2MJs, 3M$ + one campus
	noGO8s = getG08s (g, plyr);
	if ((stuTypes[3] >= 1) && (stuTypes[5] > 2) && (noCampus > 0) && (noGO8s < 8)){
		assert (isLegalAction (g, a) == TRUE);
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}
	
	//Ensure paths are well formed & legal (legal direction & length)
	//Does not leave into the sea
	//Does it lead to vacant vertexr
	//Are they allowed to place a campus on vertex
	a.actionCode = OBTAIN_ARC;
	counter = 0; //to be reused in loop
	if ((stuTypes[1] >= 0) && (stuTypes[2] > 0)){
		if (a.destination[0] == BACK) {
			assert (isLegalAction (g, a) == FALSE);
		} else if (sizeof (a.destination) > PATH_LIMIT) {
			assert (isLegalAction (g, a) == FALSE);
		} else if (getARC (g, a.destination) == VACANT_ARC) {
			assert (isLegalAction (g, a) == TRUE);
		} else {
			assert (isLegalAction (g, a) == FALSE);
		}
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}

	a.actionCode = START_SPINOFF;
	if ((stuTypes[3] >= 0) && (stuTypes[4] > 0) && (stuTypes[5] > 0)){
		assert (isLegalAction (g, a) == TRUE);
	} else {
		assert (isLegalAction (g, a) == FALSE);
	}
	
	//Illegal for players to OBTAIN_PUBLICATION and OBTAIN_IP_PATENT
	a.actionCode = OBTAIN_PUBLICATION;
	assert (isLegalAction (g, a) == FALSE);
	a.actionCode = OBTAIN_IP_PATENT;
	assert (isLegalAction (g, a) == FALSE);

	//Testing for retraining action
	//Ensure valid discipline numbers used
	a.actionCode = RETRAIN_STUDENTS;
	a.disciplineTo = -124;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = -1;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = 'f';
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = 34;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = 6;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineTo = '?';
	assert (isLegalAction (g, a) == FALSE);

	a.disciplineFrom = -124;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineFrom = 'f';
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineFrom = 34;
	assert (isLegalAction (g, a) == FALSE);
	a.disciplineFrom = '?';
	assert (isLegalAction (g, a) == FALSE);

	//Testing to ensure uni has enough students for retraining
	//Testing THDs that cannot be retrained
	a.disciplineTo = STUDENT_THD;
	a.disciplineFrom = STUDENT_THD;
	assert (isLegalAction (g, a) == FALSE);

	a.disciplineTo = 1;   //To avoid THDs (0) that cannot be retrained
	a.disciplineFrom = 1;

	while (a.disciplineFrom < 6) {
		while (a.disciplineTo < 6) {
			rate = getExchangeRate (g, plyr, a.disciplineFrom, a.disciplineTo);
			noStudents = getStudents (g, plyr, a.disciplineFrom);
			if (rate == noStudents) {
				assert (isLegalAction (g, a) == TRUE);
			} else if (rate != noStudents) {
				assert (isLegalAction (g, a) == FALSE);
			}
			a.disciplineTo ++;
		}
		a.disciplineFrom ++;
	}

	disposeGame (g); //Restarts the game for other testing.

	printf ("...isLegalAction() passed all tests.\n");
}
 

void testGetKPIpoints (void) {
	//Test if function returns KPI points of specified player

	printf ("Testing getKPIpoints()...\n");

	Game g = newGame (DEFAULT_DISCIPLINES, DEFAULT_DICE);
	action a;
	int move = 0;
	int player = 0;
	int kpiPts[3] = {0};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getKPIpoints (g, player) == 0);
		player ++;
	}

	//Tests that players have correct KPI values for every move.
	while (move < 8) {
		player = getWhoseTurn (g);
		a.actionCode = move;
		makeAction (g, a);
		kpiPts[player] += KPIVALUES[move];
		assert (getKPIpoints (g, player) == kpiPts[player]);
	}

	//Tests prestige KPI Points for most ARCs & Publications
	player = getMostARCs (g);
	kpiPts[player] += KPIVALUES[MOSTARC];
	assert (getKPIpoints (g, player) == kpiPts[player]);

	player = getMostPublications (g);
	kpiPts[player] += KPIVALUES[MOSTPUB];
	assert (getKPIpoints (g, player) == kpiPts[player]);

	disposeGame(g);
	printf ("...All tests for getKPIpoints() passed.\n");
}

void testGetARCs (void) {
	//test if it returns number of ARC grants of specified player
	printf ("Testing getARCs()...\n");

	Game g = newGame (DEFAULT_DISCIPLINES, DEFAULT_DICE);
	action a;
	int player = 0;
	int numArcs[3] = {0};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getARCs (g, player) == 0);
		player ++;
	}

	//Tests that players will get ARC after making move
	player = getWhoseTurn (g);
	a.actionCode = OBTAIN_ARC;
	makeAction (g, a);
	numArcs[player] ++;
	assert (getARCs (g, player) == numArcs [player]);

	disposeGame(g);
	printf ("...All tests for getARCs() passed.\n");
}
 
void testGetGO8s (void) {
	//test if it returns number of G08 campuses of specified player
	printf ("Testing getG08s()...\n");

	Game g = newGame (DEFAULT_DISCIPLINES, DEFAULT_DICE);
	action a;
	int player = 0;
	int numG08[3] = {0};

	//Tests all new games start with 0, inlcuding NO_ONE
	while (player < 4) {
		assert (getG08s (g, player) == 0);
		player ++;
	}

	//Tests that players will get G08 after making move
	player = getWhoseTurn (g);
	a.actionCode = BUILD_G08;
	makeAction (g, a);
	numG08 [player] ++;
	assert (getG08s (g, player) == numG08 [player]);

	disposeGame(g);
	printf ("...All tests for getG08s() passed.\n");
}

