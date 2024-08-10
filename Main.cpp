#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>

#include "API.h"
using namespace std;
// focus on junction

/* void log(const std::string& text) {
    std::cerr << text << std::endl;
} */

void log(const string& text) {
    cerr << text << endl;
}

void log(int var) {
    cerr << var << endl;
}

void log(char dir) {
    cerr << dir << endl;
}

int x = 0;
int y = 0;
int xprev=0;
int yprev=0;
int orient = 0;

int cells_num_visited[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//this is my code
int cells[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int flood[16][16]={
    {14,13,12,11,10,9,8,7,7,8,9,10,11,12,13,14},
        {13,12,11,10,9,8,7,6,6,7,8,9,10,11,12,13},
        {12,11,10,9,8,7,6,5,5,6,7,8,9,10,11,12},
        {11,10,9,8,7,6,5,4,4,5,6,7,8,9,10,11},
        {10,9,8,7,6,5,4,3,3,4,5,6,7,8,9,10},
        {9,8,7,6,5,4,3,2,2,3,4,5,6,7,8,9},
        {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8},
        {7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7},
        {7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7},
        {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8},
        {9,8,7,6,5,4,3,2,2,3,4,5,6,7,8,9},
        {10,9,8,7,6,5,4,3,3,4,5,6,7,8,9,10},
        {11,10,9,8,7,6,5,4,4,5,6,7,8,9,10,11},
        {12,11,10,9,8,7,6,5,5,6,7,8,9,10,11,12},
        {13,12,11,10,9,8,7,6,6,7,8,9,10,11,12,13},
        {14,13,12,11,10,9,8,7,7,8,9,10,11,12,13,14}
};

void showFlood() {
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            API::setText(x, y, to_string(flood[y][x]));
        }
    }
}

void updateWalls(int x, int y, int orient, bool L, bool R, bool F) {
    if (L && R && F) {
        if (orient == 0) {
            cells[y][x] = 13;
        } else if (orient == 1) {
            cells[y][x] = 12;
        } else if (orient == 2) {
            cells[y][x] = 11;
        } else if (orient == 3) {
            cells[y][x] = 14;
        }
    } else if (L && R && !F) {
        if (orient == 0 || orient == 2) {
            cells[y][x] = 9;
        } else if (orient == 1 || orient == 3) {
            cells[y][x] = 10;
        }
    } else if (L && F && !R) {
        if (orient == 0) {
            cells[y][x] = 8;
        } else if (orient == 1) {
            cells[y][x] = 7;
        } else if (orient == 2) {
            cells[y][x] = 6;
        } else if (orient == 3) {
            cells[y][x] = 5;
        }
    } else if (R && F && !L) {
        if (orient == 0) {
            cells[y][x] = 7;
        } else if (orient == 1) {
            cells[y][x] = 6;
        } else if (orient == 2) {
            cells[y][x] = 5;
        } else if (orient == 3) {
            cells[y][x] = 8;
        }
    } else if (F) {
        if (orient == 0) {
            cells[y][x] = 2;
        } else if (orient == 1) {
            cells[y][x] = 3;
        } else if (orient == 2) {
            cells[y][x] = 4;
        } else if (orient == 3) {
            cells[y][x] = 1;
        }
    } else if (L) {
        if (orient == 0) {
            cells[y][x] = 1;
        } else if (orient == 1) {
            cells[y][x] = 2;
        } else if (orient == 2) {
            cells[y][x] = 3;
        } else if (orient == 3) {
            cells[y][x] = 4;
        }
    } else if (R) {
        if (orient == 0) {
            cells[y][x] = 3;
        } else if (orient == 1) {
            cells[y][x] = 4;
        } else if (orient == 2) {
            cells[y][x] = 1;
        } else if (orient == 3) {
            cells[y][x] = 2;
        }
    }
}


