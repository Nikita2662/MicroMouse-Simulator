
#include "micromouseserver.h"
#include "qstack.h"
#include "qset.h"
#include "qstring.h"


enum Direction {West, East, North, South };
enum Movement  {Left, Right, Forward, None, TurnBack};
char debugText [512];

class Position { // target, adjacentMovement, advance, changeDirection
public:
    Position(){row=0;col=0; direction=North;}
    Position(int r, int c, Direction d) { // overloading with second constructor method
        row = r;
        col = c;
        direction = d;
    }

    Position(const Position& other){ // overloading with third constructor method
        row = other.row;
        col = other.col;
        direction = other.direction;
    }

    int row;
    int col;
    Direction direction;
    const char* directionString[4] = {"W", "E", "N", "S"};

    QString coordinateString() {
        sprintf(debugText, "%d-%d", row, col); // printing row & column to string debugText
        return QString(debugText);
    }

    char* debug(){
        sprintf(debugText, "%d-%d-%s", row, col, directionString[direction]);
        return debugText;
    }

    // return the target from the given position for the movement

    Position target(Movement m) {
        int targetCol = col;
        int targetRow = row;

        /**  currentDirection    Left       Right      Forward

                 West             row-1      row+1      col-1
                 East             row+1      row-1      col+1
                 North            col-1      col+1      row+1
                 South            col+1      col-1      row-1
         **/

        switch(direction) { // changes row or column based on orientation of mouse and direction turned
        case West:
            if(m == Left) --targetRow;
            if(m == Right) ++targetRow;
            if(m == Forward) --targetCol;
            break;
        case East:
            if(m == Left) ++targetRow;
            if(m == Right) --targetRow;
            if(m == Forward) ++targetCol;
            break;
        case North:
            if(m == Left) --targetCol;
            if(m == Right) ++targetCol;
            if(m == Forward) ++targetRow;
            break;
        case South:
            if(m == Left) ++targetCol;
            if(m == Right) --targetCol;
            if(m == Forward) --targetRow;
            break;
        }

        return Position(targetRow, targetCol, direction); // returns a Position object: based on a movement, returns x & y of target cell
    }

    Movement adjacentMovement(Position target) {
        Movement m = None;
        int colDifference = target.col - col;
        int rowDifference = target.row - row;


        /** (Current directions):E         W          N            S

         Desired Direction

        colDifference  West      TurnBack   None        Left         Right
        colDifference  East      None       Turnback    Right        Left
        rowDifference -North     Left       Right       None         TurnBack
        rowDifference -South     Right      Left        Turnback     None

         **/

        if(abs(colDifference) + abs(rowDifference) == 1) { // making sure current position & target position are adjacent
            Direction destinationDirection = North;

            if(colDifference == 0) { // then only moving row (up/ down)
                destinationDirection = rowDifference < 0 ? South: North; // if rowDifference is negative, move south
            }

            else { // if moving col (right/ left)
                destinationDirection = colDifference < 0 ? West : East; // if colDifference is negative, move west
            }

            switch(destinationDirection) { // how to get to target destination from current destination & orientation
            case West: // target is to the west
                if(direction == East) m = TurnBack; // currently facing east, turn 180 deg
                if(direction == West) m = Forward;
                if(direction == North) m = Left;
                if(direction == South) m = Right;
                break;
            case East:
                if(direction == East) m = Forward;
                if(direction == West) m = TurnBack;
                if(direction == North) m = Right;
                if(direction == South) m = Left;
                break;
            case North:
                if(direction == East) m = Left;
                if(direction == West) m = Right;
                if(direction == North) m = Forward;
                if(direction == South) m = TurnBack;
                break;
            case South:
                if(direction == East) m = Right;
                if(direction == West) m = Left;
                if(direction == North) m = TurnBack;
                if(direction == South) m = Forward;
                break;
            }
        }

        return m; // returning the direction
    }

