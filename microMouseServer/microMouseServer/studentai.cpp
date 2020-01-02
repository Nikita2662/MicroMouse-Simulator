#include "micromouseserver.h"
#include "mazeConst.h"
#include "qstack.h"

// debugging begins
#include <sstream>
#include <string.h>
using namespace std;
char const *temp="test";
// debugging ends

QPoint mousepos;

struct storeStruct {
    int x, y;
    char d1, d2, d3='O';
};

vector<storeStruct> storeVec;


bool stackflag=0;
int   colcnt, i, j=0;
int rowcnt=MAZE_HEIGHT-1;

char directionflag='N';


void microMouseServer::studentAI() {

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

    mousepos.setX(colcnt);
    mousepos.setY(rowcnt);

    if (rowcnt<20 && colcnt<20) {

        int s = storeVec.size();
        if (s>0){
            for (int z=0;z<s;z++)
            {
                // Accessing structure members using their names.
                if (mousepos.x()==storeVec[z].x && mousepos.y()==storeVec[z].y && !(storeVec[z].d1=='O')) {
                    // If the cell is revisited
                    stackflag=1;
                    if (storeVec[z].d1=='L' || storeVec[z].d1=='R'){
                        if (isWallLeft()) {
                            turnRight();
                            if (storeVec[z].d2=='L'){
                                storeVec.push_back({mousepos.x(),mousepos.y(),'L','O','O'});
                            }

                            else {
                                storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O'});
                            }

                            if (directionflag=='N') {
                                directionflag='E';
                            }

                            else if (directionflag=='S') {
                                directionflag='W';
                            }

                            else if (directionflag=='E') {
                                directionflag='S';
                            }

                            else if (directionflag=='W') {
                                directionflag='N';
                            }
                        }
                    }
                }

                else {
                    stackflag=0;
                }
            }
        }

        if (stackflag==0) {
            // first time the cell is visited

            if (isWallLeft() && isWallRight()&& isWallForward()){
                // Dead end - FRL closed
                turnLeft();
                turnLeft();
                if (directionflag=='N') {
                    directionflag='S';
                }

                else if (directionflag=='S') {
                    directionflag='N';
                }

                else if (directionflag=='E') {
                    directionflag='W';
                }

                else if (directionflag=='W') {
                    directionflag='E';
                }
            }

            else if (!isWallLeft() && !isWallRight()&& !isWallForward()){
                // FRL open, continue forward without changing the direction
                storeVec.push_back({mousepos.x(),mousepos.y(),'R','L','O'});
                printUI("FRL");

            }

            else if (isWallLeft() && isWallRight()&& !isWallForward()){
                // F only open, continue forward without changing the direction

            }

            else if (isWallLeft() && !isWallRight()&& isWallForward()){
                // R only open

                turnRight();
                if (directionflag=='N') {
                    directionflag='E';
                }

                else if (directionflag=='S') {
                    directionflag='W';
                }

                else if (directionflag=='E') {
                    directionflag='S';
                }

                else if (directionflag=='W') {
                    directionflag='N';
                }

            }

            else if (!isWallLeft() && isWallRight()&& isWallForward()){
                // L only open

                turnLeft();
                if (directionflag=='N') {
                    directionflag='W';
                }

                else if (directionflag=='S') {
                    directionflag='E';
                }

                else if (directionflag=='E') {
                    directionflag='N';
                }

                else if (directionflag=='W') {
                    directionflag='S';
                }
            }
            else if (!isWallLeft() && !isWallRight()&& isWallForward()){
                // RL  open
                storeVec.push_back({mousepos.x(),mousepos.y(),'L','O','O'});
                printUI("RL");
                turnRight();
                if (directionflag=='N') {
                    directionflag='E';
                }

                else if (directionflag=='S') {
                    directionflag='W';
                }

                else if (directionflag=='E') {
                    directionflag='S';
                }

                else if (directionflag=='W') {
                    directionflag='N';
                }

            }

            else if (!isWallLeft() && isWallRight()&& !isWallForward()){
                // FL  open, continue forward without changing the direction
                storeVec.push_back({mousepos.x(),mousepos.y(),'L','O','O'});
                printUI("FL");

            }

            else if (isWallLeft() && !isWallRight()&& !isWallForward()){
                // FR open, continue forward without changing the direction
                storeVec.push_back({mousepos.x(),mousepos.y(),'R','O','O'});
                printUI("FR");
            }
        }
        moveForward();

        // Check the direction and adjust the row and column
        if (directionflag=='N'){
            rowcnt--;
        }

        else if (directionflag=='S'){
            rowcnt++;
        }

        else if (directionflag=='E'){
            colcnt++;
        }

        else if (directionflag=='W'){
            colcnt--;
        }
    }
}


