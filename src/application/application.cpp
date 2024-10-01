#include "application.h"

namespace app {

Application::Application() : window_(ApplicationConstants::kTitle) {
}

void Application::Start() {
  std::cout << "App start" << std::endl;

  while (window_.IsOpen()) {
    std::cout << "Ping!" << std::endl;
    gui::UserChoice user_choice = window_.Tick();

    if (user_choice.type == gui::RESET) {
      Reset();
    } else if (user_choice.type == gui::CALCULATE) {
      auto result = network_.GetNetworkSettings({user_choice.ip_range.first, user_choice.ip_range.second});
      // TODO Не просто SetResult У нас будет мапа с mac-адресами. Нужно их склеить, при это добавив \n и количество
      // пробелом, равное len(Labels::kMacAddress)
      window_.SetResult(result.network_address, result.broadcast_address, result.mac_address, result.subnet_mask);
    }
  }
}

void Application::Reset() {
  window_.Reset();
}

}  // namespace app
