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
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){

}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    if (board_size == 0 || board_size != BOARD_SIZE){
        throw ServerException("Player board is the wrong size.");
    }
    else if (p1_setup_board.compare("") == 0 || p2_setup_board.compare("") == 0){
        throw ServerException("Need boards for players");
    }
    else{
        this -> board_size=board_size;
    }
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    string playerNum = to_string(player);
    string setupBoard = "player_"+playerNum+".setup_board.txt";

    int a = 0;

    if (player < 1 || player > 2){
        throw ServerException("Need a valid Player # (1 or 2)");
    }

    else if ((x < 0 || x > board_size) || (y < 0 || y > board_size)){
        return OUT_OF_BOUNDS;
    }

    ifstream check;
    check.open(setupBoard);

    string boardLine;
    vector<string> lines(board_size, "");
    while(getline(check, boardLine)){
        lines[a] = boardLine;
        cout << lines[a] << endl;
        a++;
    }
    check.close();

    char location = lines[x].at(y);
    if(location=='B' || location=='C' || location=='D' || location=='R' || location=='S'){
        return HIT;
    }
    else{
        return MISS;
    }


}


int Server::process_shot(unsigned int player) {
    if (player < 1 || player > 2) {
        throw ServerException("Player number incorrect");
    } else {
        string playerNum = to_string(player);
        string shotFile = "player_" + playerNum + ".shot.json";

        int x, y, result;
        std::ifstream file;
        file.open(shotFile);
        if (file) {
            cereal::JSONInputArchive fin(file);
            fin(y, x);
            if ((y < board_size && y >= 0) && (x < board_size && x >= 0)) {
                result = evaluate_shot(player, x, y);
            } else {
                result = OUT_OF_BOUNDS;
            }
        } else {
            return NO_SHOT_FILE;
        }

        string resultFile = "player_" + playerNum + ".result.json";
        ofstream finalout;
        finalout.open(resultFile);
        {
            cereal::JSONOutputArchive archive(finalout);
            archive(CEREAL_NVP(result));
        }
        remove(shotFile.c_str());
        return SHOT_FILE_PROCESSED;
    }
}