bool isAccessible(int x, int y, int x1, int y1) {
    // Returns true if the mouse can move from (x, y) to (x1, y1) (two adjacent cells)
    if (x == x1) {
        if (y > y1) {
            // Moving up
            if (cells[y][x] == 4 || cells[y][x] == 5 || cells[y][x] == 6 || 
                cells[y][x] == 10 || cells[y][x] == 11 || cells[y][x] == 12 || 
                cells[y][x] == 14) {
                return false;
            } else {
                return true;
            }
        } else {
            // Moving down
            if (cells[y][x] == 2 || cells[y][x] == 7 || cells[y][x] == 8 || 
                cells[y][x] == 10 || cells[y][x] == 12 || cells[y][x] == 13 || 
                cells[y][x] == 14) {
                return false;
            } else {
                return true;
            }
        }
    } else if (y == y1) {
        if (x > x1) {
            // Moving left
            if (cells[y][x] == 1 || cells[y][x] == 5 || cells[y][x] == 8 || 
                cells[y][x] == 9 || cells[y][x] == 11 || cells[y][x] == 13 || 
                cells[y][x] == 14) {
                return false;
            } else {
                return true;
            }
        } else {
            // Moving right
            if (cells[y][x] == 3 || cells[y][x] == 6 || cells[y][x] == 7 || 
                cells[y][x] == 9 || cells[y][x] == 11 || cells[y][x] == 12 || 
                cells[y][x] == 13) {
                return false;
            } else {
                return true;
            }
        }
    }

    // If neither x nor y coordinates are the same, the cells are not adjacent
    return false;
}

tuple<int, int, int, int, int, int, int, int> getSurrounds(int x, int y) {
    // Returns the coordinates of the four surrounding squares
    int x0 = x;
    int y0 = y + 1;  // North
    int x1 = x + 1;
    int y1 = y;      // East
    int x2 = x;
    int y2 = y - 1;  // South
    int x3 = x - 1;
    int y3 = y;      // West

    if (x1 >= 16) x1 = -1;
    if (y0 >= 16) y0 = -1;

    return make_tuple(x0, y0, x1, y1, x2, y2, x3, y3);
}

