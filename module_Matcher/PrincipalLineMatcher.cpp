#include <module_Matcher/PrincipalLineMatcher.h>
#include <module_FeatureExtraction/PrincipalLineFeature.h>
#include <easylogging++.h>
#include <utility>
PrincipalLineMatcher::PrincipalLineMatcher()
{

}


Mat PrincipalLineMatcher::doDistanceTransformation(Mat img){

	Mat invImg = Scalar::all(255) - img;


	for (int x = 1; x < invImg.rows - 1; ++x){
		for (int y = 1; y < invImg.cols - 1; ++y){

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y - 1) + sqrt(2)) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y - 1) + sqrt(2));

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x, y - 1) + 1) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x, y - 1) + sqrt(2));

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y - 1) + sqrt(2)) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y - 1) + sqrt(2));

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y) + 1) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y) + 1);
		}
	}

	for (int x = invImg.rows - 2; x >= 1; --x){
		for (int y = invImg.cols - 2; y >= 1; --y){

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y) + 1) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y) + 1);

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y + 1) + sqrt(2)) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y + 1) + sqrt(2));

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x, y + 1) + 1) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x, y + 1) + 1);

            if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y + 1) + sqrt(2)) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y + 1) + sqrt(2));
		}
	}

	return invImg;
}

pair<double,int> PrincipalLineMatcher::doMatching(IFeature* f){

    Mat imgDT = doDistanceTransformation(f->getFeature());

    dbAdapter = new DbAdapter();

    vector<pair<int, vector<Point>>> storedFeatures = dbAdapter->getLineFeatures();

    double min = 10000000;
	int min_id = 0;
    int matchedIndex = -1;
    double sum = 0;
	for (int i = 0; i < storedFeatures.size(); ++i){
		sum = 0;
		vector<Point> temp = storedFeatures.at(i).second;
        for (int j = 0; j < temp.size(); ++j){
			sum += imgDT.at<uchar>(temp.at(j).x, temp.at(j).y);
		}

        sum = sum / temp.size();
		if (sum < min){
			min = sum;
			min_id = storedFeatures.at(i).first;
            matchedIndex = i;
		}
		
	}

    Mat matchedImage(f->getFeature().size(),CV_8UC1, Scalar(0,0,0));

    for(Point p : storedFeatures.at(matchedIndex).second){
        matchedImage.at<uchar>(p.x,p.y) = 255;
    }

    matchedFeature = new PrincipalLineFeature(matchedImage);

    return std::make_pair(min,min_id);
}

IFeature* PrincipalLineMatcher::getMatchedFeature(){
    return matchedFeature;
}

PrincipalLineMatcher::~PrincipalLineMatcher()
{
	delete dbAdapter;
    delete matchedFeature;
}
