#pragma once

#include <string_view>
#include <string>
#include <unordered_map>

using namespace std::literals;

struct ApplicationConstants {
  ApplicationConstants() = delete;

  inline static const std::string kTitle = "Network Parameters";
  constexpr static std::string_view kPathToFont = "res/TimesNewRomanRegular.ttf"sv;

  // gui
  constexpr static int kWidth = 1600;
  constexpr static int kHeight = 500;
};
//
// struct Labels {
//  Labels() = delete;
//
//  // Buttons
//  constexpr static std::string_view kCalculate = "Calculate"sv;
//  constexpr static std::string_view kReset = "Reset"sv;
//
//  // Labels
//  constexpr static std::string_view kInput = "Input"sv;
//  constexpr static std::string_view kResults = "Results"sv;
//  constexpr static std::string_view kRange = "IP Range"sv;
//
//  // Input Labels
//  constexpr static std::string_view kStart = "Start: "sv;
//  constexpr static std::string_view kFinish = "Finish: "sv;
//  constexpr static std::string_view kNetworkParameters = "Network Parameters"sv;
//  constexpr static std::string_view kNetworkAddress = "Network Address: "sv;
//  constexpr static std::string_view kBroadcastAddress = "Broadcast Address: "sv;
//  constexpr static std::string_view kMacAddress = "MAC-address: "sv;
//  constexpr static std::string_view kSubnetMask= "Subnet mask: "sv;
//};

struct Labels {
  Labels() = delete;

  // Buttons
  constexpr static std::wstring_view kCalculate = L"Рассчитать"sv;
  constexpr static std::wstring_view kReset = L"Сброс"sv;

  // Labels
  constexpr static std::wstring_view kInput = L"Ввод"sv;
  constexpr static std::wstring_view kResults = L"Результаты"sv;
  constexpr static std::wstring_view kRange = L"Диапазон IP-адресов"sv;

  // Input Labels
  constexpr static std::wstring_view kStart = L"Начало: "sv;
  constexpr static std::wstring_view kFinish = L"Конец: "sv;
  constexpr static std::wstring_view kNetworkParameters = L"Параметры сети"sv;
  constexpr static std::string_view kNetworkAddress = "Network Address: "sv;
  constexpr static std::string_view kBroadcastAddress = "Broadcast Address: "sv;
  constexpr static std::string_view kMacAddress = "MAC-address: "sv;
  constexpr static std::string_view kSubnetMask = "Subnet Mask: "sv;
};
