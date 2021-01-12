#pragma once

#include "opencv2/core.hpp"

struct TrainDetections
{
  cv::RotatedRect box;
  cv::Mat difference_image;
  cv::Mat thresholded_image;
};

std::vector<TrainDetections> detectTrainFromMotion(const std::vector<cv::Mat>& image_sequence);

cv::Mat computeMeanOverSequence(const std::vector<cv::Mat>& image_sequence);

TrainDetections detectDifferenceInImage(const cv::Mat& reference, const cv::Mat& image);

cv::Point2f computePointAtBottomOfBox(const cv::RotatedRect& box);

std::vector<cv::Mat> getTrainSequence(const std::string& path_to_directory);

