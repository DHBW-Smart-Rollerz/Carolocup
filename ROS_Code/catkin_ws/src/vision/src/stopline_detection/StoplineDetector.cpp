#include  <vision/stopline_detection/StoplineDetector.hpp>


using namespace cv;
using namespace std;

double roiHeight;
double roiWidth;
cv::Mat cdst;
vector<Vec4i> perpendicularLines;
vector<Vec4i> lines;
static int validation = 0;

//config details
double ipmScaling;
double ipmSizeX;
double ipmSizeY;

void StoplineDetector::failureReport(string i) {
    cout << "Failed due to: " << i << endl;
}

void StoplineDetector::drawLine (vector<Vec4i> lines) {
    
    for( size_t i = 0; i < lines.size(); i++ )
      {
          Vec4i l = lines[i];
          //cout << "P1: " << Point(l[0], l[1]) << " p" << Point(l[2], l[3]) << endl;
          line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
      }
    
    imshow("detected lines", cdst);
    waitKey();
}

double StoplineDetector::getDistanceBetweenPoints(Point p1, Point p2) {
    return abs(p1.y-p2.y);
}

//Check if the line is a stopping or perpendicular line
vector<Vec4i> StoplineDetector::getPossibleEventLine(vector<Vec4i> & detectedLines) {
    vector<Vec4i> possibleStoppLine;
    Vec4i l;
    Point p1, p2;
    float angle;
    
    for( size_t i = 0; i < detectedLines.size(); i++ )
    {
	
        l = detectedLines[i];
        // draw the lines

        p1=Point(l[0], l[1]);
        p2=Point(l[2], l[3]);
        //calculate angle in radian,  if you need it in degrees just do angle * 180 / PI
        angle = abs(atan2(p1.y - p2.y, p1.x - p2.x) * (180/CV_PI));
        
        if (angle > 165 || angle < 15) {
            possibleStoppLine.push_back(detectedLines.at(i));
        }
        
        if (angle < 100 && angle > 80) {
            perpendicularLines.push_back(detectedLines.at(i));
        }
	std::cout << "Winkel spur " << i << " : " << angle << std::endl;
    }
    
    return possibleStoppLine;
}

vector<Vec4i> StoplineDetector::checkLineWidth (vector<Vec4i> lines) {
        Vec4i l1;
        Vec4i l2;
        double distance1;
        double distance2;
        double distance3;
        double distance4;
        vector<Vec4i> resultingLines;
        size_t i = 0;
        Point p1;
        Point p2;
        Point p3;
        Point p4;
        for (size_t q = 0; q < lines.size()-1; q++) {
            l1 = lines[q];
            p1 = Point(l1[0], l1[1]);
            p2 = Point(l1[2], l1[3]);
            i++;
            for(;i < lines.size(); i++ )
              {
                  l2 = lines[i];
                  p3 = Point(l2[0], l2[1]);
                  p4 = Point(l2[2], l2[3]);
                  distance1 = getDistanceBetweenPoints(p1, p3);
                  distance2 = getDistanceBetweenPoints(p1, p4);
                  distance3 = getDistanceBetweenPoints(p2, p3);
                  distance4 = getDistanceBetweenPoints(p2, p4);
                  cout << "d1: " << distance1 << "  d2: " << distance2 << endl;
                  if ((distance1 > 30 && distance1 < 100) || (distance2 > 30 && distance2 < 100) || (distance3 > 30 && distance3 < 100) || (distance4 > 30 && distance4 < 100)) {
                      resultingLines.push_back(lines[q]);
                      resultingLines.push_back(lines[i]);
                  }
                  
              }
        }
    //    drawLine(resultingLines);
        return resultingLines;
}

bool StoplineDetector::lineIsWithinArea(Vec4i line, Point carCenter) {
    if ((abs(line[0]-carCenter.x) < 200 && abs(line[1]-roiHeight) < 1500)  || (abs(line[2]-carCenter.x) < 200 && abs(line[3]-roiHeight) < 1500)) {
        return true;
    }
    
    return false;
}

 Vec4i StoplineDetector::getStopLine (vector<Vec4i> lines) {
    Vec4i i;
    vector<double> distance;
    
    Vec4i l1;
    Point p1;
    Point p2;
    Point carCenter(roiWidth/2, roiHeight);

     
    double distance1;
    double distance2;
    double min = 0;
     
    
    Vec4i resultLine;
    
    resultLine = lines[0];
     
     if (lines.size() == 1 ) {
         if (lineIsWithinArea(resultLine, carCenter)) {
             return resultLine;
         } else {
             return {};
         }
         
     } else {
     
        p1 = Point(resultLine[0], resultLine[1]);
        p2 = Point(resultLine[2], resultLine[3]);
         
         if (getDistanceBetweenPoints(p1, carCenter) < getDistanceBetweenPoints(p2, carCenter)) {
             min = getDistanceBetweenPoints(p1, carCenter);
         } else{
             min = getDistanceBetweenPoints(p2, carCenter);
         }

         
        for (size_t q = 1; q < lines.size(); q++) {
            l1 = lines[q];
            p1 = Point(l1[0], l1[1]);
            p2 = Point(l1[2], l1[3]);
            
            distance1 = getDistanceBetweenPoints(p1, carCenter);
            distance2 = getDistanceBetweenPoints(p2, carCenter);
            
            
            if (distance1 < min && lineIsWithinArea(l1, carCenter)){
                min = distance1;
                resultLine = l1;
            }
            if (distance2 < min && lineIsWithinArea(l1, carCenter)) {
                min = distance2;
                resultLine = l1;
            }
        }
     }
     
//	cout << "Yposition: " << abs(resultLine[1]-roiHeight) << " Xpos: " << abs(resultLine[0]-carCenter.x) <<endl;
     if (lineIsWithinArea(resultLine, carCenter)) {
         return resultLine;
     }else {
         return {};
     }
}

