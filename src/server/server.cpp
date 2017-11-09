#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "../common/json.hpp"
#include "../common/minesweeper.h"
#include "server.h"

using boost::asio::ip::tcp;
using namespace std;
using json = nlohmann::json;

// split string functions 
template<typename Out>
void Split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string> Split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  Split(s, delim, std::back_inserter(elems));
  return elems;
}

string InfoMinefield(MineSweeper* m) {
  json info_minefield_json;
  info_minefield_json["win"] = m->isWin();
  info_minefield_json["game_end"] = m->isGameEnd();
  info_minefield_json["col"] = m->getCol();
  info_minefield_json["row"] = m->getRow();
  info_minefield_json["mine_num"] = m->getMineNum();
  info_minefield_json["minefield"] = m->getMineState();

  return info_minefield_json.dump();
}

void ScriptExecutor(string script, MineSweeper* m) {
  vector<string> tokens;
  tokens = Split(script, ' ');
  
  string command = tokens[0];
  int col = stoi(tokens[1]);
  int row = stoi(tokens[2]);
  
  if (m->getCol() < col && col < 0
      && m->getRow() < row && row < 0) {

    cout << "The command is out of bounds."
         << "Aborting the command " << "\"" << command << "\""<< endl;
  }
  else if (command == "E")
  {
    m->RevealSingleClick(col, row);
  }
  else if (command == "F")
  {
    m->setFlagTile(col, row, true);
  }
  else if (command == "D")
  {
    m->RevealDoubleClick(col, row);
  }
}

unique_ptr<MineSweeper> StartGameTransfer() {
  cout << "Server boot up on port 1234" << endl;
  try
  {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

    tcp::socket socket(io_service);
    acceptor.accept(socket);

    cout << "Client connected" << endl;

    int col, row, mine_num;
    int order_col, order_row;

    // read
    // @TODO: When making the client, make sure this part works as well.
    boost::array<char, 600> buf;
    boost::system::error_code error;
    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    auto from_client_string = string(buf.begin(), buf.begin() + len);
    auto from_client_json = json::parse(from_client_string);
    cout << "Message read:" << from_client_json << endl;
    
    col = from_client_json["col"];
    row = from_client_json["row"];
    mine_num = from_client_json["mine_num"];
    vector<string> scripts = from_client_json["scripts"];

    auto tokens = Split(scripts[0], ' ');
    order_col = stoi(tokens[1]);
    order_row = stoi(tokens[2]);

    unique_ptr<MineSweeper> m_return(new MineSweeper(col, row, mine_num));
    m_return->CreateMineField(order_col, order_row);

    // write
    cout << "writing messages..." << endl;
    
    auto message = InfoMinefield(m_return.get());
    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    
    return move(m_return);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void InGameTransfer(MineSweeper* m) {
  cout << "Server boot up on port 1234" << endl;
  try {
    // boot up
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

    tcp::socket socket(io_service);
    acceptor.accept(socket);

    cout << "Client connected" << endl;

    // read
    boost::array<char, 600> buf;
    boost::system::error_code error;
    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    auto from_client_string = string(buf.begin(), buf.begin() + len);
    auto from_client_json = json::parse(from_client_string);
    cout << "Message read:" << from_client_string << endl;
    
    vector<string> scripts = from_client_json["scripts"];;

    for (auto& script : scripts)
    {
      ScriptExecutor(script, m);
    }

    // write
    cout << "writing messages..." << endl;

    auto message = InfoMinefield(m);
    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
