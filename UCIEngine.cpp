#include "UCIEngine.hpp"
#include "GlobalRandomGenerator.hpp"
#include "ChessUtils.hpp"
#include "OSTools.hpp"
#include <iostream>
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <array>
#include <sstream>
#include <chrono>


static constexpr uint32_t MAX_LEVEL = 10;
static constexpr uint32_t DEPTH_MID[MAX_LEVEL + 1] = { 1, 1, 1, 1, 2, 3, 4, 5, 6, 8 };
static constexpr uint32_t ERROR_PROBABILITY[MAX_LEVEL + 1] = {100, 80, 70, 60, 50, 40, 30, 20, 10, 0};
static constexpr uint32_t ERROR_DELTA[MAX_LEVEL + 1] = { 300, 250, 200, 150, 100, 75, 50, 25, 10, 0};


UCIEngine::UCIEngine() {
    // Initialize handles to NULL
    pipin_w = pipin_r = pipout_w = pipout_r = NULL;
    ZeroMemory(&sti, sizeof(sti));
    sti.cb = sizeof(sti);
    ZeroMemory(&pi, sizeof(pi));

    // Set security attributes for the pipes
    sats.nLength = sizeof(sats);
    sats.bInheritHandle = TRUE;
    sats.lpSecurityDescriptor = NULL;

    // Create pipes for input and output
    CreatePipe(&pipout_r, &pipout_w, &sats, 0);
    CreatePipe(&pipin_r, &pipin_w, &sats, 0);

    SetHandleInformation(pipin_w, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(pipout_r, HANDLE_FLAG_INHERIT, 0);

    // Set up the STARTUPINFO structure to redirect standard input, output, and error
    sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    sti.wShowWindow = SW_HIDE; // Hide the window
    sti.hStdInput = pipin_r;   // Redirect standard input to pipin_r
    sti.hStdOutput = pipout_w; // Redirect standard output to pipout_w
    sti.hStdError = pipout_w;  // Redirect standard error to pipout_w

    // Create the process, with redirection of handles set up in STARTUPINFO
    if (!CreateProcess(L"resources/stockfish.exe", NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sti, &pi)) {
        OSTools::error(L"Не удалось запустить Stockfish");
    }

    CloseHandle(pipin_r);
    pipin_r = NULL;
    CloseHandle(pipout_w);
    pipout_w = NULL;

    write("uci\n");
    readUntil("uciok");
    write("isready\n");
    readUntil("readyok");
    write("setoption name MultiPV value 10\n");
}
UCIEngine::~UCIEngine() {
    write("quit\n");

    // Close all open handles
    if (pipin_w != NULL) CloseHandle(pipin_w);
    if (pipin_r != NULL) CloseHandle(pipin_r);
    if (pipout_w != NULL) CloseHandle(pipout_w);
    if (pipout_r != NULL) CloseHandle(pipout_r);
    if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
    if (pi.hThread != NULL) CloseHandle(pi.hThread);
}
std::vector<std::string> UCIEngine::getMoves(const std::string& fen) {
    write("ucinewgame\nposition fen " + fen + "\ngo perft 1\nisready\n");
    std::stringstream ss(readUntil("readyok") + readUntil("\n"));
    std::vector<std::string> result;
    std::string line;
    while (std::getline(ss, line)) {
        if (line.find(": 1") != std::string::npos) {
            std::stringstream ss2(line);
            std::string move;
            std::getline(ss2, move, ':');
            result.push_back(move);
        }
    }
    return result;
}
std::string UCIEngine::getBestMove(const std::string& fen, uint32_t level) {
    if (level > MAX_LEVEL) {
        level = MAX_LEVEL;
    }
    uint32_t depth = DEPTH_MID[level];
    std::array<std::array<char, 8>, 8> representation = ChessUtils::getArray(fen);
    uint32_t cnt = 0;
    for (uint8_t y = 0; y < 8; y = y + 1) {
        for (uint8_t x = 0; x < 8; x = x + 1) {
            cnt += (representation[y][x] != ' ');
        }
    }
    if (cnt < 8) {
        depth = depth * 3;
        if (depth < 12) {
            depth = 12;
        }
    }
    else if (cnt < 14) {
        depth = depth * 2;
    }
    std::cout << "UCIEngine debug\ndepth: " << depth << "\n";
    write("ucinewgame\nposition fen " + fen + "\ngo depth " + std::to_string(depth) + "\nisready\n");
    std::stringstream ss(readUntil("bestmove"));
    std::vector<std::tuple<std::string, int>> moves;
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream ss2(line);
        std::string word;
        std::vector<std::string> words;
        while (ss2 >> word) {
            words.push_back(word);
        }
        if (words.size() >= 6 and words[0] == "info" and words[1] == "depth" and words[2] == std::to_string(depth)) {
            auto it1 = std::find(words.begin(), words.end(), "score");
            auto it2 = std::find(words.begin(), words.end(), "pv");
            int32_t score = std::stoi(*(it1 + 2));
            if (*(it1 + 1) == "mate") {
                score = score * 10000;
            }
            moves.emplace_back(*(it2 + 1), score);
        }
    }
    if (moves.empty()) {
        return "(none)";
    }
    if (GlobalRandomGenerator::get().gen(ERROR_PROBABILITY[level])) {
        std::cout << "planned mistake\n";
        std::vector<std::string> mistakeMoves;
        for (const auto& mv : moves) {
            int32_t mistake = std::abs((int32_t)std::get<1>(mv) - (int32_t)std::get<1>(moves[0]));
            if (mistake < ERROR_DELTA[level]) {
                mistakeMoves.push_back(std::get<0>(mv));
                std::cout << "move " << std::get<0>(mv) << " mistake " << mistake << "\n";
            }
        }
        std::string mv = mistakeMoves[GlobalRandomGenerator::get().gen() % mistakeMoves.size()];
        std::cout << "result " << mv << "\n" << std::endl;
        return mv;
    }
    std::cout << "no planned mistake\n" << std::endl;
    return std::get<0>(moves[0]);
}
std::string UCIEngine::move(const std::string& fen, const std::string& move) {
    write("ucinewgame\nposition fen " + fen + " moves " + move + "\nd\n");
    std::stringstream ss(readUntil("Key"));
    std::string line;
    for (; ;) {
        std::getline(ss, line);
        if (line.find("Fen") != std::string::npos) {
            std::stringstream ss2(line);
            std::string trash, shortFen, side, castling, enPassant, k1, k2;
            ss2 >> trash >> shortFen >> side >> castling >> enPassant >> k1 >> k2;
            return shortFen + " " + side + " " + castling + " " + enPassant + " " + k1 + " " + k2;
        }
    }
}
bool UCIEngine::check(const std::string& fen) {
    write("ucinewgame\nposition fen " + fen + "\nd\nisready\n");
    std::stringstream ss(readUntil("readyok"));
    std::string line;
    for (; ;) {
        std::getline(ss, line);
        if (line.find("Checkers") != std::string::npos) {
            std::stringstream ss2(line);
            std::string field;
            ss2 >> field;
            field.clear();
            ss2 >> field;
            return !field.empty();
        }
    }
}
void UCIEngine::write(const std::string& msg) {
    DWORD w;
    WriteFile(pipin_w, msg.c_str(), (DWORD)msg.size(), &w, NULL);
}
std::string UCIEngine::readUntil(const std::string& marker) {
    auto n1 = std::chrono::system_clock::now();
    auto d1 = n1.time_since_epoch();
    auto m1 = std::chrono::duration_cast<std::chrono::milliseconds>(d1).count();
    std::string acc;
    DWORD r;
    char buf[4096];
    for (;;) {
        if (!ReadFile(pipout_r, buf, sizeof(buf) - 1, &r, NULL) || r == 0) break;
        buf[r] = 0; acc += buf;
        if (acc.find(marker) != std::string::npos) break;
        auto n2 = std::chrono::system_clock::now();
        auto d2 = n2.time_since_epoch();
        auto m2 = std::chrono::duration_cast<std::chrono::milliseconds>(d2).count();
        if (m2 - m1 > 5000) {
            std::cout << "UCIEngine debug\nreadUntil\nmarker " << marker << "\ncontent\n" << acc << std::endl;
            OSTools::error(L"Произошла ошибка общения со Stockfish. Просмотрите терминал для деталей");
        }
    }
    return acc;
}