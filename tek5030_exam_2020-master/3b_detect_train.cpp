#include "train_detection.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

int main()
{
  try
  {
    // Read the train image sequence.
    // TODO: Set path to dataset.
    const std::string path_to_train_seq{"SET_PATH_TO_DATA/train_sequence"};
    const auto image_sequence = getTrainSequence(path_to_train_seq);

    // Detect the train in each frame.
    std::vector<TrainDetections> detections = detectTrainFromMotion(image_sequence);

    // Visualise the detections.
    cv::Mat mean_with_detections = computeMeanOverSequence(image_sequence);
    for (const auto& detection : detections)
    {
      // Convert difference image to color, so that we can draw in color.
      cv::Mat diff_img;
      cv::cvtColor(detection.difference_image, diff_img, cv::COLOR_GRAY2BGR);

      // Extract the vertices of the rotated bounding box.
      const auto& box = detection.box;
      cv::Point2f vertices[4];
      box.points(vertices);

      // Draw the rotated bounding box.
      for (int i = 0; i < 4; i++)
      {
        cv::line(diff_img, vertices[i], vertices[(i+1)%4], {0,0, 255}, 3);
      }

      // Draw a marker for the center of the box.
      cv::drawMarker(diff_img, box.center, {0, 255, 0}, cv::MARKER_CROSS, 20, 3);
      cv::drawMarker(mean_with_detections, box.center, {0, 255, 0}, cv::MARKER_CROSS, 20, 3);

      // Draw a marker on the middle of the bottom long side.
      const auto bottom_point = computePointAtBottomOfBox(box);
      cv::drawMarker(diff_img, bottom_point, {255, 0, 255}, cv::MARKER_CROSS, 20, 3);
      cv::drawMarker(mean_with_detections, bottom_point, {255, 0, 255}, cv::MARKER_CROSS, 20, 3);

      // Show as "video".
      cv::imshow("Detection", diff_img);
      cv::waitKey(100);
    }

    // Show all detections.
    cv::imshow("Detection", mean_with_detections);
    cv::waitKey();
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