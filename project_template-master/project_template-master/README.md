# C++ project template for TEK5030
This repo contains a simple project template for TEK5030, that you can use to develop your own projects.

## About the files in this template
- *CMakeLists.txt*

  The cmake project file.
  Defines the project name, and which source files that are part of the project.
  It also lets us set compiler options and define which libraries we want to find and link to.

- *main.cpp*

  Contains the main function, which will start when you run you project.
  It now implements some example code taken from [lab 1](https://github.uio.no/tek5030/lab_01).
  
- *img_grid.png*
  
  Example image used in the sample code to show how to open an image.
  Feel free to remove.
  
- *README.md*
  
  This readme file.
  You can edit this [markdown-file](https://guides.github.com/features/mastering-markdown/) to document you project.

- *.gitignore*
  
  Specifies which files git should ignore, such as build directories.
  
## How to use this template
- Clone or copy the contents of this repo.
- Define the name of your project at line 5 in *CMakeLists.txt*:
  
  ```cmake
    # Set the name of your project.
    # This will also be the name of the executable.
    project(my_project)
  ```
- Add additional source files to your project by inserting the file names into the list in the call to `add_executable` at line 14 in *CMakeLists.txt*.
  For example, add your class implemented in *my_class.h* and *my_class.cpp* by inserting:
  
  ```cmake
    # Set up you project to build an executable with the same name as your project name.
    # Add your own header and source files here.
    add_executable(${PROJECT_NAME} 
      main.cpp
      my_class.h
      my_class.cpp
  )
  ```
  You can also add source files automatically to your project in CLion:
    - Right-click the project in the project pane, or press *Alt + Insert*.
    - Choose for example *New &rarr; C/C++ Source File*.
    - Specify the name of the file, and check the box *Add to targets*.
- Remove the sample code in *main.cpp*, and start writing your own.
  Remember to include the header files you need to use functionality from external libraries.
  The sample code already includes most of the functionality from OpenCV and Eigen.
- Compile, run, test and repeat :)

## Useful resources
Please read our [introduction to C++ and OpenCV](https://github.uio.no/tek5030/lab_00/blob/master/lab-guide/resources/TEK5030-Compendium.pdf)!

- C++
  - C++ reference: [http://en.cppreference.com/w/](http://en.cppreference.com/w/)
- OpenCV
  - Documentation: [https://docs.opencv.org/4.0.1/](https://docs.opencv.org/4.0.1/)
  - Tutorials: [https://docs.opencv.org/4.0.1/d9/df8/tutorial_root.html](https://docs.opencv.org/4.0.1/d9/df8/tutorial_root.html)
- CLion
  - Documentation: [https://www.jetbrains.com/clion/documentation/](https://www.jetbrains.com/clion/documentation/)
  - Reference card: [https://resources.jetbrains.com/storage/products/clion/docs/CLion_ReferenceCard.pdf](https://resources.jetbrains.com/storage/products/clion/docs/CLion_ReferenceCard.pdf)
- Eigen
  - [Eigen documentation main page](http://eigen.tuxfamily.org/dox/)
  - [Quick reference guide](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html)
  - [Matlab to Eigen translations](http://eigen.tuxfamily.org/dox/AsciiQuickReference.txt)
