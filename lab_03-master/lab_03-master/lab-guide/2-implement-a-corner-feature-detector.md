# Step 2: Implement a corner feature detector
Now, lets implement our corner detector!

## 1. Implement the filter kernels
Take a look at the function declarations and documentation in [filters.h](https://github.com/tek5030/lab_03/blob/master/filters.h).

Then study how we have written `create1DGaussianKernel()` in [filters.cpp](https://github.com/tek5030/lab_03/blob/master/filters.cpp). 
This function takes a standard deviation and an optional filter radius to produce a filter kernel that samples and normalizes the Gaussian function

![Gaussian kernel formula](img/math_gaussian.png)

Follow our implementation of `create1DGaussianKernel()` to implement the derivated Gaussian kernel in `create1DDerivatedGaussianKernel()`, where the derivative of a Gaussian is given by

![Derivative of a Gaussian formula](img/math_derivative-of-gaussian.png)

Hint: Use `create1DGaussianKernel()`!

Check that your implementation returns the correct result, for example by printing the result to the console. 
When the kernel looks reasonable, we are ready to implement `CornerDetector`.

## 2. Compute the image gradients
Take a look at the class declaration and documentation in [corner_detector.h](https://github.com/tek5030/lab_03/blob/master/corner_detector.h). 
What corner metrics are supported by the detector?

In `CornerDetector`, we have available the private members `g_kernel_` and `dg_kernel_`, constructed from the filter kernel functions we implemented above (see the implementation of the constructor `CornerDetector::CornerDetector(...)` in [corner_detector.cpp:3](https://github.com/tek5030/lab_03/blob/master/corner_detector.cpp#L3)).

Go to `CornerDetector::detect(...)` in [corner_detector.cpp:17](https://github.com/tek5030/lab_03/blob/master/corner_detector.cpp#L17).

Recall from last week's lectures that we can apply a linearly separable 2D filter by convolving an image with each of the two corresponding 1D filter components consecutively. 
Also recall that we can estimate the image gradients in noisy images by convolving with a filter that corresponds to a derivated Gaussian in one direction, and a Gaussian in the other.

Use the 1D filter kernels in `g_kernel_` and `dg_kernel_` to compute the 2D gradient images *I*<sub>x</sub> and *I*<sub>y</sub> by using the OpenCV function [cv::sepFilter2D](https://docs.opencv.org/4.0.1/d4/d86/group__imgproc__filter.html#ga910e29ff7d7b105057d1625a4bf6318d).

Hint: Use the commented code.

Compile and run the code. 
Since `do_visualize_` should be `true`, you should be able to inspect the gradient images, and check that they look reasonable.

## 3. Compute *M* implicitly by computing *A*, *B* and *C*
Recall from the lectures that

![Definition of M matrix](img/math_m-matrix.png)

where

![Definition of A, B and C matrices](img/math_a-b-c-matrices.png)

and *w(x, y)* is a Gaussian windowing function.
The *A*, *B* and *C* images have the same size as the gradient images (and the original image).

First, compute the unwindowed version of images *A*, *B* and *C* by performing element-wise multiplication on the correct gradient images. 
See the [cv::Mat::mul()](https://docs.opencv.org/4.0.1/d3/d63/classcv_1_1Mat.html#a385c09827713dc3e6d713bfad8460706) member function.

Then, convolve these images with the Gaussian windowing filter. 
Use the 1D kernel given in the private member `win_kernel_`, and perform separable filtering as we did above.

Compile and run the code, and check that the results look reasonable.

## 4. Implement the corner metrics
We are now ready to compute the metrics for cornerishness!

Recall the following corner metrics from the lecture:
- **Harris**:
  
  ![Harris formula](img/math_harris.png)
  
- **Harmonic mean**:
  
  ![Harmonic mean formula](img/math_harmonic-mean.png)
  
- **Minimum eigen value**:
  
  ![Minimum eigen value formula](img/math_min-eigen.png)
  
Implement these metrics in the member functions `CornerDetector::harrisMetric()`, `CornerDetector::harmonicMeanMetric()` and `CornerDetector::minEigenMetric()` in [corner_detector.cpp](https://github.uio.no/tek5030/lab_03/blob/master/corner_detector.cpp).

Hint: Use image operations on the *A*, *B* and *C* images. 
You don't need to use any loops!

Compile and run the code, and check that the resulting metric image looks reasonable. 
You can change between the different metrics by changing the first argument in the construction of the detector object at [lab_3.cpp:26](https://github.com/tek5030/lab_03/blob/master/lab_3.cpp#L26).

## 5. Dilate the image to find local maximal values
We now want to find local maximum response values by dilating the response image with an appropriate kernel. 
In this way, each pixel will be equal to the maximum in the neighborhood (defined by the kernel). 
We will soon use this dilated image to perform so called *non-maximum suppression*.

Apply [cv::dilate](https://docs.opencv.org/4.0.1/d4/d86/group__imgproc__filter.html#ga4ff0f3318642c4f469d0e11f242f3b6c) on the response image with an appropriate kernel.

## 6. Compute the metric threshold
We will compute the threshold by setting it to an appropriate fraction of the maximal response.

First, find the maximum response in the response image by applying [cv::minMaxLoc](https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#gab473bf2eb6d14ff97e89b355dac20707). 
This function needs pointers to variables that will hold the resulting max- and min values. 
You can get a pointer to a variable by using the &-operator: `&max_val`.

Then, find the threshold by computing `max_val * quality_level_` using the private member `quality_level_`.

Compile and run. 
Check that the threshold seems reasonable.

## 7. Extract local maxima above the threshold
The final step is to extract the local maximum response values above the computed threshold.

Use [logical image operations](https://docs.opencv.org/4.0.1/d1/d10/classcv_1_1MatExpr.html#ad5ca0810ca85918075da95a79d9a97f7) to compute a logical image over pixels that are above the threshold and equal to the local maximum response value. 
How will this last check suppress locally non-maximum values?
(Try without it!)

Extract detected points with [cv::findNonZero](https://docs.opencv.org/4.0.1/d2/de8/group__core__array.html#gaed7df59a3539b4cc0fe5c9c8d7586190) on the logical image.

Compile and run. Check that the logical image looks reasonable.

## `CornerDetector` is finished!
Congratulations, you have implemented your own corner detector!

Play around with the detector a bit. 
Try different metrics, parameters and scenes. 
Then, continue to [the next step](3-detect-circles-from-corners-with-ransac.md), so we can use the corners to find circles!
