//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size) {

    if (player < 1 || player > 2){
        throw ClientWrongPlayerNumberException();
    }

    this->board_size = board_size;
    this->player = player;

    string playerNum = to_string(player);
    string actionBoard = "player_" + playerNum + ".action_board.json";

    vector<int> across(board_size, 0);
    vector<vector<int>> coord(board_size, across);
    ofstream file;
    file.open(actionBoard, ofstream::out);

    if (file) {
        cereal::JSONOutputArchive arc(file);
        arc(CEREAL_NVP(coord));

        file.close();
        initialized = true;
    }

    else{
        cout << "Can't find the file." << endl;
    }
}


void Client::fire(unsigned int x, unsigned int y) {
    string playerNum = to_string(player);
    string shotBoard = "player_"+playerNum+".shot.json";

    ofstream file;
    {
        file.open(shotBoard, ofstream::out);
        cereal::JSONOutputArchive arc(file);
        arc(CEREAL_NVP(x),CEREAL_NVP(y));
    }
    file.flush();
}


bool Client::result_available() {
    string playerNum = to_string(player);
    string resultFile = "player_"+playerNum+".result.json";

    ifstream check;
    check.open(resultFile);

    if(!check){
        return false;
    }
    else{
        return true;
    }
}


int Client::get_result() {
    if(result_available() == true){

        string playerNum = to_string(player);
        string resultFile = "player_"+playerNum+".result.json";

        ifstream check;
        check.open(resultFile);
        cereal::JSONInputArchive arcin(check);

        int final = 0;
        arcin(final);

        check.close();
        remove(resultFile.c_str());

        if(final <= 1 && final >= -1){
            return final;
        }
        else{
            throw ClientException("Incorrect Final Number.");
        }
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    vector<int> across(board_size, 0);
    vector<vector<int>> coord(board_size, across);

    string playerNum = to_string(player);
    string actionBoard = "player_" + playerNum + ".action_board.json";

    coord[x][y] = result;

    ofstream finalout;
    finalout.open(actionBoard);

    if(finalout.good()){
        {
            cereal::JSONOutputArchive arcin(finalout);
            arcin(CEREAL_NVP(coord));
        }
    }
}


string Client::render_action_board(){
    vector<int> across(board_size, 0);
    vector<vector<int>> coord(board_size, across);

    string playerNum = to_string(player);
    string actionBoard = "player_" + playerNum + ".action_board.json";

    ifstream check;
    check.open(actionBoard);

    if(check.good()){
        cereal::JSONInputArchive arc(check);
        arc(coord);
    }
    // Final board output we will be returning as a string
    string endLine;

    for (int i = 0; i < board_size; ++i) {
        for (int j = 0; j < board_size; ++j) {
            string result = to_string(coord[i][j]);
            endLine += result;
        }
        endLine += "\n";
    }
    return endLine;
}