    void advance(){ // adjusting row & col count based on movement made in a specific direction
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

class PositionWallCount {
public:
    Position position;
    int wallCount;
    PositionWallCount() {}

    PositionWallCount(Position p, int c) { // overloaded
        position = p;
        wallCount = c;
    }

    char* debug(){
        sprintf(debugText, "%s-%d", position.debug(), wallCount);
        return debugText;
    }
};

class Path{
public:
    bool backtrack;
    Position currPosition;
    QList<PositionWallCount> previousPositions; // list of previous positions
    //QStack<Movement> movements;

    Path(){
        backtrack = false;
    }

    void left() {
        currPosition.changeDirection(Left);
        //if(!backtrack) movements.push(Left);
    }

    void right() {
        currPosition.changeDirection(Right);
        //if(!backtrack) movements.push(Right);
    }

    void forward(int wallcount) {
        //if(!backtrack) movements.push(Forward);
        if(!backtrack) previousPositions.push_back(PositionWallCount(currPosition, wallcount));
        currPosition.advance();
    };

    bool isFinished() {
        int startCount = previousPositions.length() - 4;
        return (previousPositions.count() >= 4 && previousPositions[startCount + 0].position.row == currPosition.row && // if at least 4 squares visited previously (2x2 square) && if the 1st square visited = square right now
                previousPositions[startCount + 0].position.col == currPosition.col &&
                (abs(previousPositions[startCount + 0].position.col - previousPositions[startCount + 2].position.col) == 1
                && abs(previousPositions[startCount + 0].position.row - previousPositions[startCount + 2].position.row) == 1) && // the 1st square & the 3rd square (diagonals) are apart by 1 row & 1 col
                (abs(previousPositions[startCount + 1].position.col - previousPositions[startCount + 3].position.col) == 1
                && abs(previousPositions[startCount + 1].position.row - previousPositions[startCount + 3].position.row) == 1) &&
                previousPositions[startCount + 0].wallCount == 1 && // each position should only have 1 wall in front or left or right after turning
                previousPositions[startCount + 1].wallCount == 1 &&
                previousPositions[startCount + 2].wallCount == 1 &&
                previousPositions[startCount + 3].wallCount == 1);
    }

    Movement moveBackOnePosition() { // backtracking
        Movement m = None;

        if(!previousPositions.isEmpty()) { // if there was previous movement
            PositionWallCount prevPosition = previousPositions.back();
            previousPositions.pop_back();

            m = currPosition.adjacentMovement(prevPosition.position);
        }

        return m;
    }

};


class AltPath {
public:
    Position fromPosition;
    Position toPosition;
    AltPath() { }

    AltPath (Position p, Movement m) { // overloading
        fromPosition = p;
        toPosition = fromPosition.target(m);
    }

    AltPath(const AltPath& other){ // overloading
        fromPosition = other.fromPosition;
        toPosition = other.toPosition;
    }
};

void microMouseServer::studentAI() {

    /* ORIGINAL PSEUDOCODE:
    move forward until you reach a point where there is no wall left and/ or right
    store alternative path (store which direction it is as part of currPosition)
    once you reach a point where cannot move unless 2 turn rights/ left (3 walls), then backtracking
    go to last decision point and take alternative path & delete alternative path
    figure out if finished */

    static Path currPath; // starts at (0,0)
    static QStack<AltPath> altPaths;
    static QSet<QString> visitedPositions;

    //static QList<PositionWallCount> previousPositions;

    char buf[250];
    bool forwardMove = false;

    sprintf(buf, "currPath positions count %d", currPath.previousPositions.count());
    printUI(buf);

    bool alreadyVisitedCurrentPosition = visitedPositions.contains(currPath.currPosition.coordinateString());


    //  stores number of walls (forward, left, right)
    int wallCount = (isWallForward() ? 1 : 0) + (isWallLeft() ? 1 : 0) + (isWallRight() ? 1 : 0);

    // if finished, print message and call method
    if(currPath.isFinished()) {
        printUI("foundFinish");
        foundFinish();
        return;
    }

    // if backtracking, check if it is the top alternative path
    if(currPath.backtrack) {
        Movement m = None;
        if(!altPaths.isEmpty() && altPaths.top().fromPosition.row == currPath.currPosition.row
                && altPaths.top().fromPosition.col == currPath.currPosition.col
                /*&& altPaths.top().position.direction == currPosition.direction**/)
        {
            // move as per the top of the stack
            currPath.backtrack = false;
            AltPath altPath = altPaths.pop();
            m = currPath.currPosition.adjacentMovement(altPath.toPosition);

            sprintf(buf, "backtrack end at %d - %d.  Stack size after pop - %d", altPath.fromPosition.row, altPath.fromPosition.col, altPaths.size());
            printUI(buf);
        }

        else {
            // pop the currPath stack
            /**printUI(currPath.currPosition.debug());
            if(!currPath.previousPositions.isEmpty())
            {
                printUI(currPath.previousPositions.back().debug());
            }
            **/
            m = currPath.moveBackOnePosition();
        }

        sprintf(buf, "Move is %d", m);
        printUI(buf);

        switch(m) {
        case Right:
            turnRight();
            currPath.right();
            break;
        case Left:
            turnLeft();
            currPath.left();
            break;
        case TurnBack:
            turnRight();
            currPath.right();
            turnRight();
            currPath.right();
            break;
        }

        // if m = none, mouse has backtracked to beginning and all alternative paths are exhausted
        if (m != None) {
            forwardMove = true;
        }

        else {
            foundFinish(); // didn't find a path
        }
    }

    else {
        if(alreadyVisitedCurrentPosition || wallCount == 3) { // dead end
            currPath.backtrack = true;
            printUI("dead end or already visited. start backtrack");
            printUI(currPath.currPosition.debug());
        }

        else // movement possible {
            forwardMove = true;

        if(!isWallForward()) { // if forward is possible
            // save alternate paths if not backtracking
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
        }

        else if(!isWallRight()) { // if right is possible
            if (!alreadyVisitedCurrentPosition && !isWallLeft()) { // add left as an alternative path
                altPaths.push(AltPath(currPath.currPosition, Left));
                sprintf(buf, "Pushed Alt Path Left at %d - %d. Stack Size - %d", currPath.currPosition.row, currPath.currPosition.col, altPaths.size());
                printUI(buf);
            }

            sprintf(buf, "Turning Right at %d - %d", currPath.currPosition.row, currPath.currPosition.col);
            printUI(buf);
            turnRight();
            currPath.right();
        }

        else if(!isWallLeft()) { // if left is possible
            sprintf(buf, "Turning Left at %d - %d", currPath.currPosition.row, currPath.currPosition.col);
            printUI(buf);
            turnLeft();
            currPath.left();
        }
    }

    if(!alreadyVisitedCurrentPosition) { // adding current position to visitedPositions if this is the first visit
        visitedPositions.insert(currPath.currPosition.coordinateString());
    }

    // move forward if there isn't a wall
    if(forwardMove && !isWallForward()) {
        int wallCount = (isWallForward() ? 1 : 0) + (isWallLeft() ? 1 : 0) + (isWallRight() ? 1 : 0);
        sprintf(buf, "moving forward from %s", currPath.currPosition.debug());
        printUI(buf);
        moveForward();
        currPath.forward(wallCount);
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


