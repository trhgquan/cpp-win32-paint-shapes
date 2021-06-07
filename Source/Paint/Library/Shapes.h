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

  /// <summary>
  /// Copy constructor
  /// </summary>
  /// <param name="point"></param>
  Point(const Point& point) {
    _x = point._x;
    _y = point._y;
  }
  
  /// <summary>
  /// Assign operator.
  /// </summary>
  /// <param name="point"></param>
  /// <returns></returns>
  Point& operator=(const Point& point) {
    _x = point._x;
    _y = point._y;
    return *this;
  }

  ~Point() {
    // Do nothing.
  }
public:
  bool operator<=(const Point& point) {
    return _x <= point._x && _y <= point._y;
  }

  bool operator>=(const Point& point) {
    return _x >= point._x && _y >= point._y;
  }

  /// <summary>
  /// Left shift overload.
  /// </summary>
  /// <param name="out"></param>
  /// <param name="point"></param>
  /// <returns></returns>
  friend std::ostream& operator<<(std::ostream& out, Point point) {
    out << point.toString();
    return out;
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
    double dx = (double)start._x - (double)end._x;
    double dy = (double)start._y - (double)end._y;

    return sqrt(dx * dx + dy * dy);
  }

  /// <summary>
  /// Parse a Point from a string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  static std::shared_ptr<Point> parse(const std::string& buffer) {
    try {
      std::vector<std::string> tokens = Tokeniser::split(buffer, ",");

      std::shared_ptr<Point> newPoint = std::make_shared<Point>(
        stoi(tokens.at(0)),
        stoi(tokens.at(1))
      );

      return newPoint;
    }
    
    catch (const std::exception& e) {
      throw e;
    }
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
/// IShape Interface.
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
  const ShapeGraphic&) = 0;
  virtual std::shared_ptr<IShape> cloneShape() = 0;
  virtual void draw(HDC& hdc) = 0;
  virtual void move(int, int) = 0;
  virtual bool in(const Point&, const Point&) = 0;
  virtual std::string toString() = 0;
};

class LineShape : public IShape {
private:
  Point _start;
  Point _end;
  ShapeGraphic _graphic;

public:
  LineShape() {
    // Do nothing.
  }

  LineShape(const Point& start, const Point& end,
    const ShapeGraphic& graphic) {
    _start = start;
    _end = end;
    _graphic = graphic;
  }

  LineShape(const LineShape& line) {
    _start = line._start;
    _end = line._end;
    _graphic = line._graphic;
  }

  LineShape& operator=(const LineShape& line) {
    _start = line._start;
    _end = line._end;
    _graphic = line._graphic;
    return *this;
  }

  ~LineShape() {
    // Do nothing.
  }

public:
  /// <summary>
  /// Type of line: line.
  /// </summary>
  /// <returns></returns>
  std::string type() override {
    return "line";
  }

