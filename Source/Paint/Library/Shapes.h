#pragma once

/// <summary>
/// Define a Point.
/// </summary>
class Point {
private:
  int _x;
  int _y;

public:
  Point() {
    _x = 0;
    _y = 0;
  }

  /// <summary>
  /// Parameterised constructor of Point
  /// </summary>
  /// <param name="x"></param>
  /// <param name="y"></param>
  Point(int x, int y) {
    _x = x;
    _y = y;
  }
  
  Point& operator=(const Point& point) {
    _x = point._x;
    _y = point._y;
    return *this;
  }

  ~Point() {
    // Do nothing.
  }
public:
  /// <summary>
  /// x position
  /// </summary>
  /// <returns></returns>
  int x() { return _x; }

  /// <summary>
  /// y position
  /// </summary>
  /// <returns></returns>
  int y() { return _y; }

  /// <summary>
  /// Update positions.
  /// </summary>
  /// <param name="x"></param>
  /// <param name="y"></param>
  void update(int x, int y) {
    _x = x;
    _y = y;
  }

  /// <summary>
  /// Calculate distance between two points
  /// </summary>
  /// <param name="start"></param>
  /// <param name="end"></param>
  /// <returns></returns>
  static double distance(const Point& start, const Point& end) {
    double dx = (start._x - end._x) * 1.0;
    double dy = (start._y - end._y) * 1.0;

    return sqrt(dx * dx + dy * dy);
  }

  /// <summary>
  /// Parse a Point from a string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  static std::shared_ptr<Point> parse(const std::string& buffer) {
    std::vector<std::string> tokens = Tokeniser::split(buffer, ": ");
    
    std::shared_ptr<Point> newPoint = std::make_shared<Point>(
      stoi(tokens[0]), 
      stoi(tokens[1])
    );

    return newPoint;
  }

  /// <summary>
  /// Convert a Point to string.
  /// </summary>
  /// <returns></returns>
  std::string toString() {
    std::stringstream builder;
    
    builder << _x;
    builder << ",";
    builder << _y;

    return builder.str();
  }
};

/// <summary>
/// Shape Abstract Class.
/// </summary>
class IShape {
public:
  IShape() {
    // Do nothing.
  }

  ~IShape() {
    // Do nothing.
  }

public:
  virtual std::string type() = 0;
  virtual std::shared_ptr<IShape> parse(const std::string&) = 0;
  virtual std::shared_ptr<IShape> createShape(const Point&, const Point&,
    int, int, COLORREF, COLORREF) = 0;
  virtual void draw(HDC& hdc) = 0;
  virtual std::string toString() = 0;
};

class LineShape : public IShape {
private:
  Point _start;
  Point _end;
  int _lineStyle;
  int _lineWidth;
  COLORREF _lineColour;

public:
  LineShape() {
    // Do nothing.
  }

  LineShape(const Point& start, const Point& end,
    int lineStyle, int lineWidth, COLORREF lineColour) {
    _start = start;
    _end = end;
    _lineStyle = lineStyle;
    _lineWidth = lineWidth;
    _lineColour = lineColour;
  }

  ~LineShape() {
    // Do nothing.
  }

public:
  /// <summary>
  /// Type of line: line.
  /// </summary>
  /// <returns></returns>
  std::string type() {
    return "line";
  }

