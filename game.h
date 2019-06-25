#ifndef _game_h
#define _game_h

#include "common.h"
#include "gettimeofday.h"

class Game {
protected:
	byte exitGame;

public:
	Game();
	~Game();

	void SetExitGame(const byte &b);
	byte GetExitGame() const;
};

#endif