vector<Vec4i> StoplineDetector::checkOrthogonal(vector<Vec4i> lines, vector<Vec4i> pLines) {
    vector<Vec4i> resultingLines;
    Vec4i l;
    Vec4i p;
    int m1, m2;
    int x1, x2, x3, x4;
    int y1, y2, y3, y4;

    float angle;
    
    size_t i = 0;
    for (size_t q = 0; q < pLines.size(); q++) {
        p = pLines[q];
        for(size_t i = 0;i < lines.size(); i++ )
        {
            l = lines[i];
            x1 = l[0];
            y1 = l[1];
            x2 = l[2];
            y2 = l[3];
            
            x3 = p[0];
            y3 = p[1];
            x4 = p[2];
            y4 = p[3];
            
            angle = abs(atan2(y2-y1, x2-x1) - atan2(y4-y3,x4-x3));
            
            angle = angle *180/CV_PI;
	    //cout << "orth: " << angle << endl;
	    //cout << "x1: "  << x1 << " x2: " << x2 << " x3: " << x3 << " x4: " << x4 << endl; 
            if (angle > 80 && angle < 100 && (x1 < x3 || x2 < x4)) {
                resultingLines.push_back(lines[i]);
            }
        }
    }
    
    return resultingLines;
}

double calculateDistance(Vec4i stoppLine) {
    
    return (double)((ipmSizeY-stoppLine[1])/ipmScaling);
}

int decreaseValidation(int validation) {
    if(validation > 0 )
        validation--;
    
    return validation;
}

void readConfigDetails(std::map<std::string, std::string> config) {
    ipmSizeX         =     config.count("ipm_Size_x")          ? stod(config["ipm_Size_x"])        : 1200;
    ipmSizeY         =     config.count("ipm_Size_y")          ? stod(config["ipm_Size_y"])        : 2400;
    ipmScaling             =     config.count("ipm_scaling")             ? stod(config["ipm_scaling"])           : 1;
}
//call by reference
double StoplineDetector::detect(const vision::HoughPointsArray::ConstPtr &imageArray, int pointerSize, std::map<std::string, std::string> config) {
    cout << "-----------------------New Message---------------------------------" << endl;
    //read config details
    readConfigDetails(config);
    
    roiHeight = ipmSizeY;
    roiWidth = ipmSizeX*3/4;
    
	vector<Vec4i> lines;
	Vec4i l;
	for(int i = 0; i < pointerSize; i++){
		const vision::HoughPoints &data = imageArray->points[i];
		l[0] = data.x1;
		l[1] = data.y1;
		l[2] = data.x2;
		l[3] = data.y2;
		lines.push_back(l);
	} 
 
	//std::cout << "Detected " << lines.size() << " line(s)" << std::endl;
    if (!lines.empty()) {
	//std::cout << "Suche winkel" << std::endl;
        lines = getPossibleEventLine(lines);
    }
    else {
        decreaseValidation(validation);
        return -1.0;
    }
    
    //eigentlich kann man wenn hier nichts gefunden wird sofort abbrechen !
    if (!lines.empty()) {
        lines = checkLineWidth(lines);
    }
    else{
        decreaseValidation(validation);
        return -1.0;
    }
    
//    Check if stoppline is orthogonal to perpendicular
    if (!lines.empty()) {
        lines = checkOrthogonal(lines, perpendicularLines);
    } else{
        decreaseValidation(validation);
        return -1.0;
    }
    
    Vec4i stoppLine;
    if (!lines.empty()) {
        stoppLine = getStopLine(lines);
    }
    else{
        decreaseValidation(validation);
        return -1.0;
    }
    
    if (stoppLine[0] != 0 && stoppLine[1] != 0 && stoppLine[2] != 0 && stoppLine[3] != 0) {
        validation++;
    }else {
        decreaseValidation(validation);
        return -1.0;
    }

	cout << "validation: " << validation << endl;
    if (validation > 2) {
        return calculateDistance(stoppLine);
    }
    
    return -1.0;
}
