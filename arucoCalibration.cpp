/*

*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Mat frame;
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    cv::VideoCapture capIn(0);
    cv::VideoCapture capOut("");

    if (!capIn.isOpened())
    {
        cerr << "Error opening camera Stream !!" << endl;
        return -1;
    }

    while (true)
    {
        capIn >> imgIn;
        capOut >> imgOut;

        Mat frame = imgIn.clone();
        if (frame.empty())
        {
            cerr << "Error capturing frame !!" << endl;
            break;
        }

        vector<vector<Point2f>> corners;
        vector<int> ids;

        cv::aruco::detectMarkers(frame, dict, corners, ids);
        vector<Point2f> ptsout(4);

        int count;

        for (int i = 0; i < ids.size; i++)
        {
            if (id[i] == 0)
            {
                cnt++;
                int x = (corners[i][0].x + corners[i][2].x) / 2;
                int y = (corners[i][0].y + corners[i][2].y) / 2;
                ptsOut[i] = (cv::Point2f(x, y));
            }
            if (id[i] == 0)
            {
                cnt++;
                int x = (corners[i][0].x + corners[i][2].x) / 2;
                int y = (corners[i][0].y + corners[i][2].y) / 2;
                ptsOut[i] = (cv::Point2f(x, y));
            }
            if (id[i] == 0)
            {
                cnt++;
                int x = (corners[i][0].x + corners[i][2].x) / 2;
                int y = (corners[i][0].y + corners[i][2].y) / 2;
                ptsOut[i] = (cv::Point2f(x, y));
            }
            if (id[i] == 0)
            {
                cnt++;
                int x = (corners[i][0].x + corners[i][2].x) / 2;
                int y = (corners[i][0].y + corners[i][2].y) / 2;
                ptsOut[i] = (cv::Point2f(x, y));
            }
        }

        // input point
        cv::Point2f inPoints[4];
        inPoints[0] = cv::Point2f(0, 0);
        inPoints[1] = cv::Point2f(imgOut.cols, 0);
        inPoints[2] = cv::Point2f(0, imgOut.rows);
        inPoints[3] = cv::Point2f(imgOut.cols, imgOut.rows);

        // destination points
        cv::Point2f outPoints[4];
        outPoints[0] = ptsOut[0];
        outPoints[1] = ptsOut[1];
        outPoints[2] = ptsOut[2];
        outPoints[3] = ptsOut[3];

        cv::Mat matrix = cv::getPerspectiveTransform(inPoints, outPoints);
        cv::warpPerspective(imgOut, imgIn, matrix, imgIn.size());
        if (cnt == 4)
        {
            for (int i = 0; i < imgIn.cols; i++)
            {
                for (int j = 0; j < imgIn.rows; j++)
                {
                    if (imgIn.at<cv::Vec3b>(cv::Point(i, j))[0] <= 5 && imgIn.at<cv::Vec3b>(cv::Point(i, j))[1] <= 5 && imgIn.at<cv::Vec3b>(cv::Point(i, j))[2] <= 5)
                        imgIn.at<cv::Vec3b>(cv::Point(i, j)) = frame.at<cv::Vec3b>(cv::Point(i, j));
                }
            }

            imshow("Output", frame);

            if (waitKey(10) == 27)
            {
                break;
            }
        }

        return 0;
    }
}
