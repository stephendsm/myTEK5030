#include "Eigen/Eigen"
#include "opencv2/core/eigen.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

// You can use this definition of pi:
constexpr double pi = 3.14159265358979323846;

// You can convert from degrees to radians with this function:
constexpr double deg2rad(const double& deg)
{
  return deg * (pi/180.0);
}


int main()
{
  // 1. Eigen and homogeneous representations.
  //------------------------------------------
  // Todo: Translation.
  Eigen::Vector2d t;

  // Todo: Rotation (tip: take a look at https://eigen.tuxfamily.org/dox/group__Geometry__Module.html).
  Eigen::Matrix2d R;

  // Todo: Euclidean transformation that rotates and then translates.
  Eigen::Matrix3d E;

  std::cout << "Euclidean transformation E = " << std::endl << E << std::endl << std::endl;

  // Todo: Perform the transformation on a pixel u.
  Eigen::Vector2d u;
  Eigen::Vector2d u_transformed;

  std::cout << "Original pixel u = " << std::endl << u << std::endl << std::endl;
  std::cout << "Transformed pixel u_transformed = " << std::endl << u_transformed << std::endl << std::endl;


  // 2. Eigen and OpenCV.
  //---------------------
  cv::Mat E_cv;

  // Todo: Convert from Eigen matrix to OpenCV cv::Mat.

  std::cout << "Euclidean transformation E_cv = " << std::endl << E_cv << std::endl << std::endl;


  // 3. Transform images.
  //---------------------
  // Todo: Read the image.

  // Todo: Display the original image.

  // Todo: Perform transformation on the image.

  // Todo: Display the transformed image.


  // 4. Composing transformations.
  //------------------------------
  // Todo: Compose transformations to rotate and scale around the centre of the image.


  // Please press a key, don't close the windows with your mouse.
  cv::waitKey();
  return EXIT_SUCCESS;
}
