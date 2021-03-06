#include "sfml_text_input.h"
#include "sfml_resources.h"

#include <iostream>
#include <cassert>

sfml_text_input::sfml_text_input(const double x, const double y,
                         const double height, const double width)
  : m_x{x}, m_y{y}, m_height{height}, m_width{width},
    m_selected{false}, m_str_size{0}, m_limit{55},
    m_text_color(255, 255, 255), m_timer{0}, m_pos{0}
{
  m_shape.setSize(sf::Vector2f(m_width,m_height));
  m_shape.setPosition(sf::Vector2f(m_x,m_y));
  m_shape.setFillColor(sf::Color(53,234,151));

  m_text.setFont(sfml_resources::get().get_default_font());

  m_text.setString("|");
  sf::FloatRect bounds = m_text.getLocalBounds();
  m_text.setOrigin(bounds.left + bounds.width/2.0f,
                     bounds.top  + bounds.height/2.0f);
  m_text.setString("");
  
  //m_view = sf::View(m_shape.getLocalBounds()); TODO view
}

void sfml_text_input::set_pos(double x, double y, sf::RenderWindow& window) {
  m_x = x;
  m_y = y;
  m_shape.setPosition(window.mapPixelToCoords(
      sf::Vector2i(m_x, m_y)));

  m_text.setPosition(window.mapPixelToCoords(
      sf::Vector2i(m_x + (m_width / 2), m_y + (m_height / 2))));
}

void sfml_text_input::set_size(double width, double height, sf::RenderWindow& window) {
  m_width = width;
  m_height = height;
  m_shape.setSize(sf::Vector2f(m_width, m_height));

  m_text.setPosition(window.mapPixelToCoords(
      sf::Vector2i(m_x + (m_width / 2), m_y + (m_height / 2))));
}

/* TODO view
void sfml_text_input::update_view() {
  m_view.setViewport(m_shape.getLocalBounds());
  m_view.setCenter(0, 0);
}
*/

void sfml_text_input::set_string(const std::string str, sf::RenderWindow& window) {
  m_string = str;
  m_text.setString(str);
  sf::FloatRect bounds = m_text.getLocalBounds();
//  if (m_str_size == 0 && m_string.size() == 1) {
//    m_text.setOrigin(bounds.left + bounds.width/2.0f,
//                     bounds.top  + bounds.height/2.0f);
//  } else {
//    m_text.setOrigin(bounds.left + bounds.width/2.0f,
//                     m_text.getOrigin().y);
//  }
  m_text.setOrigin(bounds.left + bounds.width/2.0f, 
                      m_text.getOrigin().y);
//
  m_text.setPosition(window.mapPixelToCoords(
      sf::Vector2i(m_x + (m_width / 2), m_y + (m_height / 2))));
  m_str_size = m_string.size();
}

void sfml_text_input::select(const sf::RenderWindow& window) {
  sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window));
  double x = mouse.x;
  double y = mouse.y;
//  assert(event.type == sf::Event::MouseButtonPressed);
  m_selected = x > m_x && x < m_x + m_width &&
               y > m_y && y < m_y + m_height;
  m_timer = 300;
  if (m_selected) {
    m_pos = m_string.size();
  }
}

/* The delete key doesn't seem to work
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) &&
               static_cast<unsigned>(m_pos) < m_string.size()) {
      m_string.erase(m_pos, 1);
    } 
*/

void sfml_text_input::input(const sf::Event& event, sf::RenderWindow& window) {
  assert(event.type == sf::Event::TextEntered);
  if (m_selected) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
      m_selected = false;
    } else if (event.text.unicode == '\b' && m_string.size() > 0 && m_pos > 0) {
      //m_string.resize(m_string.size() - 1);
      m_string.erase(m_pos - 1, 1);
      m_pos--;
    } else if (is_normal_char(event.text.unicode) &&
               static_cast<int>(m_string.size()) < m_limit) {
      std::string s;
      s += static_cast<char>(event.text.unicode);
      m_string.insert(m_pos, s);
      m_pos++;
    }
    set_string(m_string, window);
    m_timer = 150;
  }
  assert(static_cast<unsigned int>(m_pos) <= m_string.size());
}

sf::RectangleShape &sfml_text_input::get_shape() {
  return m_shape;
}

sf::Text &sfml_text_input::get_text() {
  return m_text;
}

void sfml_text_input::update() {
  if (m_timer >= 150) {
    std::string tmp = m_string;
    tmp.insert(m_pos, "|");
    m_text.setString(tmp);
  }
  if (m_timer >= 300) {
    m_text.setString(m_string);
    m_timer = 0;
  }

  if (m_selected) {
    m_shape.setFillColor(m_select_color);
#ifndef CI
    m_text.setFillColor(m_text_color);
#endif
    ++m_timer;
  } else {
    m_shape.setFillColor(m_color);
    if (m_timer != 0) m_timer = 300;
  }
}

void sfml_text_input::left() {
  if (m_selected && m_pos > 0) {
    m_pos--;
    m_timer = 150;
  }
}

void sfml_text_input::right() {
  if (m_selected && static_cast<unsigned int>(m_pos) < m_string.size()) {
    m_pos++;
    m_timer = 150;
  }
}

void sfml_text_input::set_color(sf::Color c) {
  m_color = c;
//  float high = 1.30f;
  float low = 0.70f;
//  if ((c.r * high > 255 && c.g * high > 255) ||
//      (c.g * high > 255 && c.b * high > 255) ||
//      (c.b * high > 255 && c.g * high > 255)) {
    if (c.r * low < 0) c.r = 0;
    if (c.g * low < 0) c.g = 0;
    if (c.b * low < 0) c.b = 0;
    m_select_color = sf::Color(c.r * low, c.g * low, c.b * low, c.a);
//  } else {
//    if (c.r * high > 255) c.r = 255/high;
//    if (c.g * high > 255) c.g = 255/high;
//    if (c.b * high > 255) c.b = 255/high;
//    m_select_color = sf::Color(c.r * high, c.g * high, c.b * high, c.a);
//  }
}

bool is_normal_char(uint32_t c) { //!OCLINT
  return (c > 47 && c < 58) ||
         (c > 64 && c < 91) ||
         (c > 96 && c < 123) ||
         (c > 39 && c < 42) ||
         (c == ';') ||
         (c == ' ') ||
         (c == ',');
}

void test_normal_char() { //!OCLINT tests can be complex
  assert(is_normal_char('a'));
  assert(is_normal_char('z'));
  assert(is_normal_char('0'));
  assert(is_normal_char('9'));
  assert(is_normal_char('A'));
  assert(is_normal_char('Z'));
  assert(is_normal_char('G'));
  assert(is_normal_char('g'));
  assert(is_normal_char('1'));
  assert(is_normal_char('5'));
  assert(is_normal_char(' '));
  assert(is_normal_char('('));
  assert(is_normal_char(';'));

  assert(!is_normal_char('{'));
  assert(!is_normal_char('\\'));
  assert(!is_normal_char('\b'));
  assert(!is_normal_char('\n'));
  assert(!is_normal_char('`'));
  assert(!is_normal_char('*'));
  assert(!is_normal_char('|'));
}