  /// <summary>
  /// Create a new shape (using factory).
  /// </summary>
  /// <param name="start"></param>
  /// <param name="end"></param>
  /// <param name="graphic"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(const Point& start, const Point& end,
  const ShapeGraphic& graphic) override {
    std::shared_ptr<IShape> newLine = std::make_shared<LineShape>(
      start,
      end, 
      graphic
    );

    return newLine;
  }

  /// <summary>
  /// Clone this line.
  /// </summary>
  /// <returns></returns>
  std::shared_ptr<IShape> cloneShape() override {
    return std::shared_ptr<LineShape>(new LineShape(*this));
  }

  /// <summary>
  /// Parse a line from buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) override {
    try {
      std::vector<std::string>tokens = Tokeniser::split(buffer, " ");

      std::shared_ptr<Point> begin = Point::parse(tokens.at(0));
      std::shared_ptr<Point> end = Point::parse(tokens.at(1));
      ShapeGraphic graphic = ShapeGraphic::parse(tokens.at(2));

      std::shared_ptr<IShape> newLine = std::make_shared<LineShape>(
        *begin,
        *end,
        graphic
      );

      return newLine;
    }

    catch (const std::exception& e) {
      throw e;
    }
  }

  /// <summary>
  /// Draw a line to screen.
  /// </summary>
  /// <param name="hdc"></param>
  void draw(HDC& hdc) override {
    HPEN hPen = CreatePen(
      _graphic.lineStyle(),
      _graphic.lineWidth(),
      _graphic.lineColour()
    );
    SelectObject(hdc, hPen);
    
    MoveToEx(hdc, _start.x(), _start.y(), NULL);
    LineTo(hdc, _end.x(), _end.y());

    // Delete pen object, otherwise this can cause GDI memory leak.
    DeleteObject(hPen);
  }

  /// <summary>
  /// Move the shape by vector(dx, dy).
  /// </summary>
  /// <param name="dx"></param>
  /// <param name="dy"></param>
  void move(int dx, int dy) override {
    _start.update(
      _start.x() + dx,
      _start.y() + dy
    );

    _end.update(
      _end.x() + dx,
      _end.y() + dy
    );
  }

  /// <summary>
  /// Check if the line is inside a rectangle
  /// limited by 2 points.
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <returns></returns>
  bool in(const Point& topLeft, const Point& rightBottom) override {
    return (_start >= topLeft &&
      _end >= topLeft &&
      _start <= rightBottom &&
      _end <= rightBottom);
  }

  /// <summary>
  /// Convert a Line to String.
  /// </summary>
  /// <returns></returns>
  std::string toString() override {
    std::stringstream builder;

    builder << type();
    builder << ": ";
    builder << _start;
    builder << " ";
    builder << _end;
    builder << " ";
    builder << _graphic;

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
  ShapeGraphic _graphic;

public:
  RectangleShape() {
    // Do nothing.
  }

  RectangleShape(const Point& topLeft, const Point& rightBottom,
  const ShapeGraphic& graphic) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _graphic = graphic;
  }

  RectangleShape(const RectangleShape& rectangle) {
    _topLeft = rectangle._topLeft;
    _rightBottom = rectangle._rightBottom;
    _graphic = rectangle._graphic;
  }

  RectangleShape& operator=(const RectangleShape& rectangle) {
    _topLeft = rectangle._topLeft;
    _rightBottom = rectangle._rightBottom;
    _graphic = rectangle._graphic;

    return *this;
  }

  ~RectangleShape() {
    // Do nothing.
  }

public:
  Point topLeft() { return _topLeft; }
  Point rightBottom() { return _rightBottom; }
  void setTopLeft(const Point& topLeft) {
    _topLeft = topLeft;
  }
  void setRightBottom(const Point& rightBottom) {
    _rightBottom = rightBottom;
  }
public:
  /// <summary>
  /// Rectangle type.
  /// </summary>
  /// <returns></returns>
  std::string type() override {
    return "rectangle";
  }
  
  /// <summary>
  /// Create a new Rectangle shape (using factory).
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="graphic"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(
    const Point& topLeft, const Point& rightBottom,
    const ShapeGraphic& graphic) override {
    
    std::shared_ptr<IShape> newLine = std::make_shared<RectangleShape>(
      topLeft, 
      rightBottom, 
      graphic
    );

    return newLine;
  }

  /// <summary>
  /// Clone a rectangle!
  /// </summary>
  /// <returns></returns>
  std::shared_ptr<IShape> cloneShape() override {
    return std::shared_ptr<RectangleShape>(new RectangleShape(*this));
  }

  /// <summary>
  /// Parse a rectangle from a string.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) override {
    try {
      std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

      std::shared_ptr<Point> topLeft = Point::parse(tokens.at(0));
      std::shared_ptr<Point> rightBottom = Point::parse(tokens.at(1));
      ShapeGraphic graphic = ShapeGraphic::parse(tokens.at(2));

      std::shared_ptr<RectangleShape> newRectangle = std::make_shared<RectangleShape>(
        *topLeft,
        *rightBottom,
        graphic
      );

      return newRectangle;
    }

    catch (const std::exception& e) {
      throw e;
    }
  }

  /// <summary>
  /// Draw a rectangle to the screen.
  /// </summary>
  /// <param name="hdc"></param>
  void draw(HDC& hdc) override {
    // Create a pen (outside border)
    HPEN hPen = CreatePen(
      _graphic.lineStyle(),
      _graphic.lineWidth(),
      _graphic.lineColour()
    );
    SelectObject(hdc, hPen);

    // Fill inside shape.
    SelectObject(hdc, GetStockObject(_graphic.backgroundBrush()));
    SetDCBrushColor(hdc, _graphic.backgroundColour());

    // And draw.
    MoveToEx(hdc, _topLeft.x(), _topLeft.y(), NULL);
    Rectangle(hdc, _topLeft.x(), _topLeft.y(), _rightBottom.x(), _rightBottom.y());

    DeleteObject(hPen);
  }

  /// <summary>
  /// Move the rectangle by vector(dx, dy)
  /// </summary>
  /// <param name="dx"></param>
  /// <param name="dy"></param>
  void move(int dx, int dy) override {
    _topLeft.update(
      _topLeft.x() + dx,
      _topLeft.y() + dy
    );

    _rightBottom.update(
      _rightBottom.x() + dx,
      _rightBottom.y() + dy
    );
  }

  /// <summary>
  /// Check if this rectangle is inside a rectangle
  /// limited by 2 points.
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <returns></returns>
  bool in(const Point& topLeft, const Point& rightBottom) override {
    return (_topLeft >= topLeft &&
      _rightBottom >= topLeft &&
      _topLeft <= rightBottom &&
      _rightBottom <= rightBottom);
  }

  /// <summary>
  /// Convert a Rectangle to String.
  /// </summary>
  /// <returns></returns>
  std::string toString() override {
    std::stringstream builder;

    builder << type();
    builder << ": ";
    builder << _topLeft;
    builder << " ";
    builder << _rightBottom;
    builder << " ";
    builder << _graphic;

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
    const ShapeGraphic& graphic) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _graphic = graphic;
  }

  SquareShape(const SquareShape& square) {
    _topLeft = square._topLeft;
    _rightBottom = square._rightBottom;
    _graphic = square._graphic;
  }

  SquareShape& operator=(const SquareShape& square) {
    _topLeft = square._topLeft;
    _rightBottom = square._rightBottom;
    _graphic = square._graphic;

    return *this;
  }

  ~SquareShape() {
    // Do nothing.
  }
