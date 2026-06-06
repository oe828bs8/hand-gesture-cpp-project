# Hand Gesture Virtual Mouse (C++)

## Project Overview
Control PowerPoint presentations and mouse cursor using hand gestures.

## Features
- Real-time hand gesture recognition
- Control PowerPoint slides with fingers
- Virtual mouse functionality
- No Python or JavaScript required
- Pure C++ with OpenCV

## Gestures
| Finger Count | Action |
|--------------|--------|
| 1 finger | Previous slide |
| 2 fingers | Next slide |
| 3 fingers | Click |
| 4 fingers | Down arrow |
| 5 fingers | Up arrow |
| 0 fingers | Waiting |

## Requirements
- OpenCV 4.x
- C++11 or higher
- Webcam
- Windows (for mouse control)

## Compile
```bash
g++ hand_gesture.cpp -o hand_gesture -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
