/*
  Score module
  Contains score-keeping methods, score widget, and should probably handle highscore list
  
  Player gets points for the following:
    100 points	For removing a row.
    150 points	When the block triggering a set of row removals is completely removed as well.
*/

// Score-keeping class
class Score {
  private:
    long score;
  
  public:
    Score();
    Score(const Score& s);
    
    long getScore() const;
    void addScore(int amount);
};

/*
  The ScoreView widget
*/

class ScoreView : public Label {
  protected:
    Score* score;
    
  public:
    ScoreView(int x, int y, int width, int height, Colour c, Font* font, Score* score);
    ScoreView(const ScoreView& sv);
    
    void timerTick(float dTime);
};

