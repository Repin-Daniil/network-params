#include <iostream>
#include "application/application.h"

int main() {
  try {
    app::Application app;
    app.Start();
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
