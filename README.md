# Image Completion Project

## Introduction
This project focuses on C++-based feature identification and image patch overlay on top of an original image. It leverages the OpenCV library for image processing tasks such as feature detection and homography transformation.

## Project Structure
- `Test1.cpp`: Main source code file containing the implementation of the project.
- `image_to_complete.jpg`: The original image to which patches will be applied.
- `patch_0.jpg`, `patch_1.jpg`, `patch_2.jpg`: Patch images that will be overlaid on the original image.
- `report.pdf`: Detailed report of the project.

## Features
1. **Importing Images**: 
   - Load the original image and three patch images from the preset directory.
2. **Feature Detection**: 
   - Use Scale-Invariant Feature Transform (SIFT) to detect and describe local features in the images.
3. **Key Point Matching**: 
   - Use Brute Force Matcher (BFMatcher) to match key points between the patch images and the original image.
   - Apply a ratio test to filter out poor matches.
4. **Homography and Patch Overlay**: 
   - Compute an affine transformation (homography) using RANSAC (RANdom SAmple Consensus) algorithm.
   - Adjust the patch perspective using warpPerspective and overlay it on the original image.

## Installation
To run this project, you need to have OpenCV installed. You can install OpenCV using the following commands:

```bash
# For Debian-based systems
sudo apt-get update
sudo apt-get install libopencv-dev

# For macOS
brew install opencv

