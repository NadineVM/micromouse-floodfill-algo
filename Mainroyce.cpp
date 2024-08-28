#include <iostream>
#include <string>
#include<cstring>

#include "API.h"

// focus on junction

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

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

void updateCoordinates(int &x, int &y, int &orient){
    if(orient == 0){y -= 1;}
    else if(orient == 1){x += 1;}
    else if(orient == 2){y += 1;}
    else if(orient == 3){x -= 1;}
}

void updateOrient(char turning, int &orient){
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

// Note: the variable is not passed as reference (&) because we don't want to change the value of 'real' x and y
int nextCellNumVisited(int x_analyze, int y_analyze, int orient){
    if(orient == 0){y_analyze -= 1;}
    else if(orient == 1){x_analyze += 1;}
    else if(orient == 2){y_analyze += 1;}
    else if(orient == 3){x_analyze -= 1;}

    return cells_num_visited[y_analyze][x_analyze];
}

void update_robot_path(char (&robot_path)[16][16], int &x, int &y, int &orient){
    if(orient == 0){robot_path[y][x] = 'U';}
    else if(orient == 1){robot_path[y][x] = 'R';}
    else if(orient == 2){robot_path[y][x] = 'D';}
    else if(orient == 3){robot_path[y][x] = 'L';}
}

void reverse_value_robot_path(char (&robot_path)[16][16]){
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            if(robot_path[i][j] == 'U'){robot_path[i][j] ='D';}           
            else if(robot_path[i][j] == 'R'){robot_path[i][j] = 'L';}
            else if(robot_path[i][j] == 'D'){robot_path[i][j] = 'U';}
            else if(robot_path[i][j] == 'L'){robot_path[i][j] = 'R';}
        }
    }
}

int analyze_maze_path(char (&robot_path)[16][16]){
    int x_analyze = 0;
    int y_analyze = 15;
    int count_cells = 0;

    while(true){
        if(x_analyze>= 7 && x_analyze<= 8 && y_analyze>=7 && y_analyze<=8){
            break;
        }

        // convert information about orientation from (U, R, D, or L) to (0, 1, 2, or 3)
        int robot_path_orient;
        if(robot_path[y_analyze][x_analyze] == 'U'){robot_path_orient = 0;}
        else if(robot_path[y_analyze][x_analyze] == 'R'){robot_path_orient = 1;}
        else if(robot_path[y_analyze][x_analyze] == 'D'){robot_path_orient = 2;}
        else if(robot_path[y_analyze][x_analyze] == 'L'){robot_path_orient = 3;}
        
        updateCoordinates(x_analyze, y_analyze, robot_path_orient);
        
        count_cells++;
    }
    return count_cells;
}

void optimize_run_maze(int &x, int &y, int &orient, char (&robot_path)[16][16]){
    while(true){
        if(x>= 7 && x<= 8 && y>=7 && y<=8){
            log("Finish! Yeah!");
            break;
        }

        // convert information about orientation from (U, R, D, or L) to (0, 1, 2, or 3)
        int robot_path_orient;
        if(robot_path[y][x] == 'U'){robot_path_orient = 0;}
        else if(robot_path[y][x] == 'R'){robot_path_orient = 1;}
        else if(robot_path[y][x] == 'D'){robot_path_orient = 2;}
        else if(robot_path[y][x] == 'L'){robot_path_orient = 3;}

        // if current orientation (orient) is different from the desired orientation (robot_path_orient), keep turn right
        while(orient != robot_path_orient){
            API::turnRight();
            updateOrient('R', orient);
        }

        API::moveForward();
        updateCoordinates(x, y, orient);

    }
}

void run_maze(int info, int &x, int &y, int &orient, char (&robot_path)[16][16]){
    while (true) {
        // int w_left = API::wallLeft();
        // int w_right = API::wallRight();
        // int w_front = API::wallFront();
        
        cells_num_visited[y][x] += 1;

        if(info == 1){
            if(x>= 7 && x<= 8 && y>=7 && y<=8){
                log("Goal");
                update_robot_path(robot_path, x, y, orient);

                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        std::cerr << robot_path[i][j] << " ";
                    }
                    std::cerr << std::endl;
                }

                break;
            }
            else if (x == 5 && y == 9){
                log("Merit 1");
            }
        }
        else if(info == 2){
            if(x== 0 && y==15){
                log("Start");
                update_robot_path(robot_path, x, y, orient);

                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        std::cerr << robot_path[i][j] << " ";
                    }
                    std::cerr << std::endl;
                }

                break;
            }
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
            updateOrient('L', orient);
            update_robot_path(robot_path, x, y, orient);

            API::moveForward();
            updateCoordinates(x, y, orient);
        } else if (potentialTurn == 'F'){
            update_robot_path(robot_path, x, y, orient);

            API::moveForward();
            updateCoordinates(x, y, orient);
        } else if(potentialTurn == 'R'){
            API::turnRight();
            updateOrient('R', orient);
            update_robot_path(robot_path, x, y, orient);

            API::moveForward();
            updateCoordinates(x, y, orient);
        } else{
            API::turnRight();
            updateOrient('R', orient);

            API::turnRight();
            updateOrient('R', orient);
            update_robot_path(robot_path, x, y, orient);
        }

    }
}

int main(int argc, char* argv[]) {
    int x = 0;
    int y = 15;
    int orient = 0;
    int total_cells_path_1;
    int total_cells_path_2;

    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");

    API::setColor(5, 6, 'R');
    API::setText(5, 6, "M1");

    char robot_path_1[16][16];
    char robot_path_2[16][16];

    // 'N': Null, not travelled yet
    // 'U': move upward
    // 'R': move rightward
    // 'D': move downward
    // 'L': move leftward
    // initialize all the values in the array to 'N'
    memset(robot_path_1, 'N', sizeof(robot_path_1));

    // run_maze: running tremaux algorithm
    // run_maze parameter: (info, x, y, robot_path)
    // info = 1 indicates that the robot will run from the start zone to the goal zone.
    run_maze(1, x, y, orient, robot_path_1);
    total_cells_path_1 = analyze_maze_path(robot_path_1);
    std:: cerr << "total cells for path 1:" << total_cells_path_1 << std::endl;
    

    std::cerr << std::endl;
    
    // reset all the value in the array to 0
    memset(cells_num_visited, 0, sizeof(cells_num_visited));

    
    // 'N': Null, not travelled yet
    // 'U': move upward
    // 'R': move rightward
    // 'D': move downward
    // 'L': move leftward
    // initialize all the values in the array to 'N'
    memset(robot_path_2, 'N', sizeof(robot_path_2));
    // run_maze parameter: (info, x, y, robot_path)
    // info = 1 indicates that the robot will run from the start zone to the goal zone.
    run_maze(2, x, y, orient, robot_path_2);


    // the robot path need to be reversed because the robot is going back from goal zone to start zone
    // has problem
    // reverse_value_robot_path(robot_path_2);

    // total_cells_path_2 = analyze_maze_path(robot_path_2);
    // std:: cerr << "total cells for path 2:" << total_cells_path_2 << std::endl;

    optimize_run_maze(x, y, orient, robot_path_1);
    
}
