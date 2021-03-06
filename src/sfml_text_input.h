#ifndef SFML_TEXT_INPUT_H
#define SFML_TEXT_INPUT_H

#include "sfml_line.h"
#include <SFML/Graphics.hpp>

class sfml_text_input
{
public:

  /// @param The x coordinate of the text_input
  /// @param The y coordinate of the text_input
  /// @param The width of the text_input
  /// @param The heigth of the text_input
  sfml_text_input(const double x = 1.0, const double y = 1.0,
              const double height = 100.0, const double width = 100.0);

  /// Is the text_input clicked?
  void select(const sf::RenderWindow& window);

  void input(const sf::Event& event, sf::RenderWindow& window);

  sf::RectangleShape &get_shape();
  sf::Text &get_text();

  sf::Vector2f get_pos() noexcept { return sf::Vector2f(m_x, m_y); }
  sf::Vector2f get_size() noexcept { return sf::Vector2f(m_width, m_height); }

  void set_string(const std::string str, sf::RenderWindow& window);

  void set_pos(double x, double y, sf::RenderWindow& window);
  void set_size(double width, double height, sf::RenderWindow& window);

  bool is_selected() const noexcept { return m_selected; }

  void set_limit(int lim) { m_limit = lim; }

  void set_color(sf::Color c);

  void update();

  std::string get_string() const noexcept { return m_string; }
  
  void left();
  void right();
  
  void set_text_color(sf::Color c) { m_text_color = c; }

  /* TODO view
  sf::View &get_view() { return m_view; }
  
  void update_view();
  */
  
private:

  sf::RectangleShape m_shape;
  sf::Text m_text;
  std::string m_string;

  double m_x;
  double m_y;

  double m_height;
  double m_width;

  bool m_selected;

  int m_str_size;

  int m_limit;

  sf::Color m_color;
  sf::Color m_select_color;
  sf::Color m_text_color;
  
  int m_timer;

  int m_pos;
  
  //sf::View m_view; TODO view
  
};

bool is_normal_char(uint32_t c);

void test_normal_char();

#endif // SFML_TEXT_INPUT_H