public:
  /// <summary>
  /// Square type.
  /// </summary>
  /// <returns></returns>
  std::string type() override {
    return "square";
  }

  /// <summary>
  /// Create a square shape (factory)
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="graphic"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(
    const Point& topLeft, const Point& rightBottom,
    const ShapeGraphic& graphic) override {
    std::shared_ptr<IShape> newSquare = std::make_shared<SquareShape>(
      topLeft,
      rightBottom,
      graphic
    );

    return newSquare;
  }

  /// <summary>
  /// Clone this square!
  /// </summary>
  /// <returns></returns>
  std::shared_ptr<IShape> cloneShape() override {
    return std::shared_ptr<SquareShape>(new SquareShape(*this));
  }

  /// <summary>
  /// Parse a Square from string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) override {
    try {
      std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

      std::shared_ptr<Point> topLeft = Point::parse(tokens.at(0));
      std::shared_ptr<Point> rightBottom = Point::parse(tokens.at(1));
      ShapeGraphic graphic = ShapeGraphic::parse(tokens.at(2));

      std::shared_ptr<IShape> newSquare = std::make_shared<SquareShape>(
        *topLeft,
        *rightBottom,
        graphic
      );

      return newSquare;
    }
    
    catch (const std::exception& e) {
      throw e;
    }
  }
};

/// <summary>
/// Ellipse shape.
/// </summary>
class EllipseShape : public IShape {
protected:
  Point _topLeft;
  Point _rightBottom;
  ShapeGraphic _graphic;

public:
  EllipseShape() {
    // Do nothing.
  }

  EllipseShape(const Point& topLeft, const Point& rightBottom,
  const ShapeGraphic& graphic) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _graphic = graphic;
  }

  EllipseShape(const EllipseShape& ellipse) {
    _topLeft = ellipse._topLeft;
    _rightBottom = ellipse._rightBottom;
    _graphic = ellipse._graphic;
  }

  EllipseShape& operator=(const EllipseShape& ellipse) {
    _topLeft = ellipse._topLeft;
    _rightBottom = ellipse._rightBottom;
    _graphic = ellipse._graphic;

    return *this;
  }

  ~EllipseShape() {
    // Do nothing.
  }

