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
    if (xfront >= 0 && yfront >= 0 && flood[yfront][xfront] == val - 1) {
        //log("check if front is accessible");
        if (isAccessible(x, y, xfront, yfront)){ //if accessible, return 'F'
            return 'F';
            }
        else{ //if not accessible, flood is outdated
            log("front is not accessible");
            floodFill(x,y,xprev,yprev); //floodfill and repeat function
            showFlood();
            return toMove(x,y,orient,xprev,yprev);
        }   
    } else if (xright >= 0 && yright >= 0 && flood[yright][xright] == val - 1) { //turning right is a priority (will be checked before turning left)
        if (isAccessible(x, y, xright, yright)){
            return 'R';
            }
        else{
            floodFill(x,y,xprev,yprev);
            showFlood();
            return toMove(x,y,orient,xprev,yprev);
        }

    } else if (xleft >= 0 && yleft >= 0  && flood[yleft][xleft] == val - 1) {
        if (isAccessible(x, y, xleft, yleft)){
            return 'L';
            }
        else{
            floodFill(x,y,xprev,yprev);
            showFlood();
            return toMove(x,y,orient,xprev,yprev);
        }

    } else {
        return 'B'; //turn back if mouse runs into a dead end
        
    }
}