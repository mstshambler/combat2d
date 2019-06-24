#include "game.h"

using namespace std;

Game::Game() {
	exitGame = 0;
}

Game::~Game() {

}

void Game::SetExitGame(const byte &b) {
	exitGame = b;
}

byte Game::GetExitGame() const {
	return exitGame;
}
