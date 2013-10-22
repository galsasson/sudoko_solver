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

#pragma mark -
#pragma mark - TEAM_PARAMS

static const std::string	kAuthorTeam		= "Print your kid's face";
static const float			kMutationRate	= 0.12f;

using namespace std;
#pragma mark -
#pragma mark - TEAM_FUNCTIONS

//#define DEBDEBUG 1

static float getRowScore(const int* iBoard, int index);
static float getColScore(const int* iBoard, int index);
static float getBoxScore(const int* iBoard, int index);
static int getCorrectNum(const int* iBoard);
static void printBoard(const int* iBoard, const size_t& iTileCount);
static inline void switchRow(const int* iBoard,  int* oBoard, int index);
static void switchCol(const int* iBoard,  int* oBoard, int index);
static void switchBox(const int* iBoard,  int* oBoard, int index);
static void randomBoard(int* ioBoard, const size_t& iTileCount);
static void mutateCol2(int* iBoard, int index);
static void mutateRow2(int* iBoard, int index);
static void mutateBox2(int* iBoard, int index);

static long long callCounter=0;
static float pointsAvg = 0;
static float correctAvg = 0;

static float reduceConst = 0.75f;
static int generation = 0;


static float fitnessFunc(const int* iBoard, const size_t& iTileCount)
{
	// EXERCISE: Please implement a function that evaluates the fitness of a given sudoku board...
    
    // GAL: max points should be 1
    float points = 1;
    
    for (int i=0; i<9; i++)
    {
        points *= getBoxScore(iBoard, i);
        points *= getRowScore(iBoard, i);
        points *= getColScore(iBoard, i);
    }
  
    if (callCounter%1000 == 0) {
        generation++;
    }

#ifdef DEBDEBUG
    int correctNum = getCorrectNum(iBoard);
    pointsAvg += points/1000;
    correctAvg += (float)correctNum/1000;
    if (callCounter%50000 == 0) {
        //printBoard(iBoard, iTileCount);
        cout<<"generation["<<generation<<"] (points avg="<<pointsAvg<<", correct avg="<<correctAvg<<")"<<endl;
    }
    
    if (callCounter%1000 == 0) {
        pointsAvg = 0;
        correctAvg = 0;
    }
#endif
    
    callCounter++;
    
    // points == 1 means the board is complete
    // so we can reset the callCounter
    // and genration counter for the next run (not very accurate)
    if (points==1) {
        callCounter = 0;
        generation = 0;
    }
    
    return points;
}

static void crossoverFunc(const int* iBoardA, const int* iBoardB, int* oBoard, const size_t& iTileCount)
{
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...

    for(int i=0;i<9;i++){
        if(getRowScore(iBoardA, i) > getRowScore(iBoardB, i))
            switchRow(iBoardA, oBoard, i);
        else switchRow(iBoardB, oBoard, i);
    }
}

static void mutateFunc(int* ioBoard, const size_t& iTileCount, const float& iMutationRate)
{
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
    if (generation % 30 == 0) {
        randomBoard(ioBoard, iTileCount);
        return;
    }
    else {
        for(int i=0;i<9;i++)
        {
            if(getBoxScore(ioBoard, i) < 1.0f) mutateBox2(ioBoard, i);
            if(getRowScore(ioBoard, i) < 1.0f) mutateRow2(ioBoard, i);
            if(getColScore(ioBoard, i) < 1.0f) mutateCol2(ioBoard, i);
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

static inline float getRowScore(const int* iBoard, int index)
{
    float points = 1;
    bool checkArr[10] = {false};
    
    int *i = (int*)iBoard+index*9;
    int *end = i+9;
    
    while (i<end) {
        bool *pSlot = checkArr + (*i);
        if (*pSlot) {
            points *= reduceConst;
        }
        else {
            *pSlot = true;
        }
        i++;
    }
    
    return points;
}


static inline void switchRow(const int* iBoard,  int* oBoard, int index)
{
    // copy row index from iBoard to oBoard.
    memcpy(oBoard+index*9, iBoard+index*9, 36);
}



static inline float getColScore(const int* iBoard, int index)
{
    float points = 1;
    bool checkArr[10] = {false};
    
    int *i = (int*)iBoard + index;
    int *end = (int*)iBoard+81;

    while (i<end) {
        bool *pSlot = checkArr + *i;
        if (*pSlot) {
            points *= reduceConst;
        }
        else {
            *pSlot = true;
        }
        i+=9;
    }
    
    return points;
}


static void switchCol(const int* iBoard,  int* oBoard, int index)
{
    
    int start = index;
    
    for (int i=start; i<start+81; i+=9)
    {

        oBoard[start] = iBoard[start];
        
    }
    
}

static inline float getBoxScore(const int* iBoard, int index)
{
    float points = 1;
    bool checkArr[10] = {false};
    
    int* i = (int*)iBoard + ((index)%3)*3 + index/3*27;
    int* bottom = i + 27;
    
    while (i<bottom)
    {
        int* right = i+3;
        while (i<right) {
            bool *pSlot = checkArr + *i;
            if (*pSlot) {
                points *= reduceConst;
            }
            else {
                *pSlot = true;
            }
            i++;
        }
        i+=6;
    }
    
    return points;
}

static void switchBox(const int* iBoard, int* oBoard, int index)
{
    
    int start =((index)%3)*3 + 27*((index)/3);
    
    for (int x=0; x<3; x++)
    {
        for (int y=0; y<3; y++)
        {
            int i = start + x + y*9;
            oBoard[i] = iBoard[i];
            
       }
    }
    
}

static int getCorrectNum(const int* iBoard)
{
    int c=0;
    
    for (int i=0; i<9; i++)
    {
        if (getRowScore(iBoard, i) == 1) {
            c++;
        }
        if (getColScore(iBoard, i) == 1) {
            c++;
        }
        if (getBoxScore(iBoard, i) == 1) {
            c++;
        }
    }
    
    return c;
}

static inline void mutateRow2(int* ioBoard, int index)
{
    bool checkArr[10] = {false};
    
    int *i = (int*)ioBoard+index*9;
    int *end = i+9;
    
    while (i<end) {
        bool *pSlot = checkArr + (*i);
        if ((*pSlot)) {
            *i = randomInt(getTileValueMin(), getTileValueMax()+1);
        }
        else {
            *pSlot = true;
        }
        i++;
    }
}

static inline void mutateCol2(int* ioBoard, int index)
{
    bool checkArr[10] = {false};
    
    int *i = (int*)ioBoard + index;
    int *end = (int*)ioBoard+81;
    
    while (i<end) {
        bool *pSlot = checkArr + *i;
        if ((*pSlot)) {
            *i = randomInt(getTileValueMin(), getTileValueMax()+1);
        }
        else {
            *pSlot = true;
        }
        i+=9;
    }
}

static inline void mutateBox2(int* ioBoard, int index)
{
    bool checkArr[10] = {false};
    
    int* i = (int*)ioBoard + ((index)%3)*3 + index/3*27;
    int* bottom = i + 27;
    
    while (i<bottom)
    {
        int* right = i+3;
        while (i<right) {
            bool *pSlot = checkArr + *i;
            if (*pSlot) {
                *i = randomInt(getTileValueMin(), getTileValueMax()+1);
            }
            else {
                *pSlot = true;
            }
            i++;
        }
        i+=6;
    }
}



