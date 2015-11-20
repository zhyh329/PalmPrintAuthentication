#include "BoundaryTracking.h"
#include <iostream>

using namespace std;
using namespace cv;


int BoundaryTracking::getNeighborhoodSearchIndex(int dir){
	if (dir % 2 == 0){
		return (dir + 7) % 8;
	}
	else{
		return (dir + 6) % 8;
	}
}

Point BoundaryTracking::getNextBoundaryPoint(const Mat &img, int &dir, Point currentPoint){
	// init directionPoints
	vector<Point> directionPoints;

	directionPoints.push_back(Point(1, 0));
	directionPoints.push_back(Point(1, 1));
	directionPoints.push_back(Point(0, 1));
	directionPoints.push_back(Point(-1, 1));

	directionPoints.push_back(Point(-1, 0));
	directionPoints.push_back(Point(-1, -1));
	directionPoints.push_back(Point(0, -1));
	directionPoints.push_back(Point(1, -1));
	
	Point nextPoint(0,0);
	

	int startIndex = getNeighborhoodSearchIndex(dir);
	for (int i = startIndex; i < startIndex + 8; ++i){
		Point neighborPoint = currentPoint + directionPoints.at(i % 8);

		if (neighborPoint.y >= 0 && neighborPoint.y < img.rows && neighborPoint.x >= 0 && neighborPoint.x < img.cols){
			
			if(img.at<uchar>(neighborPoint.y, neighborPoint.x) == 255){
				nextPoint = neighborPoint;
				dir = i % 8;
				break;
			}
		}
	}

	return nextPoint;
}

vector<Point> BoundaryTracking::getBoundary(const Mat &img){

	vector<Point> boundaryVector;


	Point startingPoint;
	Size size = img.size();
	
	for (int y = 0; y < size.height; ++y){
		if (img.at<uchar>(y, 0) == 255){
			startingPoint.x = 0;
			startingPoint.y = y;
			break;
		}	
	}

	boundaryVector.push_back(startingPoint); 
	Point currentPoint = startingPoint;
	int dir = 7;

	for (int i = 0; i < 500;++i){
		currentPoint = getNextBoundaryPoint(img, dir, currentPoint);

		cout << "Dir: " << dir << endl;
		boundaryVector.push_back(currentPoint);

	}

	return boundaryVector;
}