  /// <summary>
  /// Create a new shape (using factory).
  /// </summary>
  /// <param name="start"></param>
  /// <param name="end"></param>
  /// <param name="lineStyle"></param>
  /// <param name="lineWidth"></param>
  /// <param name="lineColour"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(const Point& start, const Point& end,
  int lineStyle, int lineWidth, COLORREF lineColour, COLORREF backgroundColour) {
    std::shared_ptr<IShape> newLine = std::make_shared<LineShape>(
      start, 
      end, 
      lineStyle,
      lineWidth,
      lineColour
    );

    return newLine;
  }

  /// <summary>
  /// Parse a line from buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) {
    std::vector<std::string>tokens = Tokeniser::split(buffer, " ");

    std::shared_ptr<Point> topLeft = Point::parse(tokens[0]);
    std::shared_ptr<Point> rightBottom = Point::parse(tokens[1]);
    int lineStyle = stoi(tokens[2]);
    int lineWidth = stoi(tokens[3]);
    COLORREF lineColour = stoi(tokens[4]);

    std::shared_ptr<IShape> newLine = std::make_shared<LineShape>(
      *topLeft,
      *rightBottom,
      lineStyle,
      lineWidth,
      lineColour
    );

    return newLine;
  }

  /// <summary>
  /// Draw a line to screen.
  /// </summary>
  /// <param name="hdc"></param>
  void draw(HDC& hdc) {
    HPEN hPen = CreatePen(_lineStyle, _lineWidth, _lineColour);
    SelectObject(hdc, hPen);
    
    MoveToEx(hdc, _start.x(), _start.y(), NULL);
    LineTo(hdc, _end.x(), _end.y());

    // Delete pen object, otherwise this can cause GDI memory leak.
    DeleteObject(hPen);
  }

  /// <summary>
  /// Convert a Line to String.
  /// </summary>
  /// <returns></returns>
  std::string toString() {
    std::stringstream builder;

    builder << type();
    builder << ": ";
    builder << _start.toString();
    builder << " ";
    builder << _end.toString();
    builder << " ";
    builder << _lineStyle;
    builder << " ";
    builder << _lineWidth;
    builder << " ";
    builder << _lineColour;

    return builder.str();
  }
};

/// <summary>
/// Rectangle shape
/// </summary>
class RectangleShape : public IShape {
protected:
  Point _topLeft;
  Point _rightBottom;
  int _lineStyle;
  int _lineWidth;
  COLORREF _lineColour;
  COLORREF _backgroundColour;

public:
  RectangleShape() {
    // Do nothing.
  }

  RectangleShape(const Point& topLeft, const Point& rightBottom,
                 int lineStyle, int lineWidth,
                 COLORREF lineColour, COLORREF backgroundColour) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _lineStyle = lineStyle;
    _lineWidth = lineWidth;
    _lineColour = lineColour;
    _backgroundColour = backgroundColour;
  }

  ~RectangleShape() {
    // Do nothing.
  }
public:
  /// <summary>
  /// Rectangle type.
  /// </summary>
  /// <returns></returns>
  std::string type() {
    return "rectangle";
  }

  /// <summary>
  /// Create a new Rectangle shape (using factory).
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="lineStyle"></param>
  /// <param name="lineWidth"></param>
  /// <param name="lineColour"></param>
  /// <param name="backgoundColour"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth, COLORREF lineColour, COLORREF backgoundColour) {
    
    std::shared_ptr<IShape> newLine = std::make_shared<RectangleShape>(
      topLeft, 
      rightBottom, 
      lineStyle, 
      lineWidth, 
      lineColour,
      backgoundColour
    );

    return newLine;
  }

  /// <summary>
  /// Parse a rectangle from a string.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) {
    std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

    std::shared_ptr<Point> topLeft = Point::parse(tokens[0]);
    std::shared_ptr<Point> rightBottom = Point::parse(tokens[1]);
    int lineStyle = stoi(tokens[2]);
    int lineWidth = stoi(tokens[3]);
    COLORREF lineColour = stoi(tokens[4]);
    COLORREF backgroundColour = stoi(tokens[5]);

    std::shared_ptr<RectangleShape> newRectangle = std::make_shared<RectangleShape>(
      *topLeft,
      *rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newRectangle;
  }

  /// <summary>
  /// Draw a rectangle to the screen.
  /// </summary>
  /// <param name="hdc"></param>
  void draw(HDC& hdc) {
    // Create a pen (outside border)
    HPEN hPen = CreatePen(_lineStyle, _lineWidth, _lineColour);
    SelectObject(hdc, hPen);

    // Fill inside shape.
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, _backgroundColour);

    // And draw.
    MoveToEx(hdc, _topLeft.x(), _topLeft.y(), NULL);
    Rectangle(hdc, _topLeft.x(), _topLeft.y(), _rightBottom.x(), _rightBottom.y());

    DeleteObject(hPen);
  }

  /// <summary>
  /// Convert a Rectangle to String.
  /// </summary>
  /// <returns></returns>
  std::string toString() {
    std::stringstream builder;

    builder << type();
    builder << ": ";
    builder << _topLeft.toString();
    builder << " ";
    builder << _rightBottom.toString();
    builder << " ";
    builder << _lineStyle;
    builder << " ";
    builder << _lineWidth;
    builder << " ";
    builder << _lineColour;
    builder << " ";
    builder << _backgroundColour;

    return builder.str();
  }
};

