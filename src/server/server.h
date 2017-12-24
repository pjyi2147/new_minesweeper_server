#pragma once

// helpers
template<typename Out>
void Split(const std::string &s, char delim, Out result);
std::vector<std::string> Split(const std::string &s, char delim);
std::string InfoMinefield(MineSweeper* m);

void ScriptExecutor(std::string command, MineSweeper* m);

// main
unique_ptr<MineSweeper> StartGameTransfer();
void InGameTransfer(MineSweeper* m);