#include <tools/scales.h>
#include <game/game.h>

Scales::Scales()
{
}

Scales::~Scales()
{
}

int Scales::setup(Game *game)
{
	this->_gamePtr = game;

	this->_scaleFactors[static_cast<int>(DrawPriority::UNIVERSE)] = this->_gamePtr->settings.get("GameUniverseScale")->_double;

	this->_scaleFactors[static_cast<int>(DrawPriority::GALACTIC)] = this->_gamePtr->settings.get("GameGalacticScale")->_double;

	this->_scaleFactors[static_cast<int>(DrawPriority::STELLAR)] = this->_gamePtr->settings.get("GameStellarScale")->_double;

	this->_scaleFactors[static_cast<int>(DrawPriority::PLANETARY)] = this->_gamePtr->settings.get("GamePlanetaryScale")->_double;

	this->_scaleFactors[static_cast<int>(DrawPriority::SURFACE)] = this->_gamePtr->settings.get("GameSurfaceScale")->_double;

	this->_one = "1.0";
}
