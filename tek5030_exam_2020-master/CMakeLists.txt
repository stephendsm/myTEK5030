cmake_minimum_required( VERSION 3.5.1 )

project(tek5030_exam_2020)

# Compiler flags
if (MSVC)
  set(compiler_options /W4)
  add_definitions(/D_USE_MATH_DEFINES)
else ()
  set(compiler_options -Wall -Wextra -Wpedantic)
endif (MSVC)

# Add libraries.
find_package(OpenCV 4 REQUIRED)

add_library(${PROJECT_NAME}
  flyby.h
  flyby.cpp
  flyby_correspondences.h
  match_symmetric.h
  train_detection.h
  train_detection.cpp
  )

target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
  )

target_compile_options(${PROJECT_NAME} PRIVATE ${compiler_options})
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 14
  )

# Executables for sample solutions to the exam exercises.
add_executable(1b_show_grid 1b_show_grid.cpp)
target_link_libraries(1b_show_grid ${PROJECT_NAME})

add_executable(2b_create_map 2b_create_map.cpp)
target_link_libraries(2b_create_map ${PROJECT_NAME})

add_executable(3b_detect_train 3b_detect_train.cpp)
target_link_libraries(3b_detect_train ${PROJECT_NAME})

add_executable(3c_and_3d_map_detections 3c_and_3d_map_detections.cpp)
target_link_libraries(3c_and_3d_map_detections ${PROJECT_NAME})
