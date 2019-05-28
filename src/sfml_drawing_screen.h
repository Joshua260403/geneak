#ifndef SFML_DRAWING_SCREEN_H
#define SFML_DRAWING_SCREEN_H

#include "game_state.h"
#include "sfml_resources.h"
#include "sfml_text_input.h"
#include "sfml_button.h"
#include <SFML/Graphics.hpp>

class sfml_drawing_screen
{
public:

    sfml_drawing_screen(int ca = -1, std::string newick = "");

    void exec();

    void process_event(sf::Event event);

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();
    void set_sizes();
    
    void draw_objects();

private:

    int close_at;
    
    sf::RenderWindow& m_window;

    sf::RectangleShape m_tool_bar;
    sf::RectangleShape m_drawing_area;
    
    sfml_text_input m_input;
    
	  sfml_button m_confirm;
    sf::Sprite m_draw_sprite;
	
    sf::View m_drawing_view;
    
    bool m_move_left;
    bool m_move_right;
    bool m_move_up;
    bool m_move_down;
    
    std::vector<sfml_line> m_tree_lines;
    std::vector<sf::Text> m_tree_text;
    
    void update_tree(std::string in);
    
};

#endif // SFML_DRAWING_SCREEN_H
