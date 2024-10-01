#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "constants_storage.h"

namespace gui {

enum Event { NOTHING, CALCULATE, RESET };

struct UserChoice {
  Event type = NOTHING;
  std::pair<std::string, std::string> ip_range;
};

class Window {
 public:
  explicit Window(const std::string& title);

  // Methods
  void Init();
  UserChoice Tick();
  void Reset();

  // Setters
  void SetResult(std::string network_address, std::string broadcast_address, std::string mac_address,
                 std::string subnet_mask);

  // Predicates
  bool IsOpen() const noexcept;

 private:
  UserChoice HandleMouseButtonPressed(sf::Vector2i cursor_position);
  void Update();
  void ResizeWindow(sf::Event event);
  void Draw();
  void Clear();
  void UpdateLabels();

  // Init
  void InitWindow();
  void InitLabels();
  void InitInputFields();
  void InitResultLabels();
  void InitButtons();

  // Predicates
  bool IsMouseClicked(const sf::Event& event);
  bool IsBackspacePressed(const sf::Event& event);
  bool IsDigitInput(const sf::Event& event);
  bool IsDotEntered(const sf::Event& event);

  // Typing
  void AddSymbol(const sf::Event& event);
  void RemoveSymbol(const sf::Event& event);

 private:
  std::string network_address_;
  std::string broadcast_address_;
  std::string mac_address_;
  std::string subnet_mask_;

  /* GUI */
  sf::RenderWindow window_;
  sf::Font font_;

  // Labels
  sf::Text results_zone_label_;
  sf::Text input_zone_label_;
  sf::Text ip_range_label_;
  sf::Text network_parameters_label_;

  // Input fields
  sf::Text start_ip_range_label_;
  sf::Text finish_ip_range_label_;
  sf::Text start_input_text_;
  sf::Text finish_input_text_;
  sf::RectangleShape start_input_field_;
  sf::RectangleShape finish_input_field_;
  bool focus_on_start = true;
  bool focus_on_finish = false;

  // Results Labels
  sf::Text network_address_result_label_;
  sf::Text broadcast_address_result_label_;
  sf::Text mac_address_result_label_;
  sf::Text subnet_mask_result_label_;

  // Buttons
  sf::Text calculate_button_label_;
  sf::Text reset_button_label_;
  bool calc_button_pressed_ = false;
  bool reset_button_pressed_ = false;
};

}  // namespace gui
