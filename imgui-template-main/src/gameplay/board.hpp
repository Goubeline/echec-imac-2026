#pragma once
#include <vector>



class Board {
public:
    Board(); 

    
    int getPieceAt(int row, int col) const; 

private:
 
    int grid[8][8];
};