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

// The ScoreView widget
class ScoreView : public Label {
  protected:
    Score* score;
    
  public:
    ScoreView(int x, int y, int width, int height, Colour c, Font* font, Score* score);
    ScoreView(const ScoreView& sv);
    
    void timerTick(float dTime);
};

class HighscoreEntry {
  private:
    long score;
    string name, date;
  
  public:
    HighscoreEntry(long score, string name, string date);
    HighscoreEntry(const HighscoreEntry& he);
    
    long getScore() const;
    string getName() const;
    string getDate() const;
    
    bool operator< (const HighscoreEntry& he) const;
    bool operator> (const HighscoreEntry& he) const;
    bool operator<= (const HighscoreEntry& he) const;
    bool operator>= (const HighscoreEntry& he) const;
    bool operator== (const HighscoreEntry& he) const;
    bool operator!= (const HighscoreEntry& he) const;
};

// The Highscore-keeping class
class Highscore {
  private:
    set<HighscoreEntry> entries;
    sqlite3 *db;
    
    int insertEntry(HighscoreEntry& he);
  
  public:
    Highscore();
    Highscore(const Highscore& h);
    ~Highscore();
    
    // Returns -1 if entry didn't make it, otherwise the index of the new entry
    int addEntry(HighscoreEntry& he);
    // Get the entry at the specified index
    HighscoreEntry getEntry(int index) const;
    // Clear all entries
    void clearEntries();
};

// The HighscoreView class
class HighscoreView : public Panel {
  private:
    Highscore* highscore;
  
  public:
    HighscoreView(int x, int y, int width, int height, Colour c, Font* font, Highscore* highscore);
};

