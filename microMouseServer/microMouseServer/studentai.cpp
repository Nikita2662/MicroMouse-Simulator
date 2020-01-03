
#include "micromouseserver.h"
#include "qstack.h"
#include "qset.h"
#include "qstring.h"


enum Direction {West, East, North, South };
enum Movement  {Left, Right, Forward, None};
char debugText [512];

class Position {
public:
    Position(){row=0;col=0; direction=North;}
    Position(const Position& other){ // overloading
        row = other.row;
        col = other.col;
        direction = other.direction;
    }

    int row;
    int col;
    Direction   direction;
    const char* directionString[4] = {"W", "E", "N", "S"};

    QString coordinateString() {
        sprintf(debugText, "%d-%d", row, col); // printing row and col to string debugText
        return QString(debugText);
    }

    char* debug(){
        sprintf(debugText, "%d-%d-%s", row, col, directionString[direction]);
        return debugText;
    }

    void advance(){ // adjusting row and col count based on movement made in a specific direction
        if(direction == West) {
            col -= 1;
        }

        else if (direction == East) {
            col += 1;
        }

        else if (direction == North) {
            row += 1;
        }

        else if (direction == South) {
            row -= 1;
        }
    }

    void changeDirection(Movement x) { // changes direction (NEWS) based on if right or left turn AND current direction
        if (x == Right) {
            switch(direction) {
            case North:
                direction = East;
                break;
            case South:
                direction = West;
                break;
            case West:
                direction = North;
                break;
            case East:
                direction = South;
                break;
            }
        }

        else if (x == Left) {
            switch(direction) {
            case North:
                direction = West;
                break;
            case South:
                direction = East;
                break;
            case West:
                direction = South;
                break;
            case East:
                direction = North;
                break;
            }
        }
    }
};


class Path{
public:
    Position currPosition;
    bool backtrack;
    QStack<Movement> movements;
    Path(){
        backtrack = false;
    }

    void turnLeft() {
        currPosition.changeDirection(Left);
        if(!backtrack) movements.push(Left);
    }

    void turnRight() {
        currPosition.changeDirection(Right);
        if(!backtrack) movements.push(Right);
    }

    void moveForward() {
        currPosition.advance();
        if(!backtrack) movements.push(Forward);
    };

    Movement lastMovement() {
        return movements.isEmpty() ? None : movements.pop();
    }
};

class PositionWallCount {
public:
    Position position;
    int wallCount;
    PositionWallCount() {}

    PositionWallCount(Position p, int c) { // overloaded
        position = p;
        wallCount = c;
    }

    char* debug() {
        sprintf(debugText, "%s-%d", position.debug(), wallCount);
        return debugText;
    }
};

class AltPath {
public:
    Position position;
    Movement turn;
    AltPath(){}
    AltPath (Position p, Movement t) { // overloaded
        position = p;
        turn = t;
    }

    AltPath(const AltPath& other){
        position = other.position;
        turn = other.turn;
    }

};

