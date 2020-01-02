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

struct storeStruct{
    int x, y;
    char d1, d2, d3='O';
};
vector<storeStruct> storeVec;

bool reverseflag=0;
int cnt, pathcnt,  colcnt, i, j=0;
int rowcnt=MAZE_HEIGHT-1;
int learnCell[MAZE_WIDTH][MAZE_HEIGHT];
char directionflag='N';


void microMouseServer::studentAI(){
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
    for (j=0;j<MAZE_HEIGHT;j++) {
        for (i=0;i<MAZE_WIDTH;i++){
            learnCell[i][j]=0;
        }
    }

    if (rowcnt<20 && colcnt<20) {
        if (reverseflag==0 ) {
            // first time the cell is visited
            mousepos.setX(colcnt);
            mousepos.setY(rowcnt);


            if (isWallLeft() && isWallRight()&& isWallForward()){
                // Dead end - FRL closed
                reverseflag=1;

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
                // FRL open, continue forward without changing direction
                storeVec.push_back({mousepos.x(),mousepos.y(),'R','L','O'});
                printUI("FRL");
            }

            else if (isWallLeft() && isWallRight()&& !isWallForward()){
                // F only open, continue forward without changing direction
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
                // FL  open, continue forward without changing direction
                storeVec.push_back({mousepos.x(),mousepos.y(),'L','O','O'});
                printUI("FL");

            }

            else if (isWallLeft() && !isWallRight()&& !isWallForward()){
                // FR open, continue forward without changing direction
                storeVec.push_back({mousepos.x(),mousepos.y(),'R','O','O'});
                printUI("FR");
                int s = storeVec.size();
                for (int z=0;z<s;z++)                {
                    // Accessing structure members using their names.
                    stringstream strs;
                    strs << storeVec[z].y;
                    string temp_str = strs.str();
                    char* char_type = (char*) temp_str.c_str();
                    temp=char_type;
                    printUI(temp);
                }
            }

            moveForward();

            // Check the direction and set the row and column
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

        /*    mousepos.setX(colcnt);
            mousepos.setY(rowcnt);

            if (isWallLeft() && isWallRight()&& isWallForward()){
                // Dead end - FRL closed
                reverseflag=1;

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

            else if (isWallLeft() && isWallRight()&& !isWallForward()){
                // F only open, continue forward without changing direction
            }

            else if (isWallLeft() && !isWallRight()&& isWallForward()){
                // R only open

                turnRight();
                storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O'});
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
                storeVec.push_back({mousepos.x(),mousepos.y(),'O','O','O'});
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

                reverseflag="N";

                int s = storeVec.size();
                for (int z=0;z<s;z++){
                    // Accessing structure members using their names.

                    if (mousepos.x()==storeVec[z].x && mousepos.y()==storeVec[z].y){
                        if (storeVec[z].d1=='L') {
                            turnLeft();
                            storeVec.push_back({mousepos.x(),mousepos.y(),'R','O','O'});
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
                    }
                }
            }

            else if (!isWallLeft() && isWallRight()&& !isWallForward()){
                // FL  open, turn left
                reverseflag="N";

                int s = storeVec.size();
                for (int z=0;z<s;z++) {
                    // Accessing structure members using their names.

                    if (mousepos.x()==storeVec[z].x && mousepos.y()==storeVec[z].y){
                        if (storeVec[z].d1=='L') {
                            turnLeft();
                            storeVec.push_back({mousepos.x(),mousepos.y(),'F','O','O'});
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
                    }
                }
            }

            else if (isWallLeft() && !isWallRight()&& !isWallForward()){
                // FR open, continue forward without changing direction
                reverseflag="N";

                int s = storeVec.size();
                for (int z=0;z<s;z++) {
                    // Accessing structure members using their names.

                    if (mousepos.x()==storeVec[z].x && mousepos.y()==storeVec[z].y){
                        if (storeVec[z].d1=='R') {
                            turnRight();
                            storeVec.push_back({mousepos.x(),mousepos.y(),'F','O','O'});
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
            }

            moveForward();

            // Check the direction and set the row and column
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
    */

    }

    //turnRight();
    //moveForward();
}


