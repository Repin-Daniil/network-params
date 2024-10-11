#include "application.h"
#include <sstream>

namespace app {

Application::Application() : window_(ApplicationConstants::kTitle) {
}

void Application::Start() {
  while (window_.IsOpen()) {
    gui::UserChoice user_choice = window_.Tick();

    if (user_choice.type == gui::RESET) {
      Reset();
    } else if (user_choice.type == gui::CALCULATE) {
      try {
        auto result = network_.GetNetworkSettings({user_choice.ip_range.first, user_choice.ip_range.second});

        std::wstringstream mac_result;

        for (auto& address : result.mac_addresses) {
          mac_result << address.first << L" — " << std::wstring(address.second.begin(), address.second.end());
          mac_result << L"\n" << std::wstring(Labels::kMacAddress.size() * 2, L' ');
        }

        window_.SetResult(result.network_address, result.broadcast_address, mac_result.str(), result.subnet_mask);
      } catch (const std::exception& ex) {
        window_.ShowError(ex.what());
      }
    }
  }
}

void Application::Reset() {
  window_.Reset();
}

}  // namespace app