void microMouseServer::studentAI() {

    // move forward until you reach a point where there is no wall left and/ or right
    // store alternative path (store which direction it is as part of currPosition)
    // once you reach a point where cannot move unless 2 turn rights/ left (3 walls), then backtracking
    // go to last decision point and take alternative path & delete alternative path
    // figure out if finished

    static Path  currPath;
    // static Position currPosition;
    static QStack<AltPath> altPaths;
    static QSet<QString> visitedPositions;
    static QList<PositionWallCount> previousPositions;

    char buf[250];
    bool forwardMove = false;

    sprintf(buf, "currPath Movement count %d", currPath.movements.count());
    printUI(buf);

    bool alreadyVisitedCurrentPosition = visitedPositions.contains(currPath.currPosition.coordinateString());
    //  get wall counts
    int wallCount = (isWallForward() ? 1 : 0) + (isWallLeft() ? 1 : 0) + (isWallRight() ? 1 : 0);

    /* sprintf(buf, "prevCount-%d", previousPositions.count());
    printUI(buf);
    if(previousPositions.count() == 4) {
        printUI(previousPositions[0].debug());
        printUI(previousPositions[1].debug());
        printUI(previousPositions[2].debug());
        printUI(previousPositions[3].debug());
    }
    */

    // if finished, mark finished and exit
    if(previousPositions.count() == 4 && previousPositions[0].position.row == currPath.currPosition.row &&
            previousPositions[0].position.col == currPath.currPosition.col &&
            (abs(previousPositions[0].position.col - previousPositions[2].position.col) == 1
             && abs(previousPositions[0].position.row - previousPositions[2].position.row) == 1) &&
            (abs(previousPositions[1].position.col - previousPositions[3].position.col) == 1
             && abs(previousPositions[1].position.row - previousPositions[3].position.row) == 1) &&
            previousPositions[0].wallCount == 1 &&
            previousPositions[1].wallCount == 1 &&
            previousPositions[2].wallCount == 1 &&
            previousPositions[3].wallCount == 1) {
        printUI("foundFinish");
        foundFinish();
        return;
    }

    // if the current node is already visited, then check if it is the top alt path
    if(currPath.backtrack) {
        if(!altPaths.isEmpty() && altPaths.top().position.row == currPath.currPosition.row
                && altPaths.top().position.col == currPath.currPosition.col
                /*&& altPaths.top().position.direction == currPosition.direction**/) {
            // move per the top of the stack
            currPath.backtrack = false;
            bool left = true;
            if(altPaths.top().turn == Left &&
                    altPaths.top().position.direction != currPath.currPosition.direction) {
                left = false;
            }

            else if(altPaths.top().turn == Right &&
                    altPaths.top().position.direction == currPath.currPosition.direction){
                left = false;
            }

            if (left) { // if alternative path is Left
                turnLeft();
                currPath.turnLeft();
            }

            else { // if alternative path is right
                turnRight();
                currPath.turnRight();
            }

            AltPath altPath = altPaths.pop();

            sprintf(buf, "backtrack end at %d - %d.  Stack size after pop - %d", altPath.position.row, altPath.position.col, altPaths.size());
            printUI(buf);
        }

        else {
            // pop the currPath stack
            Movement m = currPath.lastMovement();
            switch(m) {
            case Forward:
                forwardMove = true;
                break;
            case Left:
                turnRight();
                currPath.turnRight();
                break;
            case Right:
                turnLeft();
                currPath.turnLeft();
                break;
            }
        }
    }

    else {
        if(wallCount == 3) {
            //dead end
            currPath.backtrack = true;
            printUI("dead end. backtrack start");
            // turn around
            turnRight();
            currPath.turnRight();
            turnRight();
            currPath.turnRight();
            printUI(currPath.currPosition.debug());
        }

        else // movement possible {
            // if forward is possible then
            if(!isWallForward()) {
                // save alternate altPaths if not backtracking
                if(!alreadyVisitedCurrentPosition) {
                    if (!isWallLeft()) { // adds left as an alternative path
                        altPaths.push(AltPath(currPath.currPosition, Left));
                        sprintf(buf, "Pushed Alt Path Left at %d - %d. Stack Size - %d", currPath.currPosition.row, currPath.currPosition.col, altPaths.size());
                        printUI(buf);
                    }

                    if (!isWallRight()) { // adds right as an alternative path
                        altPaths.push(AltPath(currPath.currPosition, Right));
                        sprintf(buf, "Pushed Alt Path Right at %d - %d. Stack Size - %d", currPath.currPosition.row, currPath.currPosition.col, altPaths.size());
                        printUI(buf);
                    }
                }
                forwardMove = true;
            }

            else if(!isWallRight()) {
                if (!alreadyVisitedCurrentPosition && !isWallLeft()) {
                    // add left as an alternative path
                    altPaths.push(AltPath(currPath.currPosition, Left));
                    sprintf(buf, "Pushed Alt Path Left at %d - %d. Stack Size - %d", currPath.currPosition.row, currPath.currPosition.col, altPaths.size());
                    printUI(buf);
                }

                sprintf(buf, "Turning Right at %d - %d", currPath.currPosition.row, currPath.currPosition.col);
                printUI(buf);
                turnRight();
                currPath.turnRight();
            }

            else if(!isWallLeft()) {
                sprintf(buf, "Turning Left at %d - %d", currPath.currPosition.row, currPath.currPosition.col);
                printUI(buf);
                turnLeft();
                currPath.turnLeft();
            }
    }

    visitedPositions.insert(currPath.currPosition.coordinateString());

if (forwardMove) {
    // keep track of only 4
    if(previousPositions.count() >= 4) {
        previousPositions.pop_front();
    }

    previousPositions.push_back(PositionWallCount(currPath.currPosition, wallCount));

    sprintf(buf, "moving forward from %s", currPath.currPosition.debug());
    printUI(buf);
    moveForward();
    currPath.moveForward();
    printUI(currPath.currPosition.debug());
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


