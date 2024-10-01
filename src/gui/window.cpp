#include "window.h"
#include <variant>

namespace gui {

Window::Window(const std::string& title)
    : window_({ApplicationConstants::kWidth, ApplicationConstants::kHeight}, title) {
  Init();
}

void Window::Init() {
  InitWindow();
  InitLabels();
  InitInputFields();
  InitResultLabels();
  InitButtons();
}

void Window::InitWindow() {
  window_.setFramerateLimit(144);

  if (!font_.loadFromFile(ApplicationConstants::kPathToFont.data())) {
    throw std::runtime_error("Failed to load font!");
  }
}

void Window::InitLabels() {
  results_zone_label_.setFont(font_);
  results_zone_label_.setCharacterSize(30);
  results_zone_label_.setFillColor(sf::Color::Magenta);
  results_zone_label_.setString(Labels::kResults.data());

  input_zone_label_.setFont(font_);
  input_zone_label_.setCharacterSize(30);
  input_zone_label_.setFillColor(sf::Color::Magenta);
  input_zone_label_.setString(Labels::kInput.data());

  ip_range_label_.setFont(font_);
  ip_range_label_.setCharacterSize(25);
  ip_range_label_.setFillColor(sf::Color::Cyan);
  ip_range_label_.setString(Labels::kRange.data());

  network_parameters_label_.setFont(font_);
  network_parameters_label_.setCharacterSize(25);
  network_parameters_label_.setFillColor(sf::Color::Cyan);
  network_parameters_label_.setString(Labels::kNetworkParameters.data());
}

void Window::InitInputFields() {
  start_ip_range_label_.setFont(font_);
  start_ip_range_label_.setCharacterSize(25);
  start_ip_range_label_.setFillColor(sf::Color::White);
  start_ip_range_label_.setString(Labels::kStart.data());

  finish_ip_range_label_.setFont(font_);
  finish_ip_range_label_.setCharacterSize(25);
  finish_ip_range_label_.setFillColor(sf::Color::White);
  finish_ip_range_label_.setString(Labels::kFinish.data());

  start_input_field_.setFillColor(sf::Color::Black);
  start_input_field_.setOutlineThickness(2);
  start_input_field_.setOutlineColor(sf::Color::White);

  finish_input_field_.setFillColor(sf::Color::Black);
  finish_input_field_.setOutlineThickness(2);
  finish_input_field_.setOutlineColor(sf::Color::White);

  start_input_text_.setFont(font_);
  start_input_text_.setCharacterSize(25);
  start_input_text_.setFillColor(sf::Color::White);

  finish_input_text_.setFont(font_);
  finish_input_text_.setCharacterSize(25);
  finish_input_text_.setFillColor(sf::Color::White);
}

void Window::InitResultLabels() {
  network_address_result_label_.setFont(font_);
  network_address_result_label_.setCharacterSize(25);
  network_address_result_label_.setFillColor(sf::Color::White);
  network_address_result_label_.setString(Labels::kNetworkAddress.data());

  broadcast_address_result_label_.setFont(font_);
  broadcast_address_result_label_.setCharacterSize(25);
  broadcast_address_result_label_.setFillColor(sf::Color::White);
  broadcast_address_result_label_.setString(Labels::kBroadcastAddress.data());

  mac_address_result_label_.setFont(font_);
  mac_address_result_label_.setCharacterSize(25);
  mac_address_result_label_.setFillColor(sf::Color::White);
  mac_address_result_label_.setString(Labels::kMacAddress.data());

  subnet_mask_result_label_.setFont(font_);
  subnet_mask_result_label_.setCharacterSize(25);
  subnet_mask_result_label_.setFillColor(sf::Color::White);
  subnet_mask_result_label_.setString(Labels::kSubnetMask.data());
}

void Window::InitButtons() {
  calculate_button_label_.setFont(font_);
  calculate_button_label_.setCharacterSize(35);
  calculate_button_label_.setFillColor(sf::Color::Black);
  calculate_button_label_.setString(Labels::kCalculate.data());
  calculate_button_label_.setOutlineColor(sf::Color::White);
  calculate_button_label_.setOutlineThickness(3);

  reset_button_label_.setFont(font_);
  reset_button_label_.setCharacterSize(35);
  reset_button_label_.setFillColor(sf::Color::Black);
  reset_button_label_.setString(Labels::kReset.data());
  reset_button_label_.setOutlineColor(sf::Color::White);
  reset_button_label_.setOutlineThickness(3);
}

bool Window::IsOpen() const noexcept {
  return window_.isOpen();
}

UserChoice Window::Tick() {
  sf::Event event{};
  Update();

  if (window_.waitEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window_.close();
    } else if (event.type == sf::Event::Resized) {
      ResizeWindow(event);
    } else if (IsBackspacePressed(event)) {
      RemoveSymbol(event);
    } else if (IsDigitInput(event) || IsDotEntered(event)) {
      AddSymbol(event);
    } else if (IsMouseClicked(event)) {
      return HandleMouseButtonPressed(sf::Mouse::getPosition(window_));
    }
  }

