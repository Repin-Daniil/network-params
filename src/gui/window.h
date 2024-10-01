#pragma once

#include <optional>
#include <string>
#include <vector>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

#include "../constants_storage.h"

namespace gui {

enum Event { NOTHING, ADD_PROCESS, RESET };

struct UserChoice {
  Event type = NOTHING;
  int process_time = 0;
};

class Window {
 public:
  explicit Window(const std::string& title)
      : window_({ApplicationConstants::WIDTH, ApplicationConstants::HEIGHT}, title) {
    InitWindow();
    InitInputFields();
    InitLabels();
    InitTimeSliceShape();
  }

  // Methods
  UserChoice Tick();
  void Update(const std::vector<std::vector<bool>>& table);

  // Setters
  void SetRuntime(double runtime);
  void SetTimeout(double timeout);

  // Predicates
  bool IsOpen() const noexcept;

 private:
  UserChoice HandleMouseButtonPressed(sf::Vector2i cursor_position);
  void ResizeWindow(sf::Event event);
  void Clear();
  void DrawTable(const std::vector<std::vector<bool>>& table);

  // Init
  void InitWindow();
  void InitInputFields();
  void InitLabels();
  void InitTimeSliceShape();

  // Predicates
  bool IsMouseClicked(const sf::Event& event);
  bool IsBackspacePressed(const sf::Event& event);
  bool IsDigitInput(const sf::Event& event);

 private:
  double current_timeout_ = 0.0;
  double current_runtime_ = 0.0;

  // GUI
  sf::RenderWindow window_;
  sf::Font font_;

  sf::RectangleShape input_field_;
  sf::RectangleShape time_slice_shape_;
  sf::RectangleShape add_process_border_;
  sf::RectangleShape reset_border_;

  sf::Text add_process_;
  sf::Text reset_;
  sf::Text average_timeout_label_;
  sf::Text average_runtime_label_;
  sf::Text input_text_;
};

}  // namespace gui
