#include "flyby.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

// Forward declaration for drawing function.
void drawGridLines(cv::Mat& frame, const cv::Matx33d& H_frame_grid);

int main()
{
  try
  {
    // Read the flyby frames.
    // TODO: Set path to dataset.
    const std::string path_to_flyby_seq{"SET_PATH_TO_DATA/flyby_sequence"};
    const auto flyby_sequence = getFlybySequence(path_to_flyby_seq);

    // Compute the homographies from the correspondences.
    const auto H_frames_grid = computeGridToFrameHomographies();

    // Use the homographies to draw the grid into each frame.
    for (int i=0; i < flyby_sequence.size(); ++i)
    {
      cv::Mat grid_line_image = flyby_sequence[i].clone();
      drawGridLines(grid_line_image, H_frames_grid[i]);
      cv::imshow("Grid lines", grid_line_image);

      cv::waitKey();
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Caught exception:\n"
              << e.what() << "\n";
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception\n";
  }

  return EXIT_SUCCESS;
}

void drawGridLines(cv::Mat& frame, const cv::Matx33d& H_frame_grid)
{
  constexpr int x_max = 4.f;
  constexpr int y_max = 5.f;

  // Draw vertical lines.
  for (int x=0; x <= x_max; ++x)
  {
    // Define start and end points.
    std::vector<cv::Point2f> grid_points = {cv::Point2f(x, 0.f), cv::Point2f(x, y_max)};

    // I here use OpenCV directly to compute the transformation.
    // Eigen is easier to use, but here I avoid converting between OpenCV and Eigen.
    std::vector<cv::Point2f> image_points(2);
    cv::perspectiveTransform(grid_points, image_points, H_frame_grid);

    // Draw line
    cv::line(frame, image_points[0], image_points[1], {0, 0, 255}, 3);
  }

  // Draw horisontal lines.
  for (int y=0; y <= y_max; ++y)
  {
    // Define start and end points.
    std::vector<cv::Point2f> grid_points = {cv::Point2f(0.f, y), cv::Point2f(x_max, y)};

    // Transform the points into frame coordinates.
    std::vector<cv::Point2f> image_points(2);
    cv::perspectiveTransform(grid_points, image_points, H_frame_grid);

    // Draw line.
    cv::line(frame, image_points[0], image_points[1], {0, 0, 255}, 3);
  }
}
