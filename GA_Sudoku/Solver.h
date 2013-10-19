//////////////////////////////////////////////////
/*         Evolution as a Creative Tool         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2013                   */
//////////////////////////////////////////////////

#pragma once

#include "Constants.h"
/**
 * THE CONTENTS OF THIS FILE SHOULD BE EDITED TO PRODUCE A WINNING SUDOKU SOLVER...
 */

// get to 20 with
// mut = 0.02, reduce = 0.5


#pragma mark -
#pragma mark - TEAM_PARAMS

static const std::string	kAuthorTeam		= "Print your kid's face";
static const float			kMutationRate	= 0.02f;

using namespace std;
#pragma mark -
#pragma mark - TEAM_FUNCTIONS

static float getRowScore(const int* iBoard, int index, int start, int end);
static float getColScore(const int* iBoard, int index, int start, int end);
static float getBoxScore(const int* iBoard, int index);
static int getCorrectNum(const int* iBoard);
static void printBoard(const int* iBoard, const size_t& iTileCount);

static long long printCounter=0;
static float pointsAvg = 0;
static float correctAvg = 0;

//static float reduceConst = 0.75f;
static float reduceConst = 0.65f;


static float fitnessFunc(const int* iBoard, const size_t& iTileCount)
{
	// EXERCISE: Please implement a function that evaluates the fitness of a given sudoku board...
    
    // GAL: max points should be 27
    float points = 1;
    
    for (int i=0; i<9; i++)
    {
        points *= getBoxScore(iBoard, i);
        points *= getRowScore(iBoard, i, 0, 9);
        points *= getColScore(iBoard, i, 0, 9);
    }
  
    int correctNum = getCorrectNum(iBoard);
    if (correctNum > 25) {
        cout<<"******** correctNum = "<<correctNum<<endl;
        cout<<"******** points = "<<points<<endl;
        printBoard(iBoard, iTileCount);
    }
//    points = points*points*points*points;
    pointsAvg += points/1000;
    correctAvg += (float)correctNum/1000;
    
    
    if (printCounter%100000 == 0) {
        cout<<"-------------------------------------------\n";
        printBoard(iBoard, iTileCount);
        cout<<"generation["<<printCounter/1000<<"] (points avg="<<pointsAvg<<", correct avg="<<correctAvg<<")"<<endl;
        cout<<"-------------------------------------------\n";
    }
    
    if (printCounter%1000 == 0) {
        pointsAvg=0;
        correctAvg=0;
    }

    printCounter++;
    
    return points;
}

static void crossoverFunc(const int* iBoardA, const int* iBoardB, int* oBoard, const size_t& iTileCount)
{
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
	
	int tMid = randomInt( 0, (int)iTileCount );
	for(size_t i = 0; i < iTileCount; i++) {
		if(i < tMid) { oBoard[i] = iBoardA[i]; }
		else         { oBoard[i] = iBoardB[i]; }
	}
}

static void mutateFunc(int* ioBoard, const size_t& iTileCount, const float& iMutationRate)
{
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
	
	for(int i = 0; i < iTileCount; i++) {
		if( ( (float)rand() / (float)RAND_MAX ) < iMutationRate ) {
			ioBoard[i] = randomInt( getTileValueMin(), getTileValueMax() + 1 );
		}
	}
}

static void randomBoard(int* ioBoard, const size_t& iTileCount)
{
	// EXERCISE: The contents of this function can be edited for testing, but will be reverted for league play...
	
	for(int i = 0; i < iTileCount; i++) {
		ioBoard[i] = randomInt( getTileValueMin(), getTileValueMax() + 1 );
	}
}

static void printBoard(const int* iBoard, const size_t& iTileCount)
{
	// EXERCISE: The contents of this function can be edited for testing, but will be reverted for league play...
	
	int tAxisLen = sqrt( iTileCount );
	printf( "%ix%i BOARD:\n", tAxisLen, tAxisLen );
	for(int i = 0; i < tAxisLen; i++) {
		for(int j = 0; j < tAxisLen; j++) {
			printf( "%i ", iBoard[ i * tAxisLen + j ] );
		}
		printf( "\n" );
	}
	printf( "\n" );
}

static float getRowScore(const int* iBoard, int index, int s, int e)
{
    float points = 1;
    bool checkArr[9] = {false};
    
    int start = index*9 + s;
    int end = start+(e-s);
    
    for (int i=start; i<end; i++)
    {
        if (checkArr[iBoard[i]-1] == true) {
            points *= reduceConst;
        }
        else {
            checkArr[iBoard[i]-1] = true;
        }
    }

    return points;
}

static float getColScore(const int* iBoard, int index, int s, int e)
{
    float points = 1;
    bool checkArr[9] = {false};
    
    int start = index + s*9;
    int end = start+(e-s)*9;
    
    for (int i=start; i<end; i+=9)
    {
        if (checkArr[iBoard[i]-1] == true) {
            points *= reduceConst;
        }
        else {
            checkArr[iBoard[i]-1] = true;
        }
    }
    
    return points;
}

static float getBoxScore(const int* iBoard, int index)
{
    float points = 1;
    bool checkArr[9] = {false};
    
    int start = index%3*3 + (index/3)*27;
    
    for (int x=0; x<3; x++)
    {
        for (int y=0; y<3; y++)
        {
            int i = start + x + y*9;
            if (checkArr[iBoard[i]-1] == true) {
                points *= reduceConst;
//                cout<<"error in index "<<i<<endl;
            }
            else {
                checkArr[iBoard[i]-1] = true;
            }
        }
    }

    return points;
}

static int getCorrectNum(const int* iBoard)
{
    int c=0;
    
    for (int i=0; i<9; i++)
    {
        if (getRowScore(iBoard, i, 0, 9) == 1) {
            c++;
        }
        if (getColScore(iBoard, i, 0, 9) == 1) {
            c++;
        }
        if (getBoxScore(iBoard, i) == 1) {
            c++;
        }
    }
    
    return c;
}
