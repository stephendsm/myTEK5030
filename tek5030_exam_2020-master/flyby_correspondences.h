#pragma once

#include "opencv2/core.hpp"

// A simple data structure for image <-> grid correspondences.
struct Correspondences
{
  std::vector<cv::Point2f> image_points;
  std::vector<cv::Point2f> grid_points;
};

// Returns the correspondences for each image.
std::vector<Correspondences> getFlybyCorrespondences()
{
  std::vector<Correspondences> corrs(4);

  // Frame 0:
  corrs[0].grid_points = {{1,2}, {3,2}, {2,0}, {3,0}};
  corrs[0].image_points = {{102, 911}, {1660, 878}, {885, 52}, {1323, 68}};

  // Frame 1:
  corrs[1].grid_points = {{2,4}, {4,4}, {2,2}, {4,2}};
  corrs[1].image_points = {{337, 971}, {1660, 914}, {507, 71}, {1370, 92}};

  // Frame 2:
  corrs[2].grid_points = {{1,5}, {2,5}, {1,3}, {2,3}};
  corrs[2].image_points = {{969, 938}, {1702, 970}, {1027, 37}, {1494, 28}};

  // Frame 3:
  corrs[3].grid_points = {{1,4}, {2,4}, {1,2}, {2,2}};
  corrs[3].image_points = {{843, 982}, {1551, 971}, {858, 22}, {1316, 42}};

  return corrs;
}
