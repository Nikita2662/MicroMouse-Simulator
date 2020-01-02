#include "micromouseserver.h"
#include "qstack.h"


void microMouseServer::studentAI() {

    enum Direction {Left=1, Right, Top, Down };
    class Position {
    public:
        Position(){row=0;col=0;direction=Top;}
        int row;
        int col;
        char debugText[256];
        Direction   direction;
        char* debug(){
            sprintf(debugText, "%d-%d-%d", row, col, direction);
            return debugText;
        }

        void advance(){ // adjusts row and column count based on direction moved
            if(direction == Left) {
                col -= 1;
            }

            else if (direction == Right) {
                col += 1;
            }

            else if (direction == Top) {
                row += 1;
            }

            else if (direction == Down) {
                row -= 1;
            }
        }
        void changeDirect(Direction x) { // changes direction based on orientation of mouse
            if (x == Right) {
                switch(direction) {
                case Top:
                    direction = Right;
                    break;
                case Down:
                    direction = Left;
                    break;
                case Left:
                    direction = Top;
                    break;
                case Right:
                    direction = Down;
                    break;
                }
            }

            else if (x == Left) {
                switch(direction) {
                case Top:
                    direction = Left;
                    break;
                case Down:
                    direction = Right;
                    break;
                case Left:
                    direction = Down;
                    break;
                case Right:
                    direction = Top;
                    break;
                }
            }
        }
    };

    class AltPath {
    public:
        Position position;
        Direction direction;
        AltPath(){

        }

        AltPath (Position p, Direction d) { // constructor
            position = p;
            direction = d;
        }

        AltPath(const AltPath& other){
            position = other.position;
            direction = other.direction;
        }

    };

    static Position currPosition;
    static QStack<AltPath> paths;

    // PSEUDOCODE:
    // move forward until you reach a point where there is no wall left and/ or right
    // store alternative path (store which direction it is as part of currPosition)
    // once you reach a point where cannot move unless 2 turn rights/ left (3 walls), then backtracking
    // go to last decision point and take alternative path & delete alternative path
    // figure out if finished

    char buf [250];

    if (!isWallForward()){
        if (!isWallLeft()) { // adds left as an alternative path
            paths.push(AltPath(currPosition, Left));
            // printUI("pushAltPath Left");
            printUI(currPosition.debug());
            sprintf(buf, "Left. stack size - %d", paths.size());
            printUI(buf);
        }

        if (!isWallRight()) { // adds right as an alternative path
            paths.push(AltPath(currPosition, Right));
            //printUI("pushAltPath Right");
            printUI(currPosition.debug());

            sprintf(buf, "Right. stack size - %d", paths.size());
            printUI(buf);
        }

        moveForward();
        currPosition.advance();
    }

    else if(!isWallRight()){
        if (!isWallLeft()) { // adds left as an alternative path
            paths.push(AltPath(currPosition, Left));
            // printUI("pushAltPath Left");
            printUI(currPosition.debug());
            sprintf(buf, "Left. stack size - %d", paths.size());
            printUI(buf);

        }
    }

    else { // if there is wall forward
        if (!paths.isEmpty() && paths.top().position.row == currPosition.row
                && paths.top().position.col == currPosition.col
                /*&& paths.top().position.direction == currPosition.direction**/) { // if all conditions from top entry in paths match that of currPosition

            bool left = true;
            if(paths.top().direction == Left &&
                    paths.top().position.direction != currPosition.direction) {
                left = false;
            }

            else if(paths.top().direction == Right &&
                    paths.top().position.direction == currPosition.direction){
                left = false;
            }

            if (left) { // if alternative path is Left
                turnLeft();
                currPosition.changeDirect(Left);
                //printUI("Taking AltPath left");
            }

            else { // if alternative path is right
                turnRight();
                currPosition.changeDirect(Right);
                //printUI("Taking AltPath Right");
            }
            paths.pop();
            sprintf(buf, "popped. stack size - %d", paths.size());
            printUI(buf);

        }

        else { // backtracking logic (after dead end)
            printUI("backtrack");
            if (isWallRight()) {
                if (isWallLeft()) {
                    turnRight();
                    currPosition.changeDirect(Right);
                    turnRight();
                    currPosition.changeDirect(Right);
                }

                else {
                    turnLeft();
                    currPosition.changeDirect(Left);
                }
            }

            else {
                turnRight();
                currPosition.changeDirect(Right);
            }
        }
    }
}

/*
 * The following are the eight functions that you can call. Feel free to create your own functions as well.
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


