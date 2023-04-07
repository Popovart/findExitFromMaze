//
//  main.cpp
//  maze
//
//  Created by Dmitry Popov on 07.04.2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

const int out = 9;
const int start = 2;




vector<vector<pair<int,int>>> maze = {
    {{1,1}, {1,1}, {1,1}, {1,1}, {1,1}, {1,1}},
    {{1,1}, {0,0}, {0,0}, {1,1}, {0,0}, {1,1}},
    {{1,1}, {0,0}, {1,1}, {1,1}, {0,0}, {1,1}},
    {{1,1}, {0,0}, {0,0}, {1,1}, {0,0}, {1,1}},
    {{1,1}, {out,0}, {0,0}, {0,0}, {start,1}, {1,1}},
    {{1,1}, {1,1}, {1,1}, {1,1}, {1,1}, {1,1}},
};

//vector<vector<string>> resultMaze;

vector<vector<string>> resultMaze = {
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
};

void printMaze(vector<vector<pair<int,int>>> maze){
    for (const auto& row : maze) {
        for (const auto& val : row) {
            std::cout << "[" << val.first << "," << val.second << "]" << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printMaze(vector<vector<string>> maze){
    for (const auto& row : maze) {
        for (const auto& val : row) {
            std::cout << val;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//start settings



pair<int,int> getPos(vector<vector<pair<int,int>>> maze, int parametr){
    pair<int,int> pos;
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[0].size(); j++) {
            if(maze[i][j].first == parametr){
                pos = {i,j};
            }
        }
    }
    return pos;
};

pair<int,int> exitPos = getPos(maze, out);
pair<int,int> startPos = getPos(maze, start);
list<pair<int,int>> journey = {startPos};


void makeResultMaze(vector<vector<pair<int,int>>> maze, vector<vector<string>> &resultMaze){
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[0].size(); j++) {
            if(maze[i][j].first == 1 && maze[i][j].second == 1){
                resultMaze[i][j] = " - ";
            } else if (maze[i][j].first == 0 && maze[i][j].second == 0){
                resultMaze[i][j] = " 0 ";
            } else if (maze[i][j].first == out){
                resultMaze[i][j] = " E ";
            } else if (maze[i][j].first == start){
                resultMaze[i][j] = " S ";
            }
        }
    }
}

//int distToExit(pair<int,int> startPos, pair<int,int> exitPos){
//    return abs(exitPos.first-startPos.first)+abs(exitPos.second-startPos.second);
//}

bool posChecker(vector<vector<pair<int,int>>> maze, pair<int,int> pos){
    //check for walls and visited pos
    if( (maze[pos.first][pos.second].first == 1 && maze[pos.first][pos.second].second == 1) || maze[pos.first][pos.second].second == 1){
        return false;
    }
    return true;
};


pair<bool,pair<int,int>> exitIsNear(vector<vector<pair<int,int>>> maze, pair<int,int> pos, string &action){
    pair<bool,pair<int,int>> result;
    result.first = false;
    if(pos.first-1 == exitPos.first && pos.second == exitPos.second){
        pos.first = pos.first-1;
        action = " ↑ ";
        return {result.first = true, result.second = pos};
    } else if(pos.first == exitPos.first && pos.second+1 == exitPos.second){
        pos.second = pos.second + 1;
        action = "‎ → ";
        return {result.first = true, result.second = pos};
    } else if(pos.first+1 == exitPos.first && pos.second == exitPos.second){
        pos.first = pos.first + 1;
        action = "‎‎ ↓ ";
        return {result.first = true, result.second = pos};
    } else if(pos.first == exitPos.first && pos.second-1 == exitPos.second){
        pos.second = pos.second - 1;
        action = "‎ ← ";
        return {result.first = true, result.second = pos};
    }
    return result;
};

pair<int,int> changeAction(vector<vector<string>> &resultMaze, vector<vector<pair<int,int>>> &maze, list<pair<int,int>> &journey, pair<int,int> pos){
    
    pair<int,int> tempPos = pos;
    string action;
    
    // checking when the exit is near
    pair<bool,pair<int,int>> exitIsNearRes = exitIsNear(maze, pos, action);
    if(exitIsNearRes.first){
        resultMaze[tempPos.first][tempPos.second] = action;
        return exitIsNearRes.second;
    }
    
    // TOP, RIGHT, BOT, LEFT actions
    if(posChecker(maze, {pos.first-1, pos.second})){
        pos.first = pos.first-1;
        maze[pos.first][pos.second].second = 1;
        journey.push_front(pos);
        action = " ↑ ";
    } else if(posChecker(maze, {pos.first, pos.second+1})){
        pos.second = pos.second + 1;
        maze[pos.first][pos.second].second = 1;
        journey.push_front(pos);
        action = "‎ → ";
    } else if(posChecker(maze, {pos.first+1, pos.second})){
        pos.first = pos.first + 1;
        maze[pos.first][pos.second].second = 1;
        journey.push_front(pos);
        action = "‎‎ ↓ ";
    } else if(posChecker(maze, {pos.first, pos.second-1})){
        pos.second = pos.second - 1;
        maze[pos.first][pos.second].second = 1;
        journey.push_front(pos);
        action = "‎ ← ";
    }
    
    // when agent hasn't changed position
    if(tempPos == pos){
        if(journey.empty()){
            cout << "It's unpossible to find exit, try change start settings " << endl;
            exit(0);
        }
        tempPos = *journey.begin();
        //it means that this action is wrong way
        maze[pos.first][pos.second].second = 1;
        resultMaze[tempPos.first][tempPos.second] = " 0 ";
        journey.pop_front();
        
        return tempPos;
    }
    
    resultMaze[tempPos.first][tempPos.second] = action;
    
    
    return pos;
};



void findExit(vector<vector<string>> &resultMaze, vector<vector<pair<int,int>>> &maze, pair<int,int> startPos, pair<int,int> exitPos, list<pair<int,int>> journey){
    //int distance = distToExit(startPos, exitPos);
    pair<int,int> currentPos = startPos;
    while(currentPos != exitPos){
        currentPos = changeAction(resultMaze, maze, journey, currentPos);
    };
    
};


int main(int argc, const char * argv[]) {
    printMaze(maze);
    makeResultMaze(maze, resultMaze);
    printMaze(resultMaze);
    findExit(resultMaze, maze, startPos, exitPos, journey);
    printMaze(resultMaze);
    return 0;
}


