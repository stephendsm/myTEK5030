#pragma once

#include "opencv2/core.hpp"

struct Map
{
  cv::Mat map_image;
  cv::Matx33d S_map_grid;
};

std::vector<cv::Mat> getFlybySequence(const std::string& path_to_directory);

std::vector<cv::Matx33d> computeFrameToGridHomographies();

std::vector<cv::Matx33d> computeGridToFrameHomographies();

Map createMapFromFrames(
    const std::vector<cv::Mat>& frames,
    const std::vector<cv::Matx33d>& H_grid_frames,
    double map_scale,
    bool do_visualisation = false);
