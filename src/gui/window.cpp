#include <variant>
#include "window.h"

namespace gui {

void Window::InitWindow() {
  window_.setFramerateLimit(144);

  if (!font_.loadFromFile(ApplicationConstants::PATH_TO_FONT.data())) {
    throw std::runtime_error("Failed to load font!");
  }
}

void Window::InitTimeSliceShape() {
  time_slice_shape_.setSize(sf::Vector2f(25, 25));
  time_slice_shape_.setOutlineThickness(1);
  time_slice_shape_.setOutlineColor(sf::Color::Black);
}

void Window::InitLabels() {
  add_process_.setFont(font_);
  add_process_.setCharacterSize(25);
  add_process_.setFillColor(sf::Color::Black);
//  add_process_.setString(EnglishLabels::ADD_PROCESS.data());
  add_process_.setPosition(1400, 60);
  add_process_.setOutlineColor(sf::Color::White);
  add_process_.setOutlineThickness(2);

  reset_.setFont(font_);
  reset_.setCharacterSize(25);
  reset_.setFillColor(sf::Color::Black);
//  reset_.setString(EnglishLabels::RESET.data());
  reset_.setPosition(1400, 120);
  reset_.setOutlineColor(sf::Color::White);
  reset_.setOutlineThickness(2);

  average_timeout_label_.setFont(font_);
  average_timeout_label_.setCharacterSize(25);
  average_timeout_label_.setFillColor(sf::Color::White);
//  average_timeout_label_.setString(EnglishLabels::MIDDLE_TIMEOUT.data());
  average_timeout_label_.setPosition(1250, 760);

  average_runtime_label_.setFont(font_);
  average_runtime_label_.setCharacterSize(25);
  average_runtime_label_.setFillColor(sf::Color::White);
//  average_runtime_label_.setString(EnglishLabels::MIDDLE_RUNTIME.data());
  average_runtime_label_.setPosition(1250, 820);

  input_text_.setFont(font_);
  input_text_.setCharacterSize(25);
  input_text_.setFillColor(sf::Color::White);
  input_text_.setPosition(1100, 60);
}

void Window::InitInputFields() {
  input_field_.setSize(sf::Vector2f(250, 30));
  input_field_.setFillColor(sf::Color::Black);
  input_field_.setOutlineThickness(2);
  input_field_.setOutlineColor(sf::Color::White);
  input_field_.setPosition(1100, 60);
}

bool Window::IsOpen() const noexcept {
  return window_.isOpen();
}

UserChoice Window::Tick() {
  sf::Event event{};

  if (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window_.close();
    } else if (event.type == sf::Event::Resized) {
      ResizeWindow(event);
    } else if (IsBackspacePressed(event)) {
      std::string data_str = input_text_.getString();
      input_text_.setString(data_str.empty() ? data_str : data_str.substr(0, data_str.size() - 1));
    } else if (IsDigitInput(event)) {
      input_text_.setString(input_text_.getString() + static_cast<char>(event.text.unicode));
    } else if (IsMouseClicked(event)) {
      return HandleMouseButtonPressed(sf::Mouse::getPosition(window_));
    }
  }

  return {NOTHING};
}

UserChoice Window::HandleMouseButtonPressed(sf::Vector2i cursor_position) {
  if (add_process_.getGlobalBounds().contains(static_cast<float>(cursor_position.x),
                                              static_cast<float>(cursor_position.y))) {
    int time = std::stoi(input_text_.getString().toAnsiString());

    if (time < 0) {
      return {NOTHING};
    }

    return {ADD_PROCESS, time};
  } else if (reset_.getGlobalBounds().contains(static_cast<float>(cursor_position.x),
                                               static_cast<float>(cursor_position.y))) {
    return {RESET};
  }

  return {NOTHING};
}

void Window::Update(const std::vector<std::vector<bool>> &table) {
  Clear();

  window_.draw(add_process_border_);
  window_.draw(reset_border_);
  window_.draw(add_process_);
  window_.draw(reset_);
  window_.draw(average_timeout_label_);
  window_.draw(average_runtime_label_);
  window_.draw(input_field_);
  window_.draw(input_text_);

//  average_timeout_label_.setString(
//      EnglishLabels::MIDDLE_TIMEOUT.data() + std::to_string(static_cast<double>(current_timeout_)));
//  average_runtime_label_.setString(
//      EnglishLabels::MIDDLE_RUNTIME.data() + std::to_string(static_cast<double>(current_runtime_)));

  DrawTable(table);

  window_.display();
}

void Window::DrawTable(const std::vector<std::vector<bool>> &table) {
//  int x = ApplicationConstants::MARGIN_LEFT;
//  int y = ApplicationConstants::MARGIN_TOP;

  for (auto &process : table) {
    for (auto time_slice_status : process) {
      auto color = (time_slice_status ? sf::Color::Green : sf::Color::Red);

      time_slice_shape_.setFillColor(color);
//      time_slice_shape_.setPosition(static_cast<float>(x), static_cast<float>(y));

//      x += ApplicationConstants::CELL_SIZE;

      window_.draw(time_slice_shape_);
    }

//    x = ApplicationConstants::MARGIN_LEFT;
//    y += ApplicationConstants::CELL_SIZE;
  }
}

void Window::ResizeWindow(sf::Event event) {
  window_.setView({sf::Vector2f(event.size.width / 2.0, event.size.height / 2.0),
                   sf::Vector2f(event.size.width, event.size.height)});
}

void Window::Clear() {
  window_.clear(sf::Color::Black);
}

void Window::SetRuntime(double runtime) {
  current_runtime_ = runtime;
}

void Window::SetTimeout(double timeout) {
  current_timeout_ = timeout;
}

bool Window::IsDigitInput(const sf::Event &event) {
  int backspace_code = 8;
  int ascii_scope = 128;

  return ((event.type == sf::Event::TextEntered) &&
      (event.text.unicode < ascii_scope && event.text.unicode != backspace_code) &&
      std::isdigit(static_cast<char>(event.text.unicode)));
}

bool Window::IsBackspacePressed(const sf::Event &event) {
  return (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace);
}

bool Window::IsMouseClicked(const sf::Event &event) {
  return (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left);
}

}  // namespace gui
