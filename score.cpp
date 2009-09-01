#define SCORE_MODULE
#include "common.h"

#define NUM_HIGHSCORE_ENTRIES 10

/*
  Score class
*/
Score::Score()
{
  score = 0L;
}

Score::Score(const Score& s)
{
  score = s.score;
}

long Score::getScore() const
{
  return score;
}

void Score::addScore(int amount)
{
  score += amount;
}

/*
  ScoreView widget
*/
ScoreView::ScoreView(int x, int y, int width, int height, Colour c, Font* font, Score* score) : Label(x, y, width, height, c, "", font)
{
  // Nothing much to do here
  this->score = score;
}

ScoreView::ScoreView(const ScoreView& sv) : Label(sv)
{
  // Nothing to do here
}

void ScoreView::timerTick(float dTime)
{
  if (score == NULL)
  {
    if (label == "0")
      return;
    
    label = "0";
    repaint();
  }
  else
  {
    stringstream sout;
    sout << "Score: " << score->getScore();
    string score = sout.str();
    if (score != label)
    {
      label = score;
      repaint();
    }
  }
}

/*
  HighscoreEntry class
*/
HighscoreEntry::HighscoreEntry(long score, string name, string date)
{
  this->score = score;
  this->name = name;
  this->date = date;
}

HighscoreEntry::HighscoreEntry(const HighscoreEntry& he)
{
  score = he.score;
  name = he.name;
  date = he.date;
}

long HighscoreEntry::getScore() const
{
  return score;
}

string HighscoreEntry::getName() const
{
  return name;
}

string HighscoreEntry::getDate() const
{
  return date;
}

bool HighscoreEntry::operator< (const HighscoreEntry& he) const
{
  return score < he.score;
}

bool HighscoreEntry::operator> (const HighscoreEntry& he) const
{
  return score > he.score;
}

bool HighscoreEntry::operator<= (const HighscoreEntry& he) const
{
  return score <= he.score;
}

bool HighscoreEntry::operator>= (const HighscoreEntry& he) const
{
  return score >= he.score;
}

bool HighscoreEntry::operator== (const HighscoreEntry& he) const
{
  return score == he.score && name == he.name && date == he.date;
}

bool HighscoreEntry::operator!= (const HighscoreEntry& he) const
{
  return !(*this == he);
}

/*
  Highscore-keeping class
*/
Highscore::Highscore()
{
  // TODO: Open and load score database
  int result = sqlite3_open("highscores.db", &db);
  if (result != SQLITE_OK)
  {
    printf("Highscore::Highscore(): Failed to open score database in 'highscores.db'! Highscore table will be empty.\n");
    printf("Error was:\n%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    db = NULL;
  }
  else
  {
    char* error_msg = NULL;
    result = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Highscores { scoreID INTEGER PRIMARY KEY, scoreNumber INTEGER, scoreName TEXT, scoreDate TEXT };", NULL, NULL, &error_msg);
    if (result != SQLITE_OK)
    {
      printf("Highscore::Highscore(): Failed to create highscore table in database file 'highscores.db'. Highscore table will be empty.\n");
      printf("Error was:\n%s\n", error_msg);
      sqlite3_free(error_msg);
      sqlite3_close(db);
      db = NULL;
    }
    
    {
      char** results;
      int numRows, numCols;
      result = sqlite3_get_table(db, "SELECT scoreID, scoreNumber, scoreName, scoreDate FROM Highscores ORDER BY scoreNumber;", &results, &numRows, &numCols, &error_msg);
      
      int indexID=0, indexNumber=1, indexName=2, indexDate=3;
      for (int i=0; i < numCols; i++)
      {
        if (strcmp(results[i], "scoreID") == 0)  indexID = i;
        else if (strcmp(results[i], "scoreNumber") == 0)  indexNumber = i;
        else if (strcmp(results[i], "scoreName") == 0)  indexName = i;
        else if (strcmp(results[i], "scoreDate") == 0)  indexDate = i;
      }
      // Process the table data
      for (int row=0; row < numRows; row++)
      {
        HighscoreEntry e(atol(results[numRows*(1+row)+indexNumber]),
                              string(results[numRows*(1+row)+indexName]),
                              string(results[numRows*(1+row)+indexDate]));
        insertEntry(e);
      }
      
      sqlite3_free_table(results);
    }
  }
} // end Highscore() constructor

Highscore::Highscore(const Highscore& h) : entries(h.entries)
{
  // Nothing much to do here (yet)
  // TODO: Figure out how to copy a link to an SQLite database
}

Highscore::~Highscore()
{
  if (db != NULL)
  {
    sqlite3_close(db);
    db = NULL;
  }
}

int Highscore::insertEntry(HighscoreEntry& he)
{
  if ((*entries.begin()) > he)
    return -1;
  else
  {
    entries.insert(he);
    if (entries.size() >NUM_HIGHSCORE_ENTRIES)
      entries.erase(entries.begin());
    
    int index=0;
    for (set<HighscoreEntry>::const_iterator cur = entries.begin();
                                             cur != entries.end() && (*cur) != he;
                                             cur++,index++) ;
    return index;
  }
}

int Highscore::addEntry(HighscoreEntry& he)
{
  // TODO: Insert entry (if applicable), update the database, and return the new index
  int index = insertEntry(he);
  
  if (index >= 0 && db != NULL)
  {
    char* error_msg;
    sqlite3_exec(db, "DELETE FROM Highscores WHERE scoreID = {SELECT scoreID FROM Highscores WHERE scoreNumber = {SELECT MAX(scoreNumber) FROM Highscores}};", NULL, NULL, &error_msg);
    if (error_msg != NULL)
      sqlite3_free(error_msg);
    
    ostringstream oss;
    oss << "INSERT INTO Highscores (scoreNumber, scoreName, scoreDate) VALUES (";
    oss << he.getScore() << ", '" << he.getName() << "', '" << he.getDate() << "');";
    
    sqlite3_exec(db, oss.str().c_str(), NULL, NULL, &error_msg);
    if (error_msg != NULL)
      sqlite3_free(error_msg);
  }
  
  return index;
}

HighscoreEntry Highscore::getEntry(int index) const
{
  set<HighscoreEntry>::iterator cur = entries.begin();
  while (index > 0)
  {
    index--;
    cur++;
  }
  return *cur;
}

void Highscore::clearEntries()
{
  entries.clear();
  // TODO: Empty the database
}

