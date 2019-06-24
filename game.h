#ifndef _game_h
#define _game_h

#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#endif

#include <list>
#include "common.h"

#include "gettimeofday.h"

using namespace std;

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