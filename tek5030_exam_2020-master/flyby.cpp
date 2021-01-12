#include "flyby.h"
#include "flyby_correspondences.h"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

std::vector<cv::Mat> getFlybySequence(const std::string& path_to_directory)
{
  const int num_frames = 4;
  std::vector<cv::Mat> image_sequence(num_frames);
  for (int i=0; i<num_frames; ++i)
  {
    std::stringstream filename;
    filename << path_to_directory << "/flyby_frame_" << i << ".png";
    image_sequence[i] = cv::imread(filename.str());

    if (image_sequence[i].empty())
    {
      throw std::runtime_error("Could not open file \"" + filename.str() + "\"");
    }
  }

  return image_sequence;
}

std::vector<cv::Matx33d> computeFrameToGridHomographies()
{
  const auto correspondences = getFlybyCorrespondences();

  std::vector<cv::Matx33d> homographies;
  for (const auto& corr : correspondences)
  {
    homographies.push_back(cv::findHomography(corr.image_points, corr.grid_points));
  }

  return homographies;
}

std::vector<cv::Matx33d> computeGridToFrameHomographies()
{
  const auto correspondences = getFlybyCorrespondences();

  std::vector<cv::Matx33d> homographies;
  for (const auto& corr : correspondences)
  {
    homographies.push_back(cv::findHomography(corr.grid_points, corr.image_points));
  }

  return homographies;
}

Map createMapFromFrames(
    const std::vector<cv::Mat>& frames,
    const std::vector<cv::Matx33d>& H_grid_frames,
    double map_scale,
    bool do_visualisation)
{
  // Grid dimensions.
  const int x_max = 4;
  const int y_max = 5;

  // Map dimensions.
  cv::Size map_size{static_cast<int>(x_max * map_scale), static_cast<int>(y_max * map_scale)};

  // Similiarity transformation between grid and map.
  cv::Matx33d S_map_grid{
      map_scale, 0.0, 0.0,
      0.0, map_scale, 0.0,
      0.0, 0.0, 1.0};

  // Create the map as a mosaic of each frame warped into the map coordinates.
  cv::Mat map_image;
  for (size_t i=0; i < frames.size(); ++i)
  {
    // Compute transformation from frame to map.
    const auto H_map_frame = S_map_grid * H_grid_frames[i];

    // Warp frame into map coordinate system.
    cv::Mat frame_warp;
    cv::warpPerspective(frames[i], frame_warp, H_map_frame, map_size);

    // Create a mask for the frame, and warp the map.
    cv::Mat mask = cv::Mat::ones(frames[i].size(), CV_8UC1);
    cv::Mat mask_warp;
    cv::warpPerspective(mask, mask_warp, H_map_frame, map_size);

    // Erode the mask to make the boundary prettier.
    cv::erode(mask_warp, mask_warp, cv::Mat());

    // Copy the relevant part of the frame into the map image.
    frame_warp.copyTo(map_image, mask_warp);

    // Visualise each step. Press a key to continue.
    if (do_visualisation)
    {
      cv::imshow("Original", frames[i]);
      cv::imshow("Map", map_image);
      cv::waitKey();
    }
  }

  return {map_image, S_map_grid};
}
