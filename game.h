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

struct GateHit {
	struct timeval hitTime;
	int gateId;
};

class Player {
protected:
	float pos[2];
	float angle;
	int score;
	byte status;
	list<struct GateHit *> gates;

public:
	enum PlayerStatus {
		Player_OK = 0,
		Player_Dead = 1,
	};

	Player();
	~Player();

	void GetPos(float *x, float *y) const;
	void SetPos(const float x, const float y);

	float GetAngle() const;
	void SetAngle(const float f);

	int GetScore() const;
	void SetScore(const int i);

	byte GetStatus() const;
	void SetStatus(const byte b);

	list<struct GateHit *> *GetGates();

	void Reset();
};

class Game {
protected:
	byte *gameField;
	int *gatesField;
	struct timeval startTime;
	Player players[2];
	byte fillGates;
	list<struct GateHit *> decayFields;
	int currentGate;

public:
	Game();
	~Game();

	void Reset();

	byte *GetGameField();
	int *GetGatesField();

	void GetStartTime(struct timeval *t) const;
	void SetStartTime(const struct timeval t);

	Player *GetPlayer(const int i);

	byte GetFillGates() const;
	void SetFillGates(const byte b);

	list<struct GateHit *> *GetDecayFields();

	int GetCurrentGate() const;
	void SetCurrentGate(const int i);
};

#endif