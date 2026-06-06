// hand_gesture.cpp - Hand Gesture Recognition for PowerPoint Control
// Compile: g++ hand_gesture.cpp -o hand_gesture -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
// Run: ./hand_gesture

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

// Function to move cursor
void MoveCursor(int x, int y) {
    SetCursorPos(x, y);
}

// Function to simulate key press (for PowerPoint)
void PressKey(int key) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

// Function to simulate left click
void LeftClick() {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

int main(int argc, const char** argv)
{
    // Open webcam
    VideoCapture cam(0);
    if(!cam.isOpened()){
        cout << "ERROR: Camera cannot be opened" << endl;
        return -1;
    }
    
    Mat img, img_gray, img_threshold;
    namedWindow("Hand Gesture Control", CV_WINDOW_AUTOSIZE);
    
    cout << "
========================================" << endl;
    cout << "  HAND GESTURE VIRTUAL MOUSE/PRESENTATION" << endl;
    cout << "========================================" << endl;
    cout << "GESTURES:" << endl;
    cout << "  1 Finger  = Previous Slide / Left" << endl;
    cout << "  2 Fingers = Next Slide / Right" << endl;
    cout << "  3 Fingers = Click/Select" << endl;
    cout << "  4 Fingers = Down Arrow" << endl;
    cout << "  5 Fingers = Up Arrow" << endl;
    cout << "  0 Fingers = Close/Exit" << endl;
    cout << "========================================" << endl;
    cout << "Press ESC to quit
" << endl;
    
    while(1){
        bool b = cam.read(img);
        if(!b){
            cout << "ERROR: Cannot read video" << endl;
            return -1;
        }
        
        // Convert to grayscale
        cvtColor(img, img_gray, CV_RGB2GRAY);
        
        // Apply Gaussian blur
        GaussianBlur(img_gray, img_gray, Size(19, 19), 0, 0);
        
        // Threshold to get hand
        threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
        
        // Find contours
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(img_threshold, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
        
        int fingerCount = 0;
        
        if(contours.size() > 0){
            // Find biggest contour (hand)
            size_t indexOfBiggestContour = -1;
            size_t sizeOfBiggestContour = 0;
            
            for (size_t i = 0; i < contours.size(); i++){
                if(contours[i].size() > sizeOfBiggestContour){
                    sizeOfBiggestContour = contours[i].size();
                    indexOfBiggestContour = i;
                }
            }
            
            // Get convex hull
            vector<Point> hull;
            convexHull(contours[indexOfBiggestContour], hull, true);
            
            // Get convexity defects (count fingers)
            vector<Vec4i> defects;
            convexityDefects(contours[indexOfBiggestContour], hull, defects);
            
            // Count fingers
            for(size_t k = 0; k < defects.size(); k++){
                if(defects[k][3] > 13 * 256){
                    fingerCount++;
                }
            }
            
            // Gesture control logic
            char gestureText[50];
            
            if(fingerCount == 1){
                strcpy(gestureText, "ONE - PREVIOUS SLIDE");
                PressKey(VK_LEFT);  // Previous slide
                Sleep(500);  // Delay to prevent multiple presses
            }
            else if(fingerCount == 2){
                strcpy(gestureText, "TWO - NEXT SLIDE");
                PressKey(VK_RIGHT);  // Next slide
                Sleep(500);
            }
            else if(fingerCount == 3){
                strcpy(gestureText, "THREE - CLICK");
                LeftClick();
                Sleep(500);
            }
            else if(fingerCount == 4){
                strcpy(gestureText, "FOUR - DOWN ARROW");
                PressKey(VK_DOWN);
                Sleep(500);
            }
            else if(fingerCount == 5){
                strcpy(gestureText, "FIVE - UP ARROW");
                PressKey(VK_UP);
                Sleep(500);
            }
            else{
                strcpy(gestureText, "ZERO - WAITING");
            }
            
            // Display gesture text
            putText(img, gestureText, Point(50, 50), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2, 8, false);
            
            // Draw hand contour
            drawContours(img, contours, indexOfBiggestContour, Scalar(255, 255, 0), 2, 8);
            
            // Draw convex hull
            drawContours(img, hull, -1, Scalar(0, 255, 0), 1, 8);
        }
        
        // Show frame
        imshow("Hand Gesture Control", img);
        
        // ESC to quit
        if(waitKey(30) == 27){
            cout << "
Exiting..." << endl;
            return 0;
        }
    }
    
    return 0;
}
