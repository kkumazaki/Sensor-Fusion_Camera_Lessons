#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace std;

void magnitudeSobel()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");

    // convert image to grayscale
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // apply smoothing using the GaussianBlur() function from the OpenCV
    // ToDo : Add your code here
    cv::Mat resultGauss= imgGray.clone();
    //cv::GaussianBlur(imgGray, resultGauss, cv::Size(3,3), 3, 3);
    cv::GaussianBlur(imgGray, resultGauss, cv::Size(5,5), 2.0);

    // create filter kernels using the cv::Mat datatype both for x and y
    // ToDo : Add your code here
     // create filter kernel
    float sobelX[9] = {-1., 0, +1.,
                        -2., 0, +2.,
                        -1., 0, +1.};

     float sobelY[9] = {-1., -2., -1.,
                          0.,  0.,  0.,
                         +1., +2., +1.};                              

    // apply filter
    cv::Mat kernelX = cv::Mat(3, 3, CV_32F, sobelX);
    cv::Mat kernelY = cv::Mat(3, 3, CV_32F, sobelY);
   
    // apply filter using the OpenCv function filter2D()
    // ToDo : Add your code here
    cv::Mat resultSobelX;
    cv::Mat resultSobelY;

    cv::filter2D(resultGauss, resultSobelX, -1, kernelY, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);   
    cv::filter2D(resultGauss, resultSobelY, -1, kernelY, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);   
    //cv::filter2D(resultGauss, resultSobelX, -1, kernelX, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);   
    //cv::filter2D(resultSobelX, resultSobelY, -1, kernelY, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);     

    // compute magnitude image based on the equation presented in the lesson 
    // ToDo : Add your code here
    cv::Mat magnitude = imgGray.clone();

    //cout<<"colums:"<<resultSobelX.cols<<endl;
    //cout<<"rows:  "<<resultSobelX.rows<<endl;
    //cout<<"value: "<<resultSobelX.at<unsigned char>(5,5)<<endl;

    for(int r = 0; r < magnitude.rows; r++){
        for(int c = 0; c < magnitude.cols; c++){
            magnitude.at<uchar>(r,c) = sqrt( pow(resultSobelX.at<unsigned char>(r,c),2) 
                                          +  pow(resultSobelY.at<unsigned char>(r,c),2) );
//            magnitude.at<uchar>(r,c) = sqrt( pow((resultSobelX.at<unsigned char>(r+1,c)-resultSobelX.at<unsigned char>(r,c)),2) 
//                                          +  pow((resultSobelY.at<unsigned char>(r,c+1)-resultSobelY.at<unsigned char>(r,c)),2) );
        }
    }

    // show result
    string windowName = "Gaussian Blurring";
    cv::namedWindow(windowName, 1); // create window
    //cv::imshow(windowName, resultSobelX);
    cv::imshow(windowName, magnitude);
    cv::waitKey(0); // wait for keyboard input before continuing
}

int main()
{
    magnitudeSobel();
}