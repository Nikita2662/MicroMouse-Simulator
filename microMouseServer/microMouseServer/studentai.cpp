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
    char pn, pe, pw, ps; // possible values: O for Open, C for closed, D for Dead, A not set
    char direntns, direntew; // possible values: N/S and E/W for entered directions, A not set
};

vector<storeStruct> storeVec;

bool reentryflag=0;
int   colcnt, i, j=0;
int rowcnt=MAZE_HEIGHT-1;

char directionflag='N';
char finishflag='N';


void microMouseServer::studentAI() { // studentai logic begins

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

    if (rowcnt<20 && colcnt<20 && rowcnt>=0 && colcnt>=0) { // Check to ensure it is not out of the area
        int s = storeVec.size();
        if (s>0){ // Check the stored items for this position
            for (int z=0;z<s;z++) {
                if (mousepos.x()==storeVec[z].x && mousepos.y()==storeVec[z].y) {
                    // If the cell is revisited
                    reentryflag=1;

                    if(directionflag=='N') { // Heading north
                        //if (storeVec[z].direntns=='N') { // stored value North
                        if (storeVec[z].ps=='D') {
                            storeVec[z].pe='D';
                        }

                        else {
                            storeVec[z].ps='D';
                        }

                        if (storeVec[z].pe=='O' && !isWallRight()) {
                            turnRight();
                            directionflag='E';
                        }

                        else if (storeVec[z].pw=='O' && !isWallLeft()) {
                            turnLeft();
                            directionflag='W';
                            storeVec[z].pw='D';
                        }
                    } // Stored value North ends

                    else if(directionflag=='S') { // Heading south

                        //if (storeVec[z].direntns=='S') { // stored value South
                        if (storeVec[z].pn=='D') {
                            storeVec[z].pe='D';
                        }

                        else {
                            storeVec[z].pn='D';
                        }

                        if (storeVec[z].pe=='O' && !isWallLeft()) {
                            turnLeft();
                            directionflag='E';
                        }

                        else if (storeVec[z].pw=='O' && !isWallRight()) {
                            turnRight();
                            directionflag='W';
                            storeVec[z].pw='D';
                        }
                    } // Stored value South ends

                    else if(directionflag=='E') { // Heading east
                        //if (storeVec[z].direntew=='E') { // stored value East
                        if (storeVec[z].pw=='D') {
                            storeVec[z].pn='D';
                        }

                        else {
                            storeVec[z].pw='D';}

                        if (storeVec[z].pn=='O' && !isWallLeft()) {
                            turnLeft();
                            directionflag='N';
                        }

                        else if (storeVec[z].ps=='O' && !isWallRight()) {
                            turnRight();
                            directionflag='S';
                            storeVec[z].ps='D';
                        }

                        stringstream strs;
                        //strs << storeVec[z].pe;
                        strs << mousepos.y();
                        string temp_str = strs.str();
                        char* char_type = (char*) temp_str.c_str();
                        temp=char_type;
                        printUI(temp);
                        //printUI(mousepos.manhattanLength());
                    } // Stored value East ends
                    else if(directionflag=='W') { // Heading wast
                        //if (storeVec[z].direntew=='W') { // stored value west
                        if (storeVec[z].pe=='D') {
                            storeVec[z].pn='D';
                        }

                        else {
                            storeVec[z].pe='D';}

                        if (storeVec[z].pn=='O' && !isWallRight()) {
                            turnRight();
                            directionflag='N';
                        }

                        else if (storeVec[z].ps=='O' && !isWallLeft()) {
                            turnLeft();
                            directionflag='S';
                            storeVec[z].ps='D';
                        }
                    } // Stored value west ends
                } // revisited cell  ends
                else {reentryflag=0;}
            }
        } // stored items check ends

        if (reentryflag==0) { // First time this cell is visited

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
                // FRL open, set the vector
                if (directionflag=='N') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','O','S','A'});
                }

                else if (directionflag=='S') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','O','N','A'});
                }

                else if (directionflag=='E') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','O','A','W'});
                }

                else if (directionflag=='W') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','O','A','E'});
                }
            }

            else if (isWallLeft() && isWallRight()&& !isWallForward()){
                // F only open, do nothing
            }

            else if (isWallLeft() && !isWallRight()&& isWallForward()){
                // R only open, set the direction
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
                // L only open, set the vector
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
                // RL  open, set the vector --- changed the directions - debug
                turnRight();
                if (directionflag=='N') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'C','O','O','O','S','A'});
                    directionflag='E';
                }

                else if (directionflag=='S') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','C','N','A'});
                    directionflag='W';
                }

                else if (directionflag=='E') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','C','O','O','A','W'});
                    directionflag='S';
                }

                else if (directionflag=='W') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','C','O','A','E'});
                    directionflag='N';
                }
            }

            else if (!isWallLeft() && isWallRight()&& !isWallForward()){
                // FL  open, set the vector
                if (directionflag=='N') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','C','O','O','S','A'});
                }

                else if (directionflag=='S') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','C','O','N','A'});
                }

                else if (directionflag=='E') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','C','A','W'});
                }

                else if (directionflag=='W') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'C','O','O','O','A','E'});
                }
            }

            else if (isWallLeft() && !isWallRight()&& !isWallForward()){
                // FR open, set the vector
                if (directionflag=='N') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','C','O','S','A'});
                }

                else if (directionflag=='S') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','C','O','O','N','A'});
                }

                else if (directionflag=='E') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'C','O','O','O','A','W'});
                }

                else if (directionflag=='W') {
                    storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O','C','A','E'});
                }
            }

        } // First time cell visited ends

        moveForward();
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

    } // Area check with row and column ends
} // studentai logic ends


