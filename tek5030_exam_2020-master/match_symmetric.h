#pragma once

#include "opencv2/core.hpp"

std::vector<cv::DMatch> extractSymmetricMatches(const std::vector<cv::DMatch>& matches1, const std::vector<cv::DMatch>& matches2)
{
  std::vector<cv::DMatch> symmetric_matches;
  for (const auto& match1 : matches1)
  {
    for (const auto& match2 : matches2)
    {
      if (match1.queryIdx == match2.trainIdx && match2.queryIdx == match1.trainIdx)
      {
        symmetric_matches.push_back(match1);
        break;
      }
    }
  }

  return symmetric_matches;
}