bool isConsistent(int x, int y) {
    //log("Checking consistency..");
    // Returns true if the value of the current square is one greater than the minimum value in an accessible neighbor
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
    
    tie(x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    int val = flood[y][x];
    int minVals[4] = {-1, -1, -1, -1};

    if (x0 >= 0 && y0 >= 0 && isAccessible(x, y, x0, y0)) {
        minVals[0] = flood[y0][x0];
    }
    if (x1 >= 0 && y1 >= 0 && isAccessible(x, y, x1, y1)) {
        minVals[1] = flood[y1][x1];
    }
    if (x2 >= 0 && y2 >= 0 && isAccessible(x, y, x2, y2)) {
        minVals[2] = flood[y2][x2];
    }
    if (x3 >= 0 && y3 >= 0 && isAccessible(x, y, x3, y3)) {
        minVals[3] = flood[y3][x3];
    }

    int minCount = 0;
    for (int i = 0; i < 4; ++i) {
        if (minVals[i] == -1) {
            continue;
        } else if (minVals[i] == val + 1) {
            continue;
        } else if (minVals[i] == val - 1) {
            ++minCount;
        }
    }

    return (minCount > 0);
}

void makeConsistent(int x, int y) {
    //log("Making consistent..");
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;

    tie (x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    int val = flood[y][x];
    int minVals[4] = {-1, -1, -1, -1};

    if (x0 >= 0 && y0 >= 0 && isAccessible(x, y, x0, y0)) {
        minVals[0] = flood[y0][x0];
    }
    if (x1 >= 0 && y1 >= 0 && isAccessible(x, y, x1, y1)) {
        minVals[1] = flood[y1][x1];
    }
    if (x2 >= 0 && y2 >= 0 && isAccessible(x, y, x2, y2)) {
        minVals[2] = flood[y2][x2];
    }
    if (x3 >= 0 && y3 >= 0 && isAccessible(x, y, x3, y3)) {
        minVals[3] = flood[y3][x3];
    }

    for (int i = 0; i < 4; ++i) {
        if (minVals[i] == -1) {
            minVals[i] = 1000;
        }
    }

    int minVal = *min_element(minVals, minVals + 4);
    flood[y][x] = minVal + 1;
}

void floodFill(int x, int y, int xprev, int yprev) {
    //log("Flood fill running..");
    if (!isConsistent(x, y)) {
        flood[y][x] = flood[yprev][xprev] + 1;
    }
    
    stack<int> stack;
    stack.push(x);
    stack.push(y);
    
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;

    tie (x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    if (x0 >= 0 && y0 >= 0 && isAccessible(x, y, x0, y0)) {
        stack.push(x0);
        stack.push(y0);
    }
    if (x1 >= 0 && y1 >= 0 && isAccessible(x, y, x1, y1)) {
        stack.push(x1);
        stack.push(y1);
    }
    if (x2 >= 0 && y2 >= 0 && isAccessible(x, y, x2, y2)) {
        stack.push(x2);
        stack.push(y2);
    }
    if (x3 >= 0 && y3 >= 0 && isAccessible(x, y, x3, y3)) {
        stack.push(x3);
        stack.push(y3);
    }

    while (!stack.empty()) {
        int yrun = stack.top(); stack.pop();
        int xrun = stack.top(); stack.pop();

        if (isConsistent(xrun, yrun)) {
            continue;
        } else {
            makeConsistent(xrun, yrun);
            stack.push(xrun);
            stack.push(yrun);

            tie(x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(xrun, yrun);
            if (x0 >= 0 && y0 >= 0 && isAccessible(xrun, yrun, x0, y0)) {
                stack.push(x0);
                stack.push(y0);
            }
            if (x1 >= 0 && y1 >= 0 && isAccessible(xrun, yrun, x1, y1)) {
                stack.push(x1);
                stack.push(y1);
            }
            if (x2 >= 0 && y2 >= 0 && isAccessible(xrun, yrun, x2, y2)) {
                stack.push(x2);
                stack.push(y2);
            }
            if (x3 >= 0 && y3 >= 0 && isAccessible(xrun, yrun, x3, y3)) {
                stack.push(x3);
                stack.push(y3);
            }
        }
    }

    //log("Flood fill complete.");
}

char toMove(int x, int y, int orient,int xprev, int yprev) {
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;

    int xfront;
    int yfront;
    int xright;
    int yright;
    int xleft;
    int yleft;

    tie (x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    int val = flood[y][x];

    //log("x0: " + to_string(x0) + " y0: " + to_string(y0) + " x1: " + to_string(x1) + " y1: " + to_string(y1) + " x2: " + to_string(x2) + " y2: " + to_string(y2) + " x3: " + to_string(x3) + " y3: " + to_string(y3));
    // PROBLEM : MOUSE STUCK (not) EVERYTIME IT NEEDS TO DO FLOODFILL
    //MOUSE IS STUCK WHEN DISTANCES ARE EQUAL (EG FRONT AND LEFT) MEANWHILE FRONT IS NOT ACCESSIBLE, SO LOOP KEEPS ON REPEATING
    //PRIORITY = FORWARD > RIGHT > LEFT > BACK

    if (orient==0){
        xfront=x0;
        yfront=y0;
        xright=x1;
        yright=y1;
        xleft=x3;
        yleft=y3;
    }

    else if (orient==1){
        xfront=x1;
        yfront=y1;
        xright=x2;
        yright=y2;
        xleft=x0;
        yleft=y0;
    }

    else if (orient==2){
        xfront=x2;
        yfront=y2;
        xright=x3;
        yright=y3;
        xleft=x1;
        yleft=y1;
    }

    else if (orient==3){
        xfront=x3;
        yfront=y3;
        xright=x0;
        yright=y0;
        xleft=x2;
        yleft=y2;
    }

    //if moving forward will give flood = current -1
    if (xfront >= 0 && yfront >= 0 && isAccessible(x, y, xfront, yfront)&& flood[yfront][xfront] == val - 1) {
        //log("check if front is accessible");
        return 'F';
    } else if (xright >= 0 && yright >= 0 && isAccessible(x, y, xright, yright) && flood[yright][xright] == val - 1) { //turning right is a priority (will be checked before turning left)
        return 'R';

    } else if (xleft >= 0 && yleft >= 0  && isAccessible(x, y, xleft, yleft) && flood[yleft][xleft] == val - 1) {
        return 'L';
    } else {
        return 'B'; //turn back if mouse runs into a dead end
        
    }
}
//this is my code

void updateCoordinates(){
    if(orient == 0){y += 1;}
    else if(orient == 1){x += 1;}
    else if(orient == 2){y -= 1;}
    else if(orient == 3){x -= 1;}
}

void updateOrient(char turning){
    if (turning == 'L'){
        orient -= 1;
        if (orient == -1){orient = 3;}
    }
    else if(turning == 'R'){
        orient += 1;
        if (orient == 4){orient=0;}
    }
    else if(turning== 'B'){
        if (orient==0){orient=2;}
        else if (orient==1){orient=3;}
        else if (orient==2){orient=0;}
        else if (orient==3){orient=1;}
    }
}

int nextCellNumVisited(int x, int y, int orient){
    if(orient == 0){y -= 1;}
    else if(orient == 1){x += 1;}
    else if(orient == 2){y += 1;}
    else if(orient == 3){x -= 1;}

    return cells_num_visited[y][x];
}

int main(int argc, char* argv[]) {
    log("Running...");
    showFlood();
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "starting");
    while (true) {
        int w_left = API::wallLeft();
        int w_right = API::wallRight();
        int w_front = API::wallFront();
        
        //update walls > move to adjacent cell with flood -1 (if can, if cannot, update flood) > update coordinates and orient
        

        updateWalls(x, y, orient, w_left, w_right, w_front);

        //put walls (for debug purposes)
        if (orient==0){
            if(w_front){
                API::setWall(x, y, 'n');
                /* log("wall set"); */
            }
            if(w_left){
                API::setWall(x, y, 'w');
                /* log("wall set"); */
            }
            if(w_right){
                API::setWall(x, y, 'e');
                /* log("wall set"); */
        }}

        else if (orient==1){
            if(w_front){
                API::setWall(x, y, 'e');
                /* log("wall set"); */
            }
            if(w_left){
                API::setWall(x, y, 'n');
                /* log("wall set"); */
            }
            if(w_right){
                API::setWall(x, y, 's');
                /* log("wall set"); */
        }}

        else if (orient==2){
            if(w_front){
                API::setWall(x, y, 's');
                /* log("wall set"); */
            }
            if(w_left){
                API::setWall(x, y, 'e');
                /* log("wall set"); */
            }
            if(w_right){
                API::setWall(x, y, 'w');
                /* log("wall set"); */
        }}

        else if (orient==3){
            if(w_front){
                API::setWall(x, y, 'w');
                /* log("wall set"); */
            }
            if(w_left){
                API::setWall(x, y, 's');
                /* log("wall set"); */
            }
            if(w_right){
                API::setWall(x, y, 'n');
                /* log("wall set"); */
        }}

        //delete when issue is fixed
        if (w_front){
            log("wall front");
        }
        if (w_left){
            log("wall left");
        }
        if (w_right){
            log("wall right");
        }


        if (flood[y][x]!=0){
            floodFill(x,y,xprev,yprev);}
        showFlood();
        char direction=toMove(x,y,orient,xprev,yprev);
        //log(direction);
        xprev=x;
        yprev=y;
        if(direction == 'F'){
            API::moveForward();
        }
        else if(direction == 'R'){
            API::turnRight();
            updateOrient('R');
            API::moveForward();
        }
        else if(direction == 'L'){
            API::turnLeft();
            updateOrient('L');
            API::moveForward();
        }
        else if(direction == 'B'){
            API::turnRight();
            updateOrient('R');
            API::turnRight();
            updateOrient('R');
            API::moveForward();
        }
        updateCoordinates();

        

        //(if cannot) update flood > show flood > move > update coordinates > update walls

        /* cells_num_visited[y][x] += 1;

        if(x>= 7 && x<= 8 && y>=7 && y<=8){
            log("Finish");
            break;
        }


        // At each cell, the robot decides whether to turn left, move forward, or turn right. It will choose the path that has been visited the least.
        int minVisited = -1;
        char potentialTurn = 'B';        
        if(!API::wallRight()){
            if(minVisited > nextCellNumVisited(x,y, (orient+1)%4 ) || minVisited == -1){
                minVisited = nextCellNumVisited(x,y, (orient+1)%4 );
                potentialTurn = 'R';
            }
        }
        if(!API::wallFront()){
            if(minVisited > nextCellNumVisited(x,y, orient)  || minVisited == -1){
                minVisited = nextCellNumVisited(x,y, orient);
                potentialTurn = 'F';
            }
        }
        if(!API::wallLeft()){
            if(minVisited > nextCellNumVisited(x,y, (orient-1)%4 ) || minVisited == -1){
                minVisited = nextCellNumVisited(x,y, (orient-1)%4 );
                potentialTurn = 'L';
            }
        }


        // The robot will choose its path based on the value of potentialTurn (L:turn left, F: move forward, R: turn right, B: turn backward)
        if(potentialTurn == 'L'){
            API::turnLeft();
            updateOrient('L');

            API::moveForward();
            updateCoordinates();
        } else if (potentialTurn == 'F'){
            API::moveForward();
            updateCoordinates();
        } else if(potentialTurn == 'R'){
            API::turnRight();
            updateOrient('R');

            API::moveForward();
            updateCoordinates();
        } else{
            API::turnRight();
            updateOrient('R');

            API::turnRight();
            updateOrient('R');
        }
 */
        

    }
}
