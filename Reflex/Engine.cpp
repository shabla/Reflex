#include "Engine.hpp"

Engine::Engine()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 6;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	_window.create( sf::VideoMode(W_WIDTH, W_HEIGHT, 32), "Reflex", sf::Style::Close, settings );
	_window.setFramerateLimit(60);
	//_window.setVerticalSyncEnabled( true );
	//_window.ShowMouseCursor(false);

	_gameIsStarted = false;
	_gameOver = false;
	_currentRound = 1;
	_score = 0;
	_hits = 0;
	_miss = 0;
	_lives = 5;

	// Create a sprite for the background
    if(!_backgroundImage.loadFromFile("data/images/background.jpg"))
		std::cerr << "Couldn't load background.\n";
	_background.setTexture(_backgroundImage);

	// sprite for cursor
	if(!_cursorImage.loadFromFile("data/images/aim.png"))
		std::cerr << "Couldn't load cursor image.\n";
	//_cursorImage.createMaskFromColor(sf::Color::Black);
	_cursor.setTexture(_cursorImage);

	// load sound
	if (!_soundShootBuffer.loadFromFile("data/sounds/shoot.wav"))
		std::cerr << "Couldn't load sound shoot.wav.\n";
	_soundShoot.setBuffer(_soundShootBuffer);
	_soundShoot.setVolume(30);

	if (!_soundDieBuffer.loadFromFile("data/sounds/boing.mp3"))
		std::cerr << "Couldn't load sound die.wav.\n";
	_soundDie.setBuffer(_soundDieBuffer);
	//_soundShoot.SetVolume(30);

	// Load a font
	if(!_font.loadFromFile("data/neuropol.TTF"))
		std::cerr << "Couldn't load font.\n";

	_hudTextColor = sf::Color(255, 50, 50);

	// Set the start text
	setText(_startText, "Press LEFT MOUSE BUTTON to START A GAME\nPress SPACE to SHOW SCORES\nPress ESCAPE to QUIT", 26, 50, 250);
	setText(_gameOverText, "GAME OVER", 26, 250, 250);

	// Set the hud texts
	// Top
	setText(_hudHits, "Hits             : " + toStr(_hits), 20, 15, 10);
	setText(_hudMiss, "Miss           : " + toStr(_miss), 20, 15, 30);
	setText(_hudAccuracy, "Accuracy : 0", 20, 15, 50);
	setText(_hudRound, "Round : " + toStr(_currentRound), 20, 670, 10);
	setText(_hudLives, "Lives : " + toStr(_lives) + "/5", 20, 670, 30);

	// bottom
	setText(_hudTimeleft, "Time : 15", 26, 15, 560, sf::Text::Bold);
	setText(_hudScore, toStr(_score), 20, 680, 560);
}

void Engine::setText(sf::Text& text, const std::string value, 
					   float size, float offsetX, float offsetY, unsigned long style)
{
	text.setString(value);
	text.setFont(_font);
	text.setStyle(style);
	text.setColor(_hudTextColor);
	text.move(offsetX, offsetY);
}

void Engine::run()
{
	_window.draw(_background);
	_window.draw(_cursor);
	
	if( _gameIsStarted )
	{
		float elapsedSeconds = _clock.getElapsedTime().asSeconds();

		_hudTimeleft.setString( "Time : " + toStr(5.0f - elapsedSeconds ) );

		_window.draw(_hudHits);
		_window.draw(_hudMiss);
		_window.draw(_hudAccuracy);
		_window.draw(_hudRound);
		_window.draw(_hudLives);
		_window.draw(_hudScore);
		_window.draw(_hudTimeleft);

		_roundTime = elapsedSeconds;
		// si une round est en cours
		if( _roundTime < 5 )
		{
			// créer autant de cible que le niveau actuel
			if(_targets.size() < _currentRound)
				_targets.push_back(new Target());
		}
		else
		{
			// sinon, démarre une nouvelle round
			_currentRound++;
			_hudRound.setString( "Round : " + toStr(_currentRound) );

			_lives = 5;
			_hudLives.setString( "Lives : " + toStr(_lives) + "/5" );

			_score += 100;
			_hudScore.setString( toStr(_score) );

			_clock.restart();
		}

		// affiche chaque cibles
		for(unsigned int i = 0; i < _targets.size(); i++)
		{
			// si la cible n'a pas été cliqué à temps
			if(!_targets.at(i)->draw(_window, _clock))
			{
				_targets.erase(_targets.begin() + i);
				
				_score -= 200;
				_hudScore.setString( toStr(_score) );

				_lives--;
				_hudLives.setString( "Lives : " + toStr(_lives) + "/5" );
				if( _lives < 1 )
				{
					gameOver();
					break;
				}
			}
		}
	}
	else
	{
		if( _gameOver )
			_window.draw(_gameOverText);
		else
			_window.draw(_startText);
	}

	_window.display();
}

