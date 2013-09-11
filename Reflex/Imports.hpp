#ifndef DEF_IMPORTS_H
#define DEF_IMPORTS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#define W_WIDTH 800
#define W_HEIGHT 600

template <class T> std::string toStr( const T &t )
{
    std::ostringstream oss;
    oss << t;

    return std::string (oss.str());
}

inline int randomInt( int min, int max )
{
	return ( min + int( ((max-min)+1) * rand() / (RAND_MAX + 1.0)) );
}

#endif