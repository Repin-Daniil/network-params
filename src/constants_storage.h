#pragma once

#include <string_view>
#include <unordered_map>

using namespace std::literals;

struct ApplicationConstants {
  ApplicationConstants() = delete;

  constexpr static std::string_view PATH_TO_FONT = "res/TimesNewRomanRegular.ttf"sv;

  // gui
  constexpr static int WIDTH = 1600;
  constexpr static int HEIGHT = 900;
};
