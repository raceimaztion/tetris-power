#define SCORE_MODULE
#include "common.h"

// Private score amount
long _currentScore = 0;

long scoreGetScore()
{
  return _currentScore;
}

void scoreAdd(int amount)
{
  _currentScore += amount;
}

/*
  ScoreView widget
*/
ScoreView::ScoreView(int x, int y, int width, int height, Colour c, Font* font) : Label(x, y, width, height, c, "", font)
{
  // Nothing to do here
}

ScoreView::ScoreView(const ScoreView& sv) : Label(sv)
{
  // Nothing to do here
}

void ScoreView::timerTick(float dTime)
{
  stringstream sout;
  sout << "Score: " << _currentScore;
  string score = sout.str();
  if (score != label)
  {
    label = score;
    triggerRepaint();
  }
}
