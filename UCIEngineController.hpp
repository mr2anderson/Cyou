#pragma once


#include <memory>
#include <atomic>
#include <mutex>


class Board;
class UCIEngine;
class SoundManager;
enum State;
class UCIEngineLevel;


class UCIEngineController {
public:
	UCIEngineController(std::shared_ptr<Board> board, std::shared_ptr<State> state, std::shared_ptr<UCIEngine> uci, std::shared_ptr<SoundManager> soundManager, std::shared_ptr<bool> playerToMove, std::shared_ptr<const UCIEngineLevel> level);
	void setupMove();
	void process();
private:
	std::atomic<bool> needToMove;
	
	std::atomic<bool> thinking;
	std::string pendingMove;
	std::atomic<bool> moveReady;
	std::mutex mtx;
	
	std::shared_ptr<Board> board;
	std::shared_ptr<State> state;
	std::shared_ptr<UCIEngine> uci;
	std::shared_ptr<SoundManager> soundManager;
	std::shared_ptr<bool> playerToMove;
	std::shared_ptr<const UCIEngineLevel> level;
};