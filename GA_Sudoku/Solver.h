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
//static const float			kMutationRate	= 0.12f;
static const float			kMutationRate	= 1;

using namespace std;
#pragma mark -
#pragma mark - TEAM_FUNCTIONS

#define DEBDEBUG 1


//TEAM SCORE: 'Print your kid's face'
//Average Duration: 6787.58 milliseconds
//Average Generation Count: 315.64


//TEAM SCORE: 'Print your kid's face'
//Average Duration: 7304.88 milliseconds
//Average Generation Count: 297.5

//TEAM SCORE: 'Print your kid's face'
//Average Duration: 1980.42 milliseconds
//Average Generation Count: 97.78

static float getRowScore(const int* iBoard, int index);
static float getColScore(const int* iBoard, int index);
static float getBoxScore(const int* iBoard, int index);
static int getCorrectNum(const int* iBoard);
static void printBoard(const int* iBoard, const size_t& iTileCount);
static void switchRow(const int* iBoard,  int* oBoard, int index);
static void switchCol(const int* iBoard,  int* oBoard, int index);
static void switchBox(const int* iBoard,  int* oBoard, int index);
static void randomBoard(int* ioBoard, const size_t& iTileCount);
static void mutateCol(int* iBoard, int index);
static void mutateRow(int* iBoard, int index);
static void mutateBox(int* iBoard, int index);
static void mutateCol2(int* iBoard, int index);
static void mutateRow2(int* iBoard, int index);
static void mutateBox2(int* iBoard, int index);
static void shuffleCol(int* iBoard, int index);
static void shuffleRow(int* iBoard, int index);
static void shuffleBox(int* iBoard, int index);

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

static float getRowScore(const int* iBoard, int index)
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


static void switchRow(const int* iBoard,  int* oBoard, int index)
{
    memcpy(oBoard+index*9, iBoard+index*9, 9*sizeof(int));
}



static float getColScore(const int* iBoard, int index)
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

static float getBoxScore(const int* iBoard, int index)
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
//    int start = ((index)%3)*3 + index/3*27;
//    for (int x=0; x<3; x++)
//    {
//        for (int y=0; y<3; y++)
//        {
//            int i = start + x + y*9;
//            if (checkArr[iBoard[i]] == true) {
//                points *= reduceConst;
//            }
//            else {
//                checkArr[iBoard[i]] = true;
//            }
//        }
//    }
    
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

static void shuffleBox( int* iBoard, int index ){
    vector<int> tmp;
    tmp.resize(9);
    int start =((index)%3)*3 + 27*((index)/3);
    
    int j = 0;
    for (int x=0; x<3; x++)
    {
        for (int y=0; y<3; y++)
        {
            int i = start + x + y*9;
            tmp[j] = iBoard[i];
            j++;
        }
    }
    
    std::random_shuffle(tmp.begin(), tmp.end());
    
    j = 0;
    for (int x=0; x<3; x++)
    {
        for (int y=0; y<3; y++)
        {
            int i = start + x + y*9;
            iBoard[i] = tmp[j];
            j++;
        }
    }
}

static void mutateBox(int* iBoard, int index){
    
    int start =((index)%3)*3 + 27*((index)/3);
    int* tmp = iBoard;
    
    for (int x=0; x<3; x++)
    {
        for (int y=0; y<3; y++)
        {
            int i = start + x + y*9;
           
            for (int p=0; p<3; p++)
            {
                for (int q=0; q<3; q++)
                {
                    int j = start + p + q*9;
                    if(i != j){
                        if(tmp[i] == iBoard[j]){
                            if( ( (float)rand() / (float)RAND_MAX ) < kMutationRate ) {
                            iBoard[i] = randomInt(getTileValueMin(), getTileValueMax()+1);
                            }
                        }
                    }
                }
            }
            
            
        }
    }
    
}

static void shuffleRow( int* iBoard, int index ){
    vector<int> tmp;
    tmp.resize(9);
    int start = index*9;
    
    int j = 0;
    for (int i=start; i<start+9; i++)
    {
            tmp[j] = iBoard[i];
        j++;
    }
    
    std::random_shuffle(tmp.begin(), tmp.end());
    j = 0;
    for (int i=start; i<start+9; i++)
    {
        iBoard[i] = tmp[j];
        j++;
    }
    
}

static void mutateRow(int* iBoard, int index)
{
    int* tmp = iBoard;
    int start = index*9;

    for (int i=start; i<start+9; i++)
    {
        for (int j=start; j<start+9; j++)
        {
            if(i != j){
                if(tmp[i] == iBoard[j]){
                    if( ( (float)rand() / (float)RAND_MAX ) < kMutationRate ) {
                    iBoard[i] = randomInt(getTileValueMin(), getTileValueMax()+1);
                    }
                }
            }
        }
    }

}

static void shuffleCol( int* iBoard, int index ){
    vector<int> tmp;
    tmp.resize(9);
    int start = index*9;
    
    int j = 0;
    for (int i=start; i<start+9; i++)
    {
        tmp[j] = iBoard[i];
        j++;
    }
    
    std::random_shuffle(tmp.begin(), tmp.end());
    
    j = 0;
    for (int i=start; i<start+9; i++)
    {
        iBoard[i] = tmp[j];
        j++;
    }
}

static void mutateCol(int* iBoard, int index){
    
    int* tmp = iBoard;
    
    int start = index;
    
    for (int i=start; i<start+81; i+=9)
    {
        for (int j=start; j<start+81; j+=9)
        {
            if(i != j){
                if(tmp[i] == iBoard[j]){
                    if( ( (float)rand() / (float)RAND_MAX ) < kMutationRate ) {
                    iBoard[i] = randomInt(getTileValueMin(), getTileValueMax()+1);
                    }
                }
            }
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

static void mutateRow2(int* ioBoard, int index)
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

static void mutateCol2(int* ioBoard, int index)
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

static void mutateBox2(int* ioBoard, int index)
{
    bool checkArr[10] = {false};
    
    int start = ((index)%3)*3 + 27*((index)/3);
    
    for (int x=0; x<3; x++)
    {
        for (int y=0; y<3; y++)
        {
            int i = start + x + y*9;
            if (checkArr[ioBoard[i]] == true) {
                ioBoard[i] = randomInt(getTileValueMin(), getTileValueMax()+1);
            }
            else {
                checkArr[ioBoard[i]] = true;
            }
        }
    }
}