/// <summary>
/// Square is a "special" rectangle.
/// </summary>
class SquareShape : public RectangleShape {
public:
  SquareShape() {
    // Do nothing.
  }

  SquareShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth,
    COLORREF lineColour, COLORREF backgroundColour) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _lineStyle = lineStyle;
    _lineWidth = lineWidth;
    _lineColour = lineColour;
    _backgroundColour = backgroundColour;
  }

  ~SquareShape() {
    // Do nothing.
  }
public:
  /// <summary>
  /// Square type.
  /// </summary>
  /// <returns></returns>
  std::string type() {
    return "square";
  }

  /// <summary>
  /// Create a square shape (factory)
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="lineStyle"></param>
  /// <param name="lineWidth"></param>
  /// <param name="lineColour"></param>
  /// <param name="backgroundColour"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth, COLORREF lineColour, COLORREF backgroundColour) {
    std::shared_ptr<IShape> newSquare = std::make_shared<SquareShape>(
      topLeft,
      rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newSquare;
  }

  /// <summary>
  /// Parse a Square from string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) {
    std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

    std::shared_ptr<Point> topLeft = Point::parse(tokens[0]);
    std::shared_ptr<Point> rightBottom = Point::parse(tokens[1]);
    int lineStyle = stoi(tokens[2]);
    int lineWidth = stoi(tokens[3]);
    COLORREF lineColour = stoi(tokens[4]);
    COLORREF backgroundColour = stoi(tokens[5]);

    std::shared_ptr<IShape> newSquare = std::make_shared<SquareShape>(
      *topLeft,
      *rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newSquare;
  }
};

/// <summary>
/// Ellipse shape.
/// </summary>
class EllipseShape : public IShape {
protected:
  Point _topLeft;
  Point _rightBottom;
  int _lineStyle;
  int _lineWidth;
  COLORREF _lineColour;
  COLORREF _backgroundColour;

public:
  EllipseShape() {
    // Do nothing.
  }

  EllipseShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth, 
    COLORREF lineColour, COLORREF backgroundColour) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _lineStyle = lineStyle;
    _lineWidth = lineWidth;
    _lineColour = lineColour;
    _backgroundColour = backgroundColour;
  }

  ~EllipseShape() {
    // Do nothing.
  }

public:
  /// <summary>
  /// Ellipse type.
  /// </summary>
  /// <returns></returns>
  std::string type() {
    return "ellipse";
  }
  
  /// <summary>
  /// Create a new Ellipse (factory)
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="lineStyle"></param>
  /// <param name="lineWidth"></param>
  /// <param name="lineColour"></param>
  /// <param name="backgroundColour"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth, COLORREF lineColour, COLORREF backgroundColour) {
    
    std::shared_ptr<IShape> newEllipse = std::make_shared<EllipseShape>(
      topLeft,
      rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newEllipse;
  }

  /// <summary>
  /// Parse an Ellipse from string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) {
    std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

    std::shared_ptr<Point> topLeft = Point::parse(tokens[0]);
    std::shared_ptr<Point> rightBottom = Point::parse(tokens[1]);
    int lineStyle = stoi(tokens[2]);
    int lineWidth = stoi(tokens[3]);
    COLORREF lineColour = stoi(tokens[4]);
    COLORREF backgroundColour = stoi(tokens[5]);
    
    std::shared_ptr<IShape> newEllipse = std::make_shared<EllipseShape>(
      *topLeft,
      *rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newEllipse;
  }

  /// <summary>
  /// Draw an ellipse to the screen.
  /// </summary>
  void draw(HDC& hdc) {
    HPEN hPen = CreatePen(_lineStyle, _lineWidth, _lineColour);
    SelectObject(hdc, hPen);
    
     SelectObject(hdc, GetStockObject(DC_BRUSH));
     SetDCBrushColor(hdc, _backgroundColour);

    MoveToEx(hdc, _topLeft.x(), _topLeft.y(), NULL);
    Ellipse(hdc, _topLeft.x(), _topLeft.y(), _rightBottom.x(), _rightBottom.y());

    DeleteObject(hPen);
  }

  /// <summary>
  /// Convert an Ellipse to String.
  /// </summary>
  /// <returns></returns>
  std::string toString() {
    std::stringstream builder;

    builder << type();
    builder << ": ";
    builder << _topLeft.toString();
    builder << " ";
    builder << _rightBottom.toString();
    builder << " ";
    builder << _lineStyle;
    builder << " ";
    builder << _lineWidth;
    builder << " ";
    builder << _lineColour;
    builder << " ";
    builder << _backgroundColour;

    return builder.str();
  }
};

