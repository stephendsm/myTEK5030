#include "flyby.h"
#include <iostream>

int main()
{
  try
  {
    // Read the image sequence.
    // TODO: Set path to dataset.
    const std::string path_to_flyby_seq{"SET_PATH_TO_DATA/flyby_sequence"};
    const auto flyby_sequence = getFlybySequence(path_to_flyby_seq);

    // Compute the homographies from each frame to the grid coordinate system.
    const auto H_grid_frames = computeFrameToGridHomographies();

    // Define the size of each grid cell in map pixels.
    const double map_scale = 200.0;

    // Create and visualise the map.
    const bool do_visualise = true;
    createMapFromFrames(flyby_sequence, H_grid_frames, map_scale, do_visualise);
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
