/*
  Score module
  Contains score-keeping methods, score widget, and should probably handle highscore list
  
  Player gets points for the following:
    100 points	For removing a row.
    150 points	When the block triggering a set of row removals is completely removed as well.
*/

// Find the current score
long scoreGetScore();
// Add the given amount to the current score
void scoreAdd(int amount);

/*
  The ScoreView widget
*/

class ScoreView : public Label {
  public:
    ScoreView(int x, int y, int width, int height, Colour c, Font* font);
    ScoreView(const ScoreView& sv);
    
    void timerTick(float dTime);
};

