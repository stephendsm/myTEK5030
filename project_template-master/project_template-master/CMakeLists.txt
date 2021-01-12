cmake_minimum_required( VERSION 3.5.1 )

# Set the name of your project.
# This will also be the name of the executable.
project(my_project)

# Find libraries
find_package(OpenCV 4 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Sophus REQUIRED)

# Set up you project to build an executable with the same name as your project name.
# Add your own header and source files here.
add_executable(${PROJECT_NAME} 
  main.cpp
  )

# Link the project to the libraries.
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
  Sophus::Sophus
  Eigen3::Eigen
  )

# Set compiler flags and properties.
if (MSVC)
  set(compiler_options /W4)
  add_definitions(/D_USE_MATH_DEFINES)
else ()
  set(compiler_options -Wall -Wextra -Wpedantic)
endif (MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE ${compiler_options})
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 14
  )