  return {NOTHING};
}

void Window::RemoveSymbol(const sf::Event& event) {
  if (focus_on_start) {
    std::string data_str = start_input_text_.getString();
    start_input_text_.setString(data_str.empty() ? data_str : data_str.substr(0, data_str.size() - 1));
  } else if (focus_on_finish) {
    std::string data_str = finish_input_text_.getString();
    finish_input_text_.setString(data_str.empty() ? data_str : data_str.substr(0, data_str.size() - 1));
  }
}

void Window::AddSymbol(const sf::Event& event) {
  if (focus_on_start) {
    start_input_text_.setString(start_input_text_.getString() + static_cast<char>(event.text.unicode));
  } else if (focus_on_finish) {
    finish_input_text_.setString(finish_input_text_.getString() + static_cast<char>(event.text.unicode));
  }
}

UserChoice Window::HandleMouseButtonPressed(sf::Vector2i cursor_position) {
  if (calculate_button_label_.getGlobalBounds().contains(static_cast<float>(cursor_position.x),
                                                         static_cast<float>(cursor_position.y))) {
    std::string start = start_input_text_.getString().toAnsiString();
    std::string finish = finish_input_text_.getString().toAnsiString();

    calc_button_pressed_ = true;
    focus_on_start = false;
    focus_on_finish = false;
    // TODO Проверить не пустые ли они
    return {CALCULATE, {start, finish}};
  } else if (reset_button_label_.getGlobalBounds().contains(static_cast<float>(cursor_position.x),
                                                            static_cast<float>(cursor_position.y))) {
    reset_button_pressed_ = true;
    focus_on_start = false;
    focus_on_finish = false;

    return {RESET};
  } else if (start_input_field_.getGlobalBounds().contains(static_cast<float>(cursor_position.x),
                                                           static_cast<float>(cursor_position.y))) {
    focus_on_start = true;
    focus_on_finish = false;
  } else if (finish_input_field_.getGlobalBounds().contains(static_cast<float>(cursor_position.x),
                                                            static_cast<float>(cursor_position.y))) {
    focus_on_start = false;
    focus_on_finish = true;
  }

  return {NOTHING};
}

