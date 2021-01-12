#include "flyby.h"
#include "match_symmetric.h"
#include "train_detection.h"
#include "opencv2/calib3d.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <numeric>

// Forward declarations of functions we will use.
// The definitions are below main().
cv::Matx33d computeHomographyBetweenTrainSequenceAndImageByMatching(
    const std::vector<cv::Mat>& train_sequence,
    const cv::Mat& flyby
    );
cv::Mat drawSpeedPlot(const std::vector<double>& speeds);

double computeMedian(const std::vector<double>& speeds);

int main()
{
  try
  {
    // Read the flyby sequence, and create the world map (from exercise 2c).
    // TODO: Set path to dataset.
    const std::string path_to_flyby_seq{"SET_PATH_TO_DATA/flyby_sequence"};
    const auto flyby_sequence = getFlybySequence(path_to_flyby_seq);
    const auto H_grid_flybys = computeFrameToGridHomographies();
    const double map_scale = 200.;
    const auto map = createMapFromFrames(flyby_sequence, H_grid_flybys, map_scale);

    // Read the train sequence, and detect the train in each frame (from exercise 3b).
    // TODO: Set path to dataset.
    const std::string path_to_train_seq{"SET_PATH_TO_DATA/train_sequence"};
    const auto train_sequence = getTrainSequence(path_to_train_seq);
    const auto detections = detectTrainFromMotion(train_sequence);

    // Extract point representations from the detections.
    // We have chosen to extract the middle point on the bottom long side of the detected object.
    std::vector<cv::Point2f> img_detections;
    for (const auto& detection : detections)
    {
      const auto& box = detection.box;
      img_detections.push_back(computePointAtBottomOfBox(box));
    }

    // Extract frame 1 from the flyby sequence.
    const auto& flyby_frame_1 = flyby_sequence[1];
    const auto& H_grid_flyby1 = H_grid_flybys[1];

    // Compute the homography between the train sequence and flyby frame 1
    // by matching the mean over all the train frames with flyby frame 1.
    const auto H_flyby1_trainseq = computeHomographyBetweenTrainSequenceAndImageByMatching(train_sequence, flyby_frame_1);

    // With this, we can compute the homography
    // between the train sequence and the map image.
    const auto H_map_trainseq = map.S_map_grid * H_grid_flyby1 * H_flyby1_trainseq;

    // Transform the train detection points into the map coordinate frame.
    std::vector<cv::Point2f> map_detections(img_detections.size());
    cv::perspectiveTransform(img_detections, map_detections, H_map_trainseq);

    // Visualise the detections in the map.
    cv::Mat map_with_detections = map.map_image.clone();

    // First, draw the detections.
    for (const auto& point : map_detections)
    {
      cv::drawMarker(map_with_detections, point, {0, 255, 0}, cv::MARKER_SQUARE, 5, 2);
    }

    // Then, compute and draw the velocity.
    std::vector<double> speeds;
    for (size_t i=1; i < map_detections.size()-1; ++i)
    {
      // Compute the velocity (in map units) by numerically derivating the train track.
      const auto vel = 0.5 * (map_detections[i+1] - map_detections[i-1]);

      // Compute athe magnitude of the velocity (the speed) (in map units).
      const auto speed = std::sqrt(vel.dot(vel));

      // Compute and store the speed in cm/s.
      const auto speed_in_cm_s = speed * 25 / map_scale;
      speeds.push_back(speed_in_cm_s);

      // Visualise the velocity.
      cv::arrowedLine(map_with_detections, map_detections[i], map_detections[i] + vel, {0, 255, 0}, 2);
    }

    // Show the detections with estimated velocity.
    cv::imshow("Train detections and velocity", map_with_detections);

    // Plot the speed over time.
    const auto speed_plot = drawSpeedPlot(speeds);
    cv::imshow("Speed", speed_plot);
    cv::waitKey();

    // Compute and print the mean speed.
    const auto mean = std::accumulate(speeds.begin(), speeds.end(), 0.) / speeds.size();
    std::cout << "Mean speed: " << mean << std::endl;

    // Compute and print the median speed.
    const auto median = computeMedian(speeds);
    std::cout << "Median speed: " << median << std::endl;
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

cv::Matx33d computeHomographyBetweenTrainSequenceAndImageByMatching(
    const std::vector<cv::Mat>& train_sequence,
    const cv::Mat& flyby)
{
  // Use SIFT as feature detector and descriptor extractor.
  cv::Ptr<cv::Feature2D> feature_extractor = cv::xfeatures2d::SIFT::create();
  cv::BFMatcher matcher{feature_extractor->defaultNorm()};

  // Use mean over all frames as representative for static video.
  cv::Mat mean = computeMeanOverSequence(train_sequence);
  cv::Mat mean_gray;
  mean.convertTo(mean, CV_8U, 255.);
  cv::cvtColor(mean, mean_gray, cv::COLOR_BGR2GRAY);

  // Detect features and extract descriptors in mean of static video.
  std::vector<cv::KeyPoint> mean_keypoints;
  feature_extractor->detect(mean_gray, mean_keypoints);
  cv::Mat mean_descriptors;
  feature_extractor->compute(mean_gray, mean_keypoints, mean_descriptors);

  // Detect features and extract descriptors in flyby image.
  cv::Mat flyby_gray;
  cv::cvtColor(flyby, flyby_gray, cv::COLOR_BGR2GRAY);
  std::vector<cv::KeyPoint> flyby_keypoints;
  feature_extractor->detect(flyby_gray, flyby_keypoints);
  cv::Mat flyby_descriptors;
  feature_extractor->compute(flyby_gray, flyby_keypoints, flyby_descriptors);

  // Match features both ways, and apply the cross check.
  std::vector<cv::DMatch> matches1;
  matcher.match(mean_descriptors, flyby_descriptors, matches1);
  std::vector<cv::DMatch> matches2;
  matcher.match(flyby_descriptors, mean_descriptors, matches2);
  std::vector<cv::DMatch> good_matches = extractSymmetricMatches(matches1, matches2);

  // Extract matched points.
  std::vector<cv::Point2f> mean_points;
  std::vector<cv::Point2f> flyby_points;
  for (auto& match : good_matches)
  {
    mean_points.push_back(mean_keypoints[match.queryIdx].pt);
    flyby_points.push_back(flyby_keypoints[match.trainIdx].pt);
  }

  // Compute the homography and extract the inliers.
  std::vector<char> inliers;
  cv::Matx33d H_flyby_mean = cv::findHomography(mean_points, flyby_points, cv::RANSAC, 3, inliers);

  // Extract inlier matches for visualization.
  std::vector<cv::DMatch> inlier_matches;
  for (size_t i=0; i<inliers.size(); ++i)
  {
    if (inliers[i] > 0)
    {
      inlier_matches.push_back(good_matches[i]);
    }
  }

  // Visualize matching.
  cv::Mat vis_img;
  cv::drawMatches(mean, mean_keypoints, flyby, flyby_keypoints, inlier_matches, vis_img);
  cv::namedWindow("Matches", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
  cv::imshow("Matches", vis_img);
  cv::waitKey(100);

  return H_flyby_mean;
}

cv::Mat drawSpeedPlot(const std::vector<double>& speeds)
{
  const int margin_px = 10;
  const int plot_side_px = 400;
  const double time_scale = 4.;
  const double speed_scale = 50.;
  const int side_with_margin = plot_side_px + margin_px;

  // Plot the speed data.
  cv::Mat speed_plot = cv::Mat::zeros(plot_side_px + 2*margin_px, plot_side_px + 2*margin_px, CV_8UC3);
  for (size_t i=0; i < speeds.size() - 1; ++i)
  {
    // Draw line between each speed sample.
    cv::line(speed_plot,
             cv::Point2f(margin_px + i * time_scale, side_with_margin - speeds[i] * speed_scale),
             cv::Point2f(margin_px + (i+1.) * time_scale, side_with_margin - speeds[i+1] * speed_scale),
             {0, 255, 0}, 1);
  }

  // Add axes and axes ticks on the y axis.
  cv::line(speed_plot, cv::Point2f(margin_px, margin_px), cv::Point2f(margin_px, side_with_margin), {255, 255, 255}, 1);
  cv::line(speed_plot, cv::Point2f(margin_px, side_with_margin), cv::Point2f(side_with_margin, side_with_margin), {255, 255, 255}, 1);
  for (int y=1; y < plot_side_px/speed_scale; ++y)
  {
    const auto i = side_with_margin - (y * speed_scale);
    cv::line(speed_plot, cv::Point2f(5, i), cv::Point2f(15, i), {255, 255, 255}, 1);
    cv::putText(speed_plot, std::to_string(y), cv::Point2f(0, i), cv::FONT_HERSHEY_PLAIN, 1, {255, 255, 255});
  }

  return speed_plot;
}

double computeMedian(const std::vector<double>& speeds)
{
  // Copy the vector and sort it
  auto sorted_speeds{speeds};
  std::sort(sorted_speeds.begin(), sorted_speeds.end());

  // The median is in the center of the sorted vector.
  if (sorted_speeds.size() % 2 == 0)
  {
    return 0.5 * (sorted_speeds[sorted_speeds.size() / 2 - 1] + sorted_speeds[sorted_speeds.size() / 2]);
  }
  else
  {
    return sorted_speeds[sorted_speeds.size() / 2];
  }
}
