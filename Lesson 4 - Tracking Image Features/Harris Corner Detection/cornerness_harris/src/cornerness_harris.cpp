#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    // visualize results
    //string windowName = "Harris Corner Detector Response Matrix";
    //cv::namedWindow(windowName, 4);
    //cv::imshow(windowName, dst_norm_scaled);
    //cv::waitKey(0);

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.

    
    // Try#1
    // Parameters for Histogram
    /*
    int image_num = 1;      // How many images'
    int channels[] = { 0 }; // Which channel (Grayscale, it's zero)  
    cv::MatND hist;         // Histogram result
    int dim_num = 1;        // dimension of histogram
    int bin_num = 256;       // bin number of histogram
    int bin_nums[] = { bin_num };      // number of histogram
    float range[] = { 0, 256 };        // range of histogram [0, 255]
    const float *ranges[] = { range }; // dimension of range

    cv::calcHist(&dst_norm_scaled, image_num, channels, cv::Mat(), hist, dim_num, bin_nums, ranges);
    for (int i = 0; i < bin_num; i++){
        std::cout << i << ": " << hist.at<float>(i) << std::endl;
    }
    */

    // Result:  Most recent value: 72 gray points.
    //          More than 100 values, the points number are less than 20.
    
    
    // Try#2
    // Find points whose values are more than thresohld and push back to the vector.
    // However, if there are lower value points inside apertureSize area(3*3), remove higher value points.

    // result vector
    vector<cv::KeyPoint> keypoints;

    // remove the unnecessary points on 4 border lanes to make calculation easier
    int count = 0;
    int neighber = 3; // parameter to neglect the lower value point in this range

    for(int r = neighber; r < dst_norm.rows-neighber; r++){
        for(int c = neighber; c < dst_norm.cols-neighber; c++){
    //for(int r = 1; r < dst_norm.rows-1; r++){
    //    for(int c = 1; c < dst_norm.cols-1; c++){
            int response = (int)dst_norm.at<float>(r, c);
            if (response > minResponse){
                /*
                if (((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r-1,c-1)) && 
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r-1,c))   &&
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r-1,c+1)) &&
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r,c-1))   &&
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r,c+1))   &&
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r+1,c-1)) &&
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r+1,c))   &&
                    ((int)dst_norm.at<float>(r,c) > (int)dst_norm.at<float>(r+1,c+1)) 
                ){
                */
                int comp_cnt = 0;
                int comp_thresh = (2 * neighber +1) * (2 * neighber +1) -1;
                for (int i = -neighber; i <= neighber; i++){
                    for (int j = -neighber; j <= neighber; j++){
                        if (response > (int)dst_norm.at<float>(r+i,c+j)){
                            comp_cnt += 1;
                        }
                    }
                }

                //cout << comp_cnt << endl;

                if (comp_cnt == comp_thresh){
                    cv::KeyPoint newKeyPoint;
                    newKeyPoint.pt = cv::Point2f(c, r);
                    //newKeyPoint.pt = cv::Point2f(r, c);
                    newKeyPoint.size = 2 * apertureSize;
                    keypoints.push_back(newKeyPoint);
                    //cout << cv::Point2f(r, c) << endl;
                    count += 1;
                }                  
                
            }
        }
    }
    cout << "count: " << count << endl;
    cout << "rows:  " << dst_norm_scaled.rows << endl;
    cout << "cools: " << dst_norm_scaled.cols << endl;

    // visualize keypoints
    string windowName = "Harris Corner Detection Results";
    cv::namedWindow(windowName, 5);
    cv::Mat visImage = dst_norm_scaled.clone();
    cv::drawKeypoints(dst_norm_scaled, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imshow(windowName, visImage);
    cv::waitKey(0);
    // EOF STUDENT CODE

}

int main()
{
    cornernessHarris();
}