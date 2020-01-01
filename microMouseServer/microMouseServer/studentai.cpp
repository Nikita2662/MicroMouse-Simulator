#include "micromouseserver.h"
#include "mazeConst.h"

#include <sstream>
#include <string.h>
using namespace std;
char const *temp="test";

// Above 4 lines are for debugging purpose only - can be commented

bool reverseflag=0;
int cnt, pathcnt,  colcnt, i, j=0;
int rowcnt=MAZE_HEIGHT-1;
int learnCell[MAZE_HEIGHT][MAZE_WIDTH];
char directionflag='N';

void microMouseServer::studentAI()
{

    /*
 * The following are the eight functions that you can call. Feel free to create your own fuctions as well.
 * Remember that any solution that calls moveForward more than once per call of studentAI() will have points deducted.
 *
 *The following functions return if there is a wall in their respective directions
 *bool isWallLeft();
 *bool isWallRight();
 *bool isWallForward();
 *
 *The following functions move the mouse. Move forward returns if the mouse was able to move forward and can be used for error checking
 *bool moveForward();
 *void turnLeft();
 *void turnRight();
 *
 * The following functions are called when you need to output something to the UI or when you have finished the maze
 * void foundFinish();
 * void printUI(const char *mesg);
*/
    // Intialize the array to store the values of each individual cell
    for (i=0;i<MAZE_HEIGHT;i++) {
        for (j=0;j<MAZE_WIDTH;j++){
            learnCell[i][j]=0;
        }
    }

    if (reverseflag==0 && learnCell[rowcnt][colcnt]==0) {
        if (isWallLeft() && isWallRight()&& isWallForward()){
            // Dead end - FRL closed
        }

        else if (!isWallLeft() && !isWallRight() && !isWallForward()){
            // FRL open
        }

        else if (isWallLeft() && isWallRight() && !isWallForward()){
            // F only open
            moveForward();

            // debugging begins
            stringstream strs;
            strs << cnt;
            string temp_str = strs.str();
            char* char_type = (char*) temp_str.c_str();
            temp=char_type;
            printUI(temp);
            cnt++;
            // end of debugging
        }

        else if (isWallLeft() && !isWallRight()&& isWallForward()){
            // R only open
        }

        else if (!isWallLeft() && isWallRight()&& isWallForward()){
            // L only open
        }

        else if (!isWallLeft() && !isWallRight()&& isWallForward()){
            // RL  open
        }

        else if (!isWallLeft() && isWallRight()&& !isWallForward()){
            // FL  open
        }

        else if (isWallLeft() && !isWallRight()&& !isWallForward()){
            // FR open
        }
    }

    //turnRight();
    //moveForward();
}


