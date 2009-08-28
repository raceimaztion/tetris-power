#define SCORE_MODULE
#include "common.h"

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

/*
  Highscore-keeping class
*/
Highscore::Highscore()
{
  // TODO: Open and load score database
}

Highscore::Highscore(const Highscore& h) : entries(h.entries)
{
  // Nothing much to do here (yet)
}

int Highscore::addEntry(HighscoreEntry& he)
{
  return -1;
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
}

