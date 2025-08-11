#pragma once


#include <Windows.h>
#include <vector>
#include <string>


class UCIEngine {
public:
    UCIEngine();
    ~UCIEngine();

    std::vector<std::string> getMoves(const std::string& fen);
    std::string getBestMove(const std::string& fen, uint32_t level);
    std::string move(const std::string& fen, const std::string& move);
    bool check(const std::string& fen);
private:
    STARTUPINFO sti = { 0 };
    SECURITY_ATTRIBUTES sats = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
    BYTE buffer[2048];
    DWORD writ, excode, read, available;

    void write(const std::string& msg);
    std::string readUntil(const std::string& marker);
};