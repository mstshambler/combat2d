#include "game.h"

using namespace std;

Player::Player() {
	Reset();
}

Player::~Player() {
	gates.clear();
}

void Player::GetPos(float *x, float *y) const {
	if (x)
		*x = pos[0];
	if (y)
		*y = pos[1];
}

void Player::SetPos(const float x, const float y) {
	pos[0] = x;
	pos[1] = y;
}

float Player::GetAngle() const {
	return angle;
}

void Player::SetAngle(const float f) {
	angle = f;
}

int Player::GetScore() const {
	return score;
}

void Player::SetScore(const int i) {
	score = i;
}

byte Player::GetStatus() const {
	return status;
}

void Player::SetStatus(const byte b) {
	status = b;
}

list<struct GateHit *> *Player::GetGates() {
	return &gates;
}

void Player::Reset() {
	pos[0] = pos[1] = -1.0f;
	score = 0;
	status = PlayerStatus::Player_OK;
	gates.clear();
	angle = 0.0f;
}


Game::Game() {
	gameField = new byte[512 * 512];
	gatesField = new int[512 * 512];
	startTime.tv_sec = startTime.tv_usec = 0;
}

Game::~Game() {
	delete gameField;
	delete gatesField;
}

void Game::Reset() {
	memset(gameField, 0, sizeof(byte) * 512 * 512);
	memset(gatesField, 0, sizeof(int)* 512 * 512);
	
	players[0].Reset();
	players[1].Reset();

	players[0].SetPos(1.0f, 251.0f);
	players[0].SetAngle(0.0f);

	players[1].SetPos(502.0f, 251.0f);
	players[1].SetAngle(180.0f);

	fillGates = 0;

	currentGate = 0;

	decayFields.clear();
}

byte *Game::GetGameField() {
	return gameField;
}

int *Game::GetGatesField() {
	return gatesField;
}

void Game::GetStartTime(struct timeval *t) const {
	if (t) {
		t->tv_sec = startTime.tv_sec;
		t->tv_usec = startTime.tv_usec;
	}
}

void Game::SetStartTime(const struct timeval t) {
	startTime.tv_sec = t.tv_sec;
	startTime.tv_usec = t.tv_usec;
}

Player *Game::GetPlayer(const int i) {
	if (i < 0 || i > 1)
		return NULL;
	return &players[i];
}

byte Game::GetFillGates() const {
	return fillGates;
}

void Game::SetFillGates(const byte b) {
	fillGates = b;
}

list<struct GateHit *> *Game::GetDecayFields() {
	return &decayFields;
}

int Game::GetCurrentGate() const {
	return currentGate;
}

void Game::SetCurrentGate(const int i) {
	currentGate = i;
}