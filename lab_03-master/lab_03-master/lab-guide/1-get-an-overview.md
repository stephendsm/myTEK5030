# Step 1: Get an overview
We will start by presenting an overview of the method and the contents of this project.

## Algorithm overview
We will use `CornerDetector` to compute and return a list of detected keypoints. 
The main steps in the detection algorithm are:
- Compute the image gradients using derivative of Gaussian filters.
- Use the image gradients to compute the images A, B, C from the lectures:
  - Element-wise products of the gradients.
  - Apply windowing by convolving these with a (bigger) Gaussian.
- Use A, B and C to compute a corner metric for the entire image:
  - *&lambda;*<sub>min</sub>, Harris and Harmonic Mean.
- Threshold the corner metric image and find local maxima:
  - Morphological operations.
  - Logical operations.
  
We will then try to find find a circle hidden in the list of detected corner keypoints. 
To achieve this, we will use `CircleEstimator` to apply RANSAC, and extract the circle estimate based on the largest set of inliers.

## Introduction to the project source files
We have chosen to distribute the code on the following files:
- *main.cpp*
  
  Starts lab 3, catches any exceptions and prints their error message on the console.
  
- *lab_3.h, lab_3.cpp*

  Implements the lab 3 loop:
  - Get next frame
  - Detect corners
  - Estimate circle from corner points
  - Display results
 
 - *corner_detector.h, corner_detector.cpp*
   
   Implements a class that can perform corner detection on frames. 
   You will be responsible for writing most of the code in *corner_detection.cpp*.
   
 - *filters.h, filters.cpp*
   
   Contains functions that create 1D Gaussian and 1D derivated Gaussian kernels. 
   You will implement the derivated Gaussian kernel function.
   
 - *circle_estimator.h, circle_estimator.cpp*
   
   Implements a class that estimates circles from points along the circle circumference. 
   You will be responsible for finishing the RANSAC loop.

- *circle.h, circle.cpp*
  
  Implements a basic circle and functions for creating circles and measuring distances between circles and points.

## `lab3()`
First, take a look at the `lab3()` function in [lab_3.cpp](https://github.com/tek5030/lab_03/blob/master/lab_3.cpp). 
Try to understand the steps taken here, and please ask one of the instructors if you are uncertain.

Then build and run the project. 
You should be able to see a video stream from the camera, but the program doesn't do anything interesting - yet!

Please continue to the [next step](2-implement-a-corner-feature-detector.md) to get started with the interesting stuff!
