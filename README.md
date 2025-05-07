# Calibration-For-Camera
Camera Calibration using OpenCV

chessboardCalibration.cpp

    Purpose: Camera calibration using a 9x6 chessboard pattern.

    Features:

        Real-time chessboard corner detection and refinement.
        Interactive calibration workflow:
            Press s to save calibration frames.
            Press c to compute camera matrix, distortion coefficients, and reprojection error.
            Press w to save calibration data to Calibration.yml.
        Displays real-time corner detection status and reprojection error.

    Usage:
        Requires a physical chessboard.
        Collect at least 5 calibration frames for accurate results.

arucoCalibration.cpp

    Purpose: ArUco marker detection and image warping for AR applications.

    Features:
        Detects predefined ArUco markers (DICT_6X6_250).
        Computes perspective transformation to overlay an external image (imgOut) onto detected markers.
        Basic blending to preserve original pixels in dark regions.

    Setup:
        Replace capOut("") with a valid video/image source for imgOut.

Dependencies

    ArUco Module: Ensure OpenCV is built with opencv_contrib modules.

Notes

    ArUco Calibration (Work in Progress):
    The arucoCalibration.cpp code is experimental and needs debugging. Contributions welcome!
    Use chessboardCalibration.cpp to generate Calibration.yml for other projects.
