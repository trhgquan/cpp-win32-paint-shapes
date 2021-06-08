#pragma once

/// <summary>
/// Handle some geometric actions.
/// </summary>
namespace Geometric {
  /// <summary>
  /// Return the supposed standing in that shape's diagonal
  /// </summary>
  /// <param name="first"></param>
  /// <param name="second"></param>
  void diagonalStanding(const Point& first, Point& second) {
    int dx = first.x() - second.x();
    int dy = first.y() - second.y();
    int delta = max(abs(dx), abs(dy));

    // topLeft to rightBottom
    if (first.x() <= second.x() &&
        first.y() <= second.y()) {
      second.update(
        first.x() + delta,
        first.y() + delta
      );
    }

    // rightBottom to topLeft
    else if (first.x() > second.x() &&
      first.y() <= second.y()) {
      second.update(
        first.x() - delta,
        first.y() + delta
      );
    }

    // topRight to leftBottom
    else if (first.x() > second.x() &&
             first.y() > second.y()) {
      second.update(
        first.x() - delta,
        first.y() - delta
      );
    }

    // leftBottom to topRight
    else {
      second.update(
        first.x() + delta,
        first.y() - delta
      );
    }
  }

  /// <summary>
  /// Fixing to topLeft - rightBottom (shapes only, not line)
  /// </summary>
  /// <param name="shapeType"></param>
  /// <param name="first"></param>
  /// <param name="second"></param>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  void fixingPosition(int shapeType, 
    const Point& first, const Point& second, 
    Point& topLeft, Point& rightBottom) {
    if (!shapeType) {
      topLeft = first;
      rightBottom = second;

      return;
    }

    // leftTop to rightBottom
    if (first.x() <= second.x() &&
        first.y() <= second.y()) {
      topLeft = first;
      rightBottom = second;
    }

    // rightBottom to leftTop
    if (first.x() > second.x() &&
      first.y() > second.y()) {
      topLeft = second;
      rightBottom = first;
    }

    // leftBottom to rightTop
    if (first.x() <= second.x() &&
        first.y() > second.y()) {
      topLeft.update(first.x(), second.y());
      rightBottom.update(second.x(), first.y());
    }

    // rightTop to leftBottom
    if (first.x() > second.x() &&
      first.y() <= second.y()) {
      topLeft.update(second.x(), first.y());
      rightBottom.update(first.x(), second.y());
    }
  }
}