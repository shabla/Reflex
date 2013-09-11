#ifndef DEF_TARGET_H
#define DEF_TARGET_H

#include "Imports.hpp"

class Target
{
	private:
		sf::Texture _targetImage;
		sf::Sprite _target;
		float _lifeTime;
		
	public:
		Target();
		bool draw( sf::RenderWindow& window, sf::Clock& c );
		sf::Sprite getSprite() { return _target; };
};

#endif