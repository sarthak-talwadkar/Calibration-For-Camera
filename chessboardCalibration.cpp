/*
    Sarthak Uday Talwadkar
    March 4, 2026
    Calibration of camera using Chessboard pattern
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

/*
    The pattern is defined as Number of Horizontal rows and Vertical columns
    It will be used later to find the chessboard pattern in video stream
    Once the enough calibration data is found then reprojection error, camera matrix and distortion coefficient is calculated
    using OpenCV calibrateCamera function
    It is also saved in a yml file once calculated
*/
int main(int argc, char *argv[])
{
    // Type of pattern defination
    int cols = 9;
    int rows = 6;
    Size patternSize(cols, rows);
    Size imageSize;

    // Coordinates for chessboard inner corners, global coordinates and calibration image for saving and later on used for calibration
    vector<vector<Point2f>> cornerLists;
    vector<vector<Vec3f>> pointLists;
    vector<Vec3f> pointSet;
    vector<Mat> savedImage;

    // Defining the calibration parameters
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distortionCoeficients;
    vector<Mat> rotationalVectors, translationVectors;
    double reprojectionError = 0;

    // Setting Global coordinates
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pointSet.push_back(Vec3f(i, -j, 0));
        }
    }

    // Video stream
    VideoCapture cap(2);
    if (!cap.isOpened())
    {
        cerr << "Error opening capture device" << endl;
        return -1;
    }

    // Defining the feature and Mat for saving and calculations and saving the frame
    Mat frame, gray;
    vector<Point2f> lastCorners;
    Mat lastImage;
    int saveCount = 0;
    bool calibration = false;
    int i = 0;

    // Loop start
    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            cerr << "Blank frame" << endl;
            break;
        }

        if (imageSize == Size())
        {
            imageSize = frame.size();
        }

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        /*
            Finding the chessboard pattern
            If found then calculating of coordinates for the current frame and drawing corner marker
        */
        bool foundCorners = findChessboardCorners(gray, patternSize, lastCorners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);

        if (foundCorners)
        {
            cornerSubPix(gray, lastCorners, Size(11, 11),
                         Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.0001));

            lastImage = frame.clone();

            drawChessboardCorners(frame, patternSize, Mat(lastCorners), foundCorners);

            /*if (!lastCorners.empty())
            {
                cout << "First corner coordinates (x, y): "
                     << lastCorners[0].x << ", " << lastCorners[0].y << endl;
            }*/

            string status = "Corners Detected : " + to_string(lastCorners.size());
            putText(frame, status, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
        }

        if (calibration)
        {
            putText(frame, "Error: " + to_string(reprojectionError), Point(20, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 0), 2);
        }

        imshow("Chessboard corners", frame);

        /*
            If user presses s it will save the current frame and corner data
            If user presses c it will calibrate the camera and calculates the data
            If user presses w it will write the data to Calibration.yml file
            If user presses Esc it will exit
        */
        char key = waitKey(10);
        if (key == 27)
            break;
        else if (key == 's')
        {
            if (!lastCorners.empty())
            {
                cornerLists.push_back(lastCorners);
                pointLists.push_back(pointSet);
                savedImage.push_back(lastImage);

                string name = "calibrated_image_" + to_string(i) + to_string(lastCorners.size()) + ".jpg";
                imwrite(name, lastImage);
                i++;

                cout << "Saved Calibration Frame ...." << endl;
            }
            else
            {
                cout << "No coner points found!!" << endl;
            }
        }
        else if (key == 'c')
        {
            if (cornerLists.size() < 5)
            {
                cout << "Need atleast 5 calibration frame !!" << endl;
                continue;
            }
            cameraMatrix.at<double>(0, 2) = imageSize.width / 2.0;
            cameraMatrix.at<double>(1, 2) = imageSize.height / 2.0;

            int flags = CALIB_FIX_ASPECT_RATIO | CALIB_FIX_PRINCIPAL_POINT;
            cout << "\nInitial Camera Matrix:\n"
                 << cameraMatrix << endl;

            reprojectionError = calibrateCamera(pointLists, cornerLists, imageSize, cameraMatrix, distortionCoeficients, rotationalVectors, translationVectors, flags);

            cout << "\nCalibration Results:" << endl;
            cout << "Reprojection error: " << reprojectionError << endl;
            cout << "Camera Matrix:\n"
                 << cameraMatrix << endl;
            cout << "Distortion Coefficients: " << distortionCoeficients << endl;

            calibration = true;
        }
        else if (key == 'w' && calibration)
        {
            FileStorage fs("Calibration.yml", FileStorage::WRITE);
            fs << "cameraMatix" << cameraMatrix;
            fs << "distortionCoeficients" << distortionCoeficients;
            fs << "reprojectionError" << reprojectionError;
            fs.release();
            cout << "Parameters saved to Calibration.yml" << endl;
        }
    }
    return 0;
}