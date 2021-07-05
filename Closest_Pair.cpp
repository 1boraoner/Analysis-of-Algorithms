double distance_calculate(Point p1, Point p2) {         //calculates the distance between two given points
    return sqrt(pow((p1.xCoordinate - p2.xCoordinate), 2) + pow((p1.yCoordinate - p2.yCoordinate), 2));
}

double FindClosestPairDistance() {      //returns the minimum distance among the list of points

    vector<Point> points_x = points;    //sorted list in terms of x-coordinates of the Points
    vector<Point> points_y = points;    //sorted list in terms of y-coordinates of the Points

    sort(points_x.begin(), points_x.end(),x_compare); //the points are sorted in terms of their x-coordinates 
    sort(points_y.begin(), points_y.end(), y_compare);  //sorting the list looking at the y-coordiantes

    double closest_distance = closest_points(points_x,points_y);   //minimum distance that is calculated
    return closest_distance;   //return the minimum distance
}

double closest_points(vector<Point> Px,vector<Point> Py) {

    if (Px.size() <= 3 && Py.size() <= 3) { //if total points are less than 3 brute force the closest points

        double distance1 = 10000;    //initial distance is INF for first partition
        double distance2 = 10000;    //initial distance is INF for second partition
        for (unsigned int i = 0; i < Px.size(); i++) {   //iterating over all elements

            for (unsigned int j = i+1; j < Px.size(); j++) { //not including the point that is selected in the outer loop

                double d_inter1 = distance_calculate(Px[i], Px[j]);  //calculate distance in order to compare
                if (d_inter1 < distance1) {   //if the two points are closer than the other
                    distance1 = d_inter1;     //assign the new distance
                }

                double d_inter2 = distance_calculate(Py[i], Py[j]);  //calculate distance in order to compare
                if (d_inter2 < distance2) {   //if the two points are closer than the other
                    distance2 = d_inter2;     //assign the new distance
                }
            }
        }

        return min(distance1,distance2);    //return the min distance among the Px and Py lists points
    }

    //Qx, Qy and Rx, Ry
    vector<Point> Qx;   //left split of the Px list
    vector<Point> Qy;   //left split of the Py list
    vector<Point> Rx;   //right split of the Px list
    vector<Point> Ry;   //right split of the Py list

    for (int i = 0; i< (int)Px.size(); i++) {  //construction of the 4 vectors

        if (i < (int)Px.size()/2 || ((i%2 !=0) && (i == (int)Px.size()/2))) {   //the splipt is made from the half
            Qx.push_back(Px[i]);    //append element into Qx
            Qy.push_back(Py[i]);    //append element into Qy
        }
        else {
            Rx.push_back(Px[i]);    //append Rx
            Ry.push_back(Py[i]);    //append Ry
        }
    }

    double dq_points = closest_points(Qx, Qy);  //minimum distance comes from the left sublist
    double dr_points = closest_points(Rx, Ry);  //minimum distance comes from the right sublist
    double min_partition = min(dq_points, dr_points);   //the minimum of the minimums calculated

    Point L_point = Qx[Qx.size()-1];                //the point where the partition is made (the element which is at the half of the list)

    vector<Point> set_s;                            //the set where the points that are within the minimmum distance around the partition element
    for (unsigned int i = 0; i < Py.size();i++) {   //scaning the Py list 

        if (L_point.xCoordinate-min_partition < Py[i].xCoordinate && L_point.xCoordinate + min_partition > Py[i].xCoordinate) { // inside the parition points are added
            set_s.push_back(Py[i]);                 //the points are appended in y increasing order
        }
    }

    double s_min = 1000;    //the initial distance in the set is INF
    for (unsigned int i = 0; i < set_s.size(); i++) {   //for each element in the set

        int edge_boundry = set_s.size();    // the default comparison is done with set size where set size < 15
        if (set_s.size() - i >= 15) {       // if the set size is big than the next 15 elements will be brute force to calculate distance 
            edge_boundry = i + 15;          // the next 15 elements is the edge boundry for the comparions points
        }

        for (int j = i+1; j < edge_boundry; j++) { //scanning thourgh the consequtive points (for 15 points or the less than 15)

            double d_inter1 = distance_calculate(set_s[i], set_s[j]);   //calculate distance in order to compare
            if (d_inter1 < s_min) {                                     //if the two points are closer than the other
                s_min = d_inter1;                                       //assign the new distance
            }
        }
    }
    return s_min < min_partition ? s_min :min_partition;    // cheking if the minimum distance is between 
}
 