void Window::Draw() {
  results_zone_label_.setPosition((window_.getSize().x - window_.getSize().x / 3) / 64 + window_.getSize().x / 3, 20);
  input_zone_label_.setPosition(window_.getSize().x / 64, 20);
  ip_range_label_.setPosition(window_.getSize().x / 12, 60);
  network_parameters_label_.setPosition((2 * window_.getSize().x / 3) / 2 + window_.getSize().x / 3, 60);

  start_ip_range_label_.setPosition(window_.getSize().x / 64, 120);
  finish_ip_range_label_.setPosition(window_.getSize().x / 64, 180);
  start_input_field_.setPosition(window_.getSize().x / 64 + 100, 120);
  start_input_field_.setSize(sf::Vector2f(window_.getSize().x / 3 - 150, 30));
  finish_input_field_.setPosition(window_.getSize().x / 64 + 100, 180);
  finish_input_field_.setSize(sf::Vector2f(window_.getSize().x / 3 - 150, 30));
  start_input_text_.setPosition(window_.getSize().x / 64 + 100, 120);
  finish_input_text_.setPosition(window_.getSize().x / 64 + 100, 180);

  calculate_button_label_.setPosition((window_.getSize().x / 3) / 12, window_.getSize().y - 70);
  reset_button_label_.setPosition((window_.getSize().x / 3) - (window_.getSize().x / 8), window_.getSize().y - 70);

  if (calc_button_pressed_) {
    calculate_button_label_.setOutlineColor(sf::Color::Red);
    calc_button_pressed_ = false;
    window_.draw(calculate_button_label_);
  } else {
    calculate_button_label_.setOutlineColor(sf::Color::White);
  }

  if (reset_button_pressed_) {
    reset_button_label_.setOutlineColor(sf::Color::Red);
    reset_button_pressed_ = false;
    window_.draw(reset_button_label_);
  } else {
    reset_button_label_.setOutlineColor(sf::Color::White);
  }

  network_address_result_label_.setPosition((window_.getSize().x / 3) + 20, 120);
  broadcast_address_result_label_.setPosition((window_.getSize().x / 3) + 20, 180);
  subnet_mask_result_label_.setPosition((window_.getSize().x / 3) + 20, 240);
  mac_address_result_label_.setPosition((window_.getSize().x / 3) + 20, 300);

  window_.draw(results_zone_label_);
  window_.draw(input_zone_label_);
  window_.draw(ip_range_label_);
  window_.draw(network_parameters_label_);

  window_.draw(start_ip_range_label_);
  window_.draw(finish_ip_range_label_);
  window_.draw(start_input_field_);
  window_.draw(finish_input_field_);
  window_.draw(start_input_text_);
  window_.draw(finish_input_text_);

  window_.draw(calculate_button_label_);
  window_.draw(reset_button_label_);

  window_.draw(network_address_result_label_);
  window_.draw(broadcast_address_result_label_);
  window_.draw(subnet_mask_result_label_);
  window_.draw(mac_address_result_label_);

  sf::Vertex vertical_1[] = {sf::Vertex(sf::Vector2f(window_.getSize().x / 3, 0)),
                             sf::Vertex(sf::Vector2f(window_.getSize().x / 3, window_.getSize().y))};

  sf::Vertex horizontal_1[] = {sf::Vertex(sf::Vector2f(0, window_.getSize().y - 110)),
                               sf::Vertex(sf::Vector2f(window_.getSize().x / 3, window_.getSize().y - 110))};

  sf::Vertex horizontal_2[] = {sf::Vertex(sf::Vector2f(0, 110)), sf::Vertex(sf::Vector2f(window_.getSize().x, 110))};

  window_.draw(vertical_1, 4, sf::Lines);
  window_.draw(horizontal_1, 4, sf::Lines);
  window_.draw(horizontal_2, 4, sf::Lines);
}

void Window::UpdateLabels() {
  network_address_result_label_.setString(std::string(Labels::kNetworkAddress.data()) + network_address_);
  broadcast_address_result_label_.setString(std::string(Labels::kBroadcastAddress.data()) + broadcast_address_);
  subnet_mask_result_label_.setString(std::string(Labels::kSubnetMask.data()) + subnet_mask_);
  mac_address_result_label_.setString(std::string(Labels::kMacAddress.data()) + mac_address_);
}

void Window::Update() {
  Clear();
  Draw();
  UpdateLabels();

  window_.display();
}

void Window::ResizeWindow(sf::Event event) {
  window_.setView({sf::Vector2f(event.size.width / 2.0, event.size.height / 2.0),
                   sf::Vector2f(event.size.width, event.size.height)});
}

void Window::Reset() {
  start_input_text_.setString("");
  finish_input_text_.setString("");
  network_address_.clear();
  broadcast_address_.clear();
  mac_address_.clear();
  subnet_mask_.clear();
}

void Window::Clear() {
  window_.clear(sf::Color::Black);
}

bool Window::IsDigitInput(const sf::Event& event) {
  int backspace_code = 8;
  int ascii_scope = 128;

  return ((event.type == sf::Event::TextEntered) &&
          (event.text.unicode < ascii_scope && event.text.unicode != backspace_code) &&
          std::isdigit(static_cast<char>(event.text.unicode)));
}

bool Window::IsDotEntered(const sf::Event& event) {
  int ascii_scope = 128;
  int dot_code = 46;

  return ((event.type == sf::Event::TextEntered) && (event.text.unicode < ascii_scope && event.text.unicode == 46));
}

bool Window::IsBackspacePressed(const sf::Event& event) {
  return (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace);
}

bool Window::IsMouseClicked(const sf::Event& event) {
  return (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left);
}

void Window::SetResult(std::string network_address, std::string broadcast_address, std::string mac_address,
                       std::string subnet_mask) {
  network_address_ = network_address;
  broadcast_address_ = broadcast_address;
  mac_address_ = mac_address;
  subnet_mask_ = subnet_mask;
}

}  // namespace gui
