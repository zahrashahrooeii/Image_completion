#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
    // Load the original image
    Mat image = imread("image_to_complete.jpg");

    // Check if image is loaded fine
    if (image.empty()) {
        cout << "Error loading the main image" << endl;
        return -1;
    }

    // Load the patch images
    vector<Mat> patches;

    for (int i = 0; i < 3; i++) {
        Mat patch = imread("patch_" + to_string(i) + ".jpg");

        // Check if patch is loaded fine
        if (patch.empty()) {
            cout << "Error loading the patch " << i << endl;
            return -1;
        }

        patches.push_back(patch);
    }

    // Create SIFT object
    Ptr<SIFT> sift = SIFT::create();

    // Compute SIFT features for the main image
    vector<KeyPoint> keypointsImage;
    Mat descriptorsImage;
    sift->detectAndCompute(image, noArray(), keypointsImage, descriptorsImage);

    // Compute SIFT features for the patch images
    vector<vector<KeyPoint>> keypointsPatches(3);
    vector<Mat> descriptorsPatches(3);

    for (int i = 0; i < 3; i++) {
        sift->detectAndCompute(patches[i], noArray(), keypointsPatches[i], descriptorsPatches[i]);

        // Create a BFMatcher object
        BFMatcher matcher(NORM_L2);

        // Match keypoints and descriptors between the image and patch
        vector<DMatch> matches;
        matcher.match(descriptorsImage, descriptorsPatches[i], matches);

        // Find the minimum distance among the matches
        double minDistance = numeric_limits<double>::max();
        for (const auto& match : matches) {
            if (match.distance < minDistance)
                minDistance = match.distance;
        }

        // Define the ratio threshold
        double ratio = 28;

        // Refine the matches by selecting those with distance less than ratio * min_distance
        vector<DMatch> refinedMatches;
        for (const auto& match : matches) {
            if (match.distance < ratio * minDistance)
                refinedMatches.push_back(match);
        }

        // Display the number of matches and refined matches for each patch
        cout << "Patch " << i << ": " << matches.size() << " matches, " << refinedMatches.size() << " refined matches" << endl;

        // Find affine transformation using RANSAC and the refined matches
        vector<Point2f> imagePoints;
        vector<Point2f> patchPoints;

        for (const auto& match : refinedMatches) {
            imagePoints.push_back(keypointsImage[match.queryIdx].pt);
            patchPoints.push_back(keypointsPatches[i][match.trainIdx].pt);
        }

        if (imagePoints.size() >= 4 && patchPoints.size() >= 4) {
            Mat inliersMask;
            Mat affineTransform = findHomography(patchPoints, imagePoints, RANSAC, 3, inliersMask);

            // Calculate the number of inliers
            int inlierCount = countNonZero(inliersMask);
            cout << "Number of inliers for Patch " << i << ": " << inlierCount << endl;

            // Overlay the patch on the image using the affine transformation
            Mat warpedPatch;
            warpPerspective(patches[i], warpedPatch, affineTransform, image.size());

            // Create a mask for the patch region
            Mat patchMask = Mat::ones(patches[i].size(), CV_8U) * 255; // a mask filled with 255
            Mat warpedMask;
            warpPerspective(patchMask, warpedMask, affineTransform, image.size());

            // Copy the warped patch onto the image using the mask
            warpedPatch.copyTo(image, warpedMask);
        }

        else {
            cout << "Insufficient points for Homography calculation for Patch " << i << endl;
        }

    }

    // Display the final image with the patch overlay
    imshow("Image with Patch Overlay", image);
    waitKey(0);

    return 0;
}
