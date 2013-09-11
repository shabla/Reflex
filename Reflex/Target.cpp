#include "Target.hpp"

Target::Target()
{
	// Create a sprite for the background
    if(!_targetImage.loadFromFile("data/images/target.png"))
		std::cerr << "Couldn't load target image.\n";
	_target.setTexture(_targetImage);

	// met le centre du sprite au centre de l'image (donc sa position = au centre de l'image)
	_target.setPosition( _target.getLocalBounds().width/2, _target.getLocalBounds().height/2 );
	
	int randomX = randomInt( _target.getLocalBounds().width, W_WIDTH - _target.getLocalBounds().width );
	int randomY = randomInt( _target.getLocalBounds().height, W_HEIGHT - _target.getLocalBounds().height );

	_target.setPosition( randomX, randomY );

	_lifeTime = 0.f;
}

bool Target::draw( sf::RenderWindow& window, sf::Clock& c )
{
	_lifeTime += c.getElapsedTime().asMilliseconds();
	c.restart();

	if( _lifeTime <= 6.f )
	{
		if( _lifeTime <= 3.f )
			_target.setScale(_lifeTime/3,_lifeTime/3);
		else
			_target.setScale(1 - (_lifeTime / 3 - 1),1 - (_lifeTime / 3 - 1));

		window.draw(_target);
	}
	else
		return false;

	return true;
}