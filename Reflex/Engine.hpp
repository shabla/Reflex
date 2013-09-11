#ifndef DEF_ENGINE_H
#define DEF_ENGINE_H

#include "Imports.hpp"
#include "Target.hpp"

class Engine
{
	private:
		// Window configuration
		sf::RenderWindow _window;

		// HUD
		void setText(sf::Text& text, const std::string value, float size, 
					   float offsetX, float offsetY, unsigned long style = 0);
		sf::Texture _backgroundImage;
		sf::Sprite _background;
		sf::Texture _cursorImage;
		sf::Sprite _cursor;

		sf::Font _font;
		sf::Color _hudTextColor;
		sf::Text _startText;
		sf::Text _gameOverText;
		sf::Text _hudHits;
		sf::Text _hudMiss;
		sf::Text _hudAccuracy;
		sf::Text _hudRound;
		sf::Text _hudScore;
		sf::Text _hudLives;
		sf::Text _hudTimeleft;

		// Sound
		sf::SoundBuffer _soundShootBuffer;
		sf::Sound		_soundShoot;
		sf::SoundBuffer _soundDieBuffer;
		sf::Sound		_soundDie;

		// Game
		sf::Clock _clock;
		bool _gameIsStarted;
		bool _gameOver;
		unsigned int _currentRound;
		unsigned int _hits;
		unsigned int _miss;
		unsigned int _lives;
		float _roundTime;
		int _score;
		std::vector<Target*> _targets;
		

	public:
		Engine();
		void run();
		void gameOver();
		bool eventManager();
};

#endif