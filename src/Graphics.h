#ifndef SRC_GRAPHICS_H_
#define SRC_GRAPHICS_H_

#include <opencv2/core.hpp>
#include <string>
#include <vector>

#include "TrafficObject.h"

class Graphics {
 public:
  // constructor / desctructor

  // getters / setters
  void setBgFilename(const std::string &filename) { _bgFilename = filename; }
  void setTrafficObjects(
      std::vector<std::shared_ptr<TrafficObject>> const  &trafficObjects) {
    _trafficObjects = trafficObjects;
  }

  // typical behaviour methods
  void simulate();

 private:
  // typical behaviour methods
  void loadBackgroundImg();
  void drawTrafficObjects();

  //  member variables
  std::vector<std::shared_ptr<TrafficObject>> _trafficObjects;
  std::string _bgFilename;
  std::string _windowName;
  std::vector<cv::Mat> _images;
};

#endif // SRC_GRAPHICS_H_