/// <summary>
/// Circle is a "special" ellipse.
/// </summary>
class CircleShape : public EllipseShape {
public:
  CircleShape() {
    // Do nothing.
  }

  CircleShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth, 
    COLORREF lineColour, COLORREF backgroundColour) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _lineStyle = lineStyle;
    _lineWidth = lineWidth;
    _lineColour = lineColour;
    _backgroundColour = backgroundColour;
  }

  ~CircleShape() {
    // Do nothing.
  }
public:
  /// <summary>
  /// Circle type.
  /// </summary>
  /// <returns></returns>
  std::string type() {
    return "circle";
  }

  /// <summary>
  /// Create a new circle (factory)
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="lineStyle"></param>
  /// <param name="lineWidth"></param>
  /// <param name="lineColour"></param>
  /// <param name="backgroundColour"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(const Point& topLeft, const Point& rightBottom,
    int lineStyle, int lineWidth, COLORREF lineColour, COLORREF backgroundColour) {
    std::shared_ptr<IShape> newCircle = std::make_shared<CircleShape>(
      topLeft,
      rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newCircle;
  }

  /// <summary>
  /// Parse a Circle from string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) {
    std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

    std::shared_ptr<Point> topLeft = Point::parse(tokens[0]);
    std::shared_ptr<Point> rightBottom = Point::parse(tokens[1]);
    int lineStyle = stoi(tokens[2]);
    int lineWidth = stoi(tokens[3]);
    COLORREF lineColour = stoi(tokens[4]);
    COLORREF backgroundColour = stoi(tokens[5]);
    
    std::shared_ptr<IShape> newCircle = std::make_shared<CircleShape>(
      *topLeft,
      *rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );

    return newCircle;
  }
};

class ShapeFactory {
private:
  inline static ShapeFactory* _instance = NULL;
  std::vector<std::shared_ptr<IShape>> _prototype;

  ShapeFactory() {
    _prototype.push_back(std::make_shared<LineShape>());
    _prototype.push_back(std::make_shared<RectangleShape>());
    _prototype.push_back(std::make_shared<SquareShape>());
    _prototype.push_back(std::make_shared<EllipseShape>());
    _prototype.push_back(std::make_shared<CircleShape>());
  }

  ~ShapeFactory() {
    // Do nothing.
  }

public:
  /// <summary>
  /// Release Singleton instance.
  /// </summary>
  void deleteInstance() {
    delete _instance;
  }

  /// <summary>
  /// Create a new instance.
  /// </summary>
  /// <returns></returns>
  static ShapeFactory* getInstance() {
    if (_instance == NULL) {
      _instance = new ShapeFactory();
    }

    return _instance;
  }

  /// <summary>
  /// Get prototype size.
  /// </summary>
  /// <returns></returns>
  int prototypeSize() {
    return (int)_prototype.size();
  }

  /// <summary>
  /// Create a shape pointer from given information.
  /// </summary>
  /// <param name="shapeType"></param>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="lineStyle"></param>
  /// <param name="lineWidth"></param>
  /// <param name="lineColour"></param>
  /// <param name="backgroundColour"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> create(int shapeType, const Point& topLeft, const Point& rightBottom,
  int lineStyle, int lineWidth, COLORREF lineColour, COLORREF backgroundColour) {
    
    return _prototype[shapeType]->createShape(
      topLeft,
      rightBottom,
      lineStyle,
      lineWidth,
      lineColour,
      backgroundColour
    );
  }

  /// <summary>
  /// Create a shape from a parsed string.
  /// </summary>
  /// <param name="type"></param>
  /// <param name="value"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& type, const std::string& value) {
    std::shared_ptr<IShape> result = NULL;

    for (int i = 0; i < _prototype.size(); ++i) {
      if (_prototype[i]->type() == type) {
        result = _prototype[i]->parse(value);
        break;
      }
    }

    return result;
  }
};