#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct point
{
  double x, y;

  // Graham Scan Functions - To get the convex hull of a set
  friend bool compare_by_coordinates(point a, point b)
  {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
  }
  friend bool is_clockwise(point a, point b, point c)
  {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
  }
  friend bool is_counterclockwise(point a, point b, point c)
  {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
  }
};

void convex_hull(vector<point> &points)
{
  if (points.size() == 1)
    return;
  sort(points.begin(), points.end(), [](point a, point b) { return compare_by_coordinates(a, b); });
  point p1 = points[0], p2 = points.back();
  vector<point> up, down;
  up.push_back(p1);
  down.push_back(p1);
  for (int i = 1; i < (int)points.size(); i++)
  {
    if (i == points.size() - 1 || is_clockwise(p1, points[i], p2))
    {
      while (up.size() >= 2 && !is_clockwise(up[up.size() - 2], up[up.size() - 1], points[i]))
        up.pop_back();
      up.push_back(points[i]);
    }
    if (i == points.size() - 1 || is_counterclockwise(p1, points[i], p2))
    {
      while (down.size() >= 2 && !is_counterclockwise(down[down.size() - 2], down[down.size() - 1], points[i]))
        down.pop_back();
      down.push_back(points[i]);
    }
  }

  points.clear();
  for (int i = 0; i < (int)up.size(); i++)
    points.push_back(up[i]);
  for (int i = down.size() - 2; i > 0; i--)
    points.push_back(down[i]);
}