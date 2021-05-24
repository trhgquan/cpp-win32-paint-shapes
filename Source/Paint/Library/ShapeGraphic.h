#pragma once

/// <summary>
/// Graphics of a shape.
/// </summary>
class ShapeGraphic {
private:
  int _lineStyle;
  int _lineWidth;
  COLORREF _lineColour;
  COLORREF _backgroundColour;

public:
  ShapeGraphic() {
    // Do nothing.
  }

  ShapeGraphic(int lineStyle, int lineWidth,
    COLORREF lineColour, COLORREF backgroundColour) {
    _lineStyle = lineStyle;
    _lineWidth = lineWidth;
    _lineColour = lineColour;
    _backgroundColour = backgroundColour;
  }

  ~ShapeGraphic() {
    // Do nothing.
  }

public:
  int lineStyle() { return _lineStyle; }
  int lineWidth() { return _lineWidth; }
  COLORREF lineColour() { return _lineColour; }
  COLORREF backgroundColour() { return _backgroundColour; }

  void setLineStyle(int lineStyle) {
    _lineStyle = lineStyle;
  }
  void setLineWidth(int lineWidth) {
    _lineWidth = lineWidth;
  }
  void setLineColour(COLORREF lineColour) { 
    _lineColour = lineColour; 
  }
  void setBackgroundColour(COLORREF backgroundColour) {
    _backgroundColour = backgroundColour;
  }

public:
  /// <summary>
  /// Output overload.
  /// </summary>
  /// <param name="out"></param>
  /// <param name="sg"></param>
  /// <returns></returns>
  friend std::ostream& operator<<(std::ostream& out, ShapeGraphic& sg) {
    out << sg.toString();
    return out;
  }

  /// <summary>
  /// Convert ShapeGraphic to a string.
  /// </summary>
  /// <returns></returns>
  std::string toString() {
    std::stringstream builder;

    builder << _lineStyle;
    builder << ",";
    builder << _lineWidth;
    builder << ",";
    builder << _lineColour;
    builder << ",";
    builder << _backgroundColour;

    return builder.str();
  }

  /// <summary>
  /// Parse a string to a ShapeGraphic.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  static ShapeGraphic parse(const std::string& buffer) {
    std::vector<std::string> tokens = Tokeniser::split(buffer, ",");

    return ShapeGraphic(
      stoi(tokens[0]),
      stoi(tokens[1]),
      stoi(tokens[2]),
      stoi(tokens[3])
    );
  }
};