#include "application.h"

namespace app {

void Application::Start() {
  std::cout << "Приложение запущено" << std::endl;
//  while (window_.IsOpen()) {
//    gui::UserChoice user_choice = window_.Tick();
//
//    if (user_choice.type == gui::RESET) {
//      Reset();
//    } else if (user_choice.type == gui::ADD_PROCESS) {
//
//    }
//
//    window_.Update();
//  }
}

void Application::Reset() {
  network_.reset(); //? Может не нужно, просто передавать новый адрес
  window_.SetRuntime(0.0);
  window_.SetTimeout(0.0);
}

}  // namespace app
