#include "train_detection.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

constexpr double pi = 3.14159265358979323846;

constexpr double deg2rad(const double& deg)
{
  return deg * (pi/180.0);
}

std::vector<TrainDetections> detectTrainFromMotion(const std::vector<cv::Mat>& image_sequence)
{
  cv::Mat reference = computeMeanOverSequence(image_sequence);

  std::vector<TrainDetections> detections(image_sequence.size());
  for (size_t i=0; i<image_sequence.size(); ++i)
  {
    detections[i] = detectDifferenceInImage(reference, image_sequence[i]);
  }

  return detections;
}

cv::Mat computeMeanOverSequence(const std::vector<cv::Mat>& image_sequence)
{
  cv::Mat mean(image_sequence[0].size(), CV_32FC3);
  for (const auto& image : image_sequence)
  {
    mean += image;
  }
  mean /= image_sequence.size();

  return mean;
}

TrainDetections detectDifferenceInImage(const cv::Mat& reference, const cv::Mat& image)
{
  // Take the difference between the reference (mean over all images) and the current image.
  cv::Mat diff_img;
  cv::absdiff(reference, image, diff_img);

  // Extract the red channel, which has good signal for the target, and reduces the influence of shadow.
  cv::extractChannel(diff_img, diff_img, 2);

  // Detect difference above a given threshold, chosen manually for simplicity.
  cv::Mat det_img;
  cv::threshold(diff_img, det_img, 0.51, 1., cv::THRESH_BINARY);

  // Convert to 8-bit image, and extract detected pixels.
  det_img.convertTo(det_img, CV_8U, 255.);
  std::vector<cv::Point> det_pixels;
  cv::findNonZero(det_img, det_pixels);

  // Return the detection result.
  return {cv::fitEllipse(det_pixels), diff_img, det_img};
}

std::vector<cv::Mat> getTrainSequence(const std::string& path_to_directory)
{
  const int num_images = 100;
  std::vector<cv::Mat> image_sequence(num_images);

  for (int i=0; i< num_images; ++i)
  {
    std::stringstream filename;
    filename << path_to_directory << "/static_video_frame_" << i << ".png";
    cv::Mat img = cv::imread(filename.str());

    if (img.empty())
    {
      throw std::runtime_error("Could not read \"" + filename.str() + "\"");
    }

    img.convertTo(image_sequence[i], CV_32F, 1.0/255.0);
  }

  return image_sequence;

}

cv::Point2f computePointAtBottomOfBox(const cv::RotatedRect& box)
{
  return box.center + cv::Point2f(0.5 * box.size.width * std::cos(deg2rad(box.angle)), 0.5 * box.size.width * std::sin(deg2rad(box.angle)));
}
