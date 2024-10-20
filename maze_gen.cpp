#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

#define RESET           "\x1b[0m"
#define BOLD            "\x1b[1m"
#define RED             "\x1b[31m"
#define GREEN           "\x1b[32m"
#define YELLOW          "\x1b[33m"
#define CLEAR_SCREEN    "\x1b[2J"
#define CURSOR_TO_TOP   "\x1b[H"
#define HIDE_CURSOR     "\x1b[?25l"
#define SHOW_CURSOR     "\x1b[?25h"

void draw_maze(const vector<vector<char>> &maze) {
    cout << CURSOR_TO_TOP << HIDE_CURSOR;
    for (const auto &row : maze) {
        for (char cell : row) {
            if(cell == '.') {
                cout << BOLD RED << cell << RESET << " ";
            }
            else if(cell == '#') {
                cout << BOLD YELLOW << cell << RESET << " ";
            }
            else {
                cout << BOLD GREEN << cell << RESET << " ";
            }
        }
        cout << endl;
    }
}

void rat_solve(int row, int col, vector<vector<char>> &maze, vector<vector<bool>> &vis) {
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vis[row][col] = true;
    draw_maze(maze);

    for (auto dir : directions) {
        int new_row = row + dir.first;
        int new_col = col + dir.second;

        if (new_row >= 0 && new_row < maze.size() && new_col >= 0 && new_col < maze[0].size() && (maze[new_row][new_col] == ' ' || maze[new_row][new_col] == '#') && !vis[new_row][new_col]) {
            if (maze[new_row][new_col] == '#') {
                cout << BOLD GREEN << "Rat found the treasure!" << RESET << endl;
                exit(0);
            }

            maze[new_row][new_col] = '.';
            rat_solve(new_row, new_col, maze, vis);

            maze[new_row][new_col] = ' ';
            draw_maze(maze);
        }
    }
    vis[row][col] = false;
}

void dfs(vector<vector<char>> &maze, vector<vector<bool>> &vis, int row, int col) {
    maze[row][col] = ' ';
    vis[row][col] = true;
    vector<pair<int, int>> directions = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    random_shuffle(directions.begin(), directions.end());
    // draw_maze(maze);

    for (auto dir : directions) {
        int new_row = row + dir.first;
        int new_col = col + dir.second;

        if (new_row > 0 && new_row < maze.size() - 1 && new_col > 0 && new_col < maze[0].size() - 1 && !vis[new_row][new_col]) {
            maze[row + dir.first / 2][col + dir.second / 2] = ' ';
            dfs(maze, vis, new_row, new_col);
        }
    }
}

void generate_maze(vector<vector<char>> &maze) {
    int init_row = 1;
    int init_col = 1;
    int height = maze.size();
    int width = maze[0].size();
    vector<vector<bool>> vis(height, vector<bool>(width, false));

    dfs(maze, vis, init_row, init_col);
}

int main(int argc, char **argv) {
    srand(static_cast<unsigned int>(time(0)));
    int width, height;

    if (argc != 3) {
        cout << "Usage: ./maze_generator <height> <width>" << endl;
        return 1;
    }

    try {
        height = stoi(argv[1]);
        width = stoi(argv[2]);

        if (height <= 0 || width <= 0) {
            cout << "Height and width must be positive integers." << endl;
            return 1;
        }
    } catch (const invalid_argument &) {
        cout << "Invalid input. Please enter integer values." << endl;
        return 1;
    } catch (const out_of_range &) {
        cout << "Input value out of range." << endl;
        return 1;
    }

    cout << BOLD << GREEN;
    vector<vector<char>> maze(height * 2 + 1, vector<char>(2 * width + 1, '*'));
    maze[1][0] = ' ';
    maze[height * 2 - 1][2 * width] = '#';
    generate_maze(maze);

    system("cls");
    cout << "Maze: " << endl;
    draw_maze(maze);

    vector<vector<bool>> vis(maze.size(), vector<bool>(maze[0].size(), false));
    vis[1][0] = true;
    maze[1][0] = '.';
    rat_solve(1, 0, maze, vis);

    cout << RESET SHOW_CURSOR << endl;

    return 0;
}
