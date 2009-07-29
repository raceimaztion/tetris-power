/*
  Score module
  Contains score-keeping methods, score widget, and should probably handle highscore list
  
  Player gets points for the following:
    100 points	For removing a row.
    150 points	When the block triggering a set of row removals is completely removed as well.
*/

// Find the current score
long scoreGetScore();
// Tell the score system that a block was removed. TODO: Add a parameter for block type
void scoreRowRemoved(int x, int y);
//
