#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct unit
{
    int x;
    int y;
};

struct tetromino
{
    char type;
    vector<unit> vec_units;
    int width;
    int height;
};

enum tetromino_type
{
    Q = 0, 
    Z, 
    S, 
    T, 
    I, 
    L, 
    J
};

tetromino vec_tetrominoes[] = 
{
    {'Q', {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, 2, 2},
    {'Z', {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, 3, 2},
    {'S', {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, 3, 2},
    {'T', {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, 3, 2},
    {'I', {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, 4, 1},
    {'L', {{0, 0}, {1, 0}, {1, 1}, {1, 2}}, 2, 3}, 
    {'J', {{0, 0}, {1, 0}, {1, 1}, {1, 2}}, 2, 3}
};

class PlayGround
{
private:
    static const int MAX_WIDTH = 10;
    static const int MAX_HEIGHT = 100;
    int grid[MAX_WIDTH][MAX_HEIGHT] = {{0}};

public:
    void clear_row(int h)
    {
        for(int x = 0; x < MAX_WIDTH; ++x)
        {
            grid[x][h] = 0;
        }
    }
    
    void clear_grid()
    {
        for(int x = 0; x < MAX_WIDTH; ++x)
        {
            for(int y = 0; y < MAX_HEIGHT; ++y)
            {
                grid[x][y] = 0;
            }
        }
    }

    int max_height_2(int start, int end)
    {
        int maxH = 0;
        for (int col = start; col < end; ++col)
        {
            int temp_y = 0;
            for (int i = MAX_HEIGHT - 1; i >= 0; --i)
            {
                if (grid[col][i] == 1)
                {
                    temp_y = i + 1;
                    break;
                }
            }
            maxH = max(maxH, temp_y);
        }
        return maxH;
    }

    int occupied_max_grid_ht(int start, int end)
    {
        int maxH = 0;
        for (int col = start; col < end; ++col)
        {
            maxH = max(maxH, height(col));
        }
        return maxH;
    }

    int height(int col)
    {
        for (int i = MAX_HEIGHT - 1; i >= 0; --i)
        {
            if (grid[col][i] == 1)
                return i + 1;
        }
        return 0;
    }

    bool collision(int h, const tetromino &tetromino, int col)
    {
        for (auto &unit : tetromino.vec_units)
        {
            if (grid[col + unit.x][h + unit.y] == 1)
            {
                return true;
            }
        }
        return false;
    }

    void update_grid(int h, const tetromino &tetromino, int col)
    {
        for (auto &unit : tetromino.vec_units)
        {
            grid[col + unit.x][h + unit.y] = 1;
        }
    }

    void drop_tetromino(const tetromino &tetromino, int col)
    {
        int approx_height = occupied_max_grid_ht(col, col + tetromino.width);
        int stable_height = approx_height;
        bool one_more_drop = true;

        if(tetromino.type == 'T' || tetromino.type == 'S' || tetromino.type == 'Z')
        {
            for (int y = approx_height; y >= 0; --y)
            {
                if(collision(y, tetromino, col))
                {
                    one_more_drop = false;
                    break;
                }
                stable_height = y;
            }
        }
        update_grid(stable_height, tetromino, col);
        clear_rows();
    }

    void clear_rows()
    {
        for (int y = 0; y < MAX_HEIGHT; ++y)
        {
            bool is_row_full = true;
            for (int x = 0; x < MAX_WIDTH; ++x)
            {
                if(grid[x][y] == 0)
                {
                   is_row_full = false; 
                   break; 
                }
            }
            if (is_row_full)
            {
                for (int x = 0; x < MAX_WIDTH; ++x)
                {
                    for (int y_ = y; y_ < MAX_HEIGHT - 1; ++y_)
                    {
                        grid[x][y_] = grid[x][y_ + 1];
                    }
                    grid[x][MAX_HEIGHT - 1] = 0;
                }
            }
        }
    }

    int finaliseResult(const string &line)
    {
        istringstream iss(line);
        string type_and_pos;
        while (getline(iss, type_and_pos, ','))
        {
            if (type_and_pos.empty())
                continue;

            char type = type_and_pos[0];
            int col = stoi(type_and_pos.substr(1));

            switch(type)
            {
                case 'Q': drop_tetromino(vec_tetrominoes[Q], col); break;
                case 'Z': drop_tetromino(vec_tetrominoes[Z], col); break;
                case 'S': drop_tetromino(vec_tetrominoes[S], col); break;
                case 'T': drop_tetromino(vec_tetrominoes[T], col); break;
                case 'I': drop_tetromino(vec_tetrominoes[I], col); break;
                case 'L': drop_tetromino(vec_tetrominoes[L], col); break;
                case 'J': drop_tetromino(vec_tetrominoes[J], col); break;
            }
        }
        return occupied_max_grid_ht(0, MAX_WIDTH);
    }
};

int main()
{
    PlayGround grid;
    string line;

    while (getline(cin, line)) 
    {
        cout << grid.finaliseResult(line) << endl; 
        grid.clear_grid();
    }

    return 0;
}
