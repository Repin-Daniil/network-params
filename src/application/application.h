#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "domain/network.h"
#include "gui/window.h"

namespace app {

class Application {
 public:
  Application();
  void Start();

 private:
  void Reset();

 private:
  gui::Window window_;
  network::Network network_;
};

}  // namespace app