bool Engine::eventManager()
{
	sf::Event e;
	while(_window.pollEvent(e))
	{
		switch(e.type)
		{
		case sf::Event::Closed:
			return false;
			break;
		
		case sf::Event::KeyPressed:
			// Event for each keys
			switch(e.key.code)
			{
			case sf::Keyboard::Escape:
				return false;
				break;

			case sf::Keyboard::S:
				std::cerr << "Show scores\n";
				break;
			}
			break;

		case sf::Event::MouseButtonPressed:
			// si la partie n'est pas en cours
			if( !_gameIsStarted )
			{
				_gameIsStarted = true;
				_gameOver = false;
				_clock.restart();
				srand(time(0));  // Initialize random number generator.
			}
			else if( !_gameIsStarted && _gameOver )
			{
				_gameOver = false;
			}
			else
			{
				_soundShoot.play();

				// pour chaque cibles
				for(unsigned int i=0; i<_targets.size(); i++)
				{
					// trouver la distance entre l'endroit du clic et le centre de la cible
					float centreX  = _targets[i]->getSprite().getPosition().x;
					float centreY  = _targets[i]->getSprite().getPosition().y;
					float rayon    = centreX / 2;
					float deltaX   = e.mouseButton.x - centreX;
					float deltaY   = e.mouseButton.y - centreY;
					float distance = sqrt( pow(deltaX, 2) + pow(deltaY,2) );
					
					// si la distance est inférieur au rayon de la cible ( qui est circulaire )
					if( distance <= rayon )
					{
						_soundDie.play();
						// destruction de la cible
						_targets.erase(_targets.begin() + i);

						// modification des stats
						_hits++;
						_hudHits.setString( "Hits             : " + toStr(_hits) );

						if( distance < rayon/2 )
							_score += 80;
						else
							_score += 50;
						_hudScore.setString( toStr(_score) );

						// fin forcé de la boucle, sinon les cibles superposé sont toutes 
						// détruites en même temps
						break;
					}
					
					// si ont est rendu à la vérification de dernière cible et qu'on
					// est encore dans le boucle, c'est qu'aucune cible n'a été touchée
					// donc malus dans les scores
					if(i == _targets.size()-1)
					{
						_miss++;
						_hudMiss.setString( "Miss           : " + toStr(_miss) );

						_score -= 75;
						sf::Text test;
						test.setString("test");
						//test.setString( "-75", 12, _targets.at(i)->getSprite().getPosition().x, _targets.at(i)->getSprite().getPosition().x, 0 );
						_window.draw(test);
						_hudScore.setString( toStr(_score) );

						_lives--;
						_hudLives.setString( "Lives : " + toStr(_lives) + "/5" );
						if( _lives < 1 )
						{
							gameOver();
							break;
						}
					}
				}
			}
			break;

		/*case sf::Event::MouseMoved:
			_cursor.SetX(e.MouseMove.X - _cursor.GetSize().x/2);
			_cursor.SetY(e.MouseMove.Y - _cursor.GetSize().y/2);
			break;*/
		}
	}

	return true;
}

void Engine::gameOver()
{
	_gameIsStarted = false;
	_gameOver = true;

	for( int i=0; i<_targets.size(); i++ )
		_targets.erase(_targets.begin() + i);

	_currentRound = 1;
	_hits = 0;
	_miss = 0;
	_lives = 5;
	_score = 0;
}