public:
  /// <summary>
  /// Ellipse type.
  /// </summary>
  /// <returns></returns>
  std::string type() override {
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
  std::shared_ptr<IShape> createShape(
    const Point& topLeft, const Point& rightBottom,
    const ShapeGraphic& graphic) override {
    std::shared_ptr<IShape> newEllipse = std::make_shared<EllipseShape>(
      topLeft,
      rightBottom,
      graphic
    );

    return newEllipse;
  }

  /// <summary>
  /// Clone this ellipse!
  /// </summary>
  /// <returns></returns>
  std::shared_ptr<IShape> cloneShape() override {
    return std::shared_ptr<EllipseShape>(new EllipseShape(*this));
  }

  /// <summary>
  /// Parse an Ellipse from string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) override {
    std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

    std::shared_ptr<Point> topLeft = Point::parse(tokens.at(0));
    std::shared_ptr<Point> rightBottom = Point::parse(tokens.at(1));
    ShapeGraphic graphic = ShapeGraphic::parse(tokens.at(2));
    
    std::shared_ptr<IShape> newEllipse = std::make_shared<EllipseShape>(
      *topLeft,
      *rightBottom,
      graphic
    );

    return newEllipse;
  }
    
  /// <summary>
  /// Draw an ellipse to the screen.
  /// </summary>
  /// <param name="hdc"></param>
  void draw(HDC& hdc) override {
    HPEN hPen = CreatePen(
      _graphic.lineStyle(),
      _graphic.lineWidth(),
      _graphic.lineColour()
    );
    SelectObject(hdc, hPen);
    
    SelectObject(hdc, GetStockObject(_graphic.backgroundBrush()));
    SetDCBrushColor(hdc, _graphic.backgroundColour());

    MoveToEx(hdc, _topLeft.x(), _topLeft.y(), NULL);
    Ellipse(hdc, _topLeft.x(), _topLeft.y(), _rightBottom.x(), _rightBottom.y());

    DeleteObject(hPen);
  }

  /// <summary>
  /// Move an ellipse for a vector(dx, dy)
  /// </summary>
  /// <param name="dx"></param>
  /// <param name="dy"></param>
  void move(int dx, int dy) override {
    _topLeft.update(
      _topLeft.x() + dx,
      _topLeft.y() + dy
    );

    _rightBottom.update(
      _rightBottom.x() + dx,
      _rightBottom.y() + dy
    );
  }

  /// <summary>
  /// Check if this ellipse is inside a rectangle
  /// limited by 2 points.
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <returns></returns>
  bool in(const Point& topLeft, const Point& rightBottom) override {
    return (_topLeft >= topLeft &&
      _rightBottom >= topLeft &&
      _topLeft <= rightBottom &&
      _rightBottom <= rightBottom);
  }

  /// <summary>
  /// Convert an Ellipse to String.
  /// </summary>
  /// <returns></returns>
  std::string toString() override {
    std::stringstream builder;

    builder << type();
    builder << ": ";
    builder << _topLeft;
    builder << " ";
    builder << _rightBottom;
    builder << " ";
    builder << _graphic;

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
  const ShapeGraphic& graphic) {
    _topLeft = topLeft;
    _rightBottom = rightBottom;
    _graphic = graphic;
  }

  CircleShape(const CircleShape& circle) {
    _topLeft = circle._topLeft;
    _rightBottom = circle._rightBottom;
    _graphic = circle._graphic;
  }

  CircleShape& operator=(const CircleShape& circle) {
    _topLeft = circle._topLeft;
    _rightBottom = circle._rightBottom;
    _graphic = circle._graphic;

    return *this;
  }

  ~CircleShape() {
    // Do nothing.
  }
public:
  /// <summary>
  /// Circle type.
  /// </summary>
  /// <returns></returns>
  std::string type() override {
    return "circle";
  }
  
  /// <summary>
  /// Create a new circle (factory)
  /// </summary>
  /// <param name="topLeft"></param>
  /// <param name="rightBottom"></param>
  /// <param name="graphic"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> createShape(
    const Point& topLeft, const Point& rightBottom,
    const ShapeGraphic& graphic) override {
    std::shared_ptr<IShape> newCircle = std::make_shared<CircleShape>(
      topLeft,
      rightBottom,
      graphic
    );

    return newCircle;
  }

  /// <summary>
  /// Clone this circle.
  /// </summary>
  /// <returns></returns>
  std::shared_ptr<IShape> cloneShape() override {
    return std::shared_ptr<CircleShape>(new CircleShape(*this));
  }

  /// <summary>
  /// Parse a Circle from string buffer.
  /// </summary>
  /// <param name="buffer"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> parse(const std::string& buffer) override {
    try {
      std::vector<std::string> tokens = Tokeniser::split(buffer, " ");

      std::shared_ptr<Point> topLeft = Point::parse(tokens.at(0));
      std::shared_ptr<Point> rightBottom = Point::parse(tokens.at(1));
      ShapeGraphic graphic = ShapeGraphic::parse(tokens.at(2));

      std::shared_ptr<IShape> newCircle = std::make_shared<CircleShape>(
        *topLeft,
        *rightBottom,
        graphic
      );

      return newCircle;
    }
    
    catch (const std::exception& e) {
      throw e;
    }
  }
};

/// <summary>
/// ShapeFactory generates Shapes!
/// </summary>
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
  /// <param name="shapeGraphic"></param>
  /// <returns></returns>
  std::shared_ptr<IShape> create(int shapeType,
    const Point& topLeft, const Point& rightBottom,
    const ShapeGraphic& shapeGraphic) {
    
    return _prototype[shapeType]->createShape(
      topLeft,
      rightBottom,
      shapeGraphic
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