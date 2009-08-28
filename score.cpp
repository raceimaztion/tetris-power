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
