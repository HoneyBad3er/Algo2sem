//
//  geometry.h
//  Geometry
//
//  Created by Marat Amirov on 17/04/2020.
//  Copyright © 2020 Marat Amirov. All rights reserved.
//

#ifndef geometry_h
#define geometry_h

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <initializer_list>
#include <utility>
#include <typeinfo>
#include <limits>

//__________________DOUBLE_EQUAL__________________
bool isEqual(double lhs, double rhs){
    return std::fabs(lhs - rhs) < 1e-3;
}


//__________________POINT__________________
class Point {
public:
    Point(double x, double y);
    Point(const Point& begin, const Point& end);
    
    //Transformations
    double get_length() const;
    Point get_normal() const;
    void rotate(const Point& center, double angle);
    void scale(Point center, double coefficient);
    void reflex(Point center);
    
    //Relations operators
    Point& operator = (const Point& rhs);
    friend bool operator == (const Point& lhs, const Point& rhs);
    friend bool operator != (Point& lhs, Point& rhs);
    
    //Arithmetic operators
    Point& operator -= (const Point& rhs);
    Point& operator += (const Point& rhs);
    Point& operator *= (double num);
    friend Point operator - (const Point& lhs, const Point& rhs);
    friend Point operator + (const Point& lhs, const Point& rhs);
    friend Point  operator * (const Point& v1, double num);
    friend Point operator * (double num, const Point& v1);
    friend double  operator % (const Point& v1, const Point& v2);
    friend double  operator * (const Point& v1, const Point& v2);
    double x;
    double y;

};

Point::Point(double x, double y): x(x), y(y) {}

Point::Point(const Point& begin, const Point& end): Point(end.x - begin.x,
                                                                    end.y - begin.y) {}

double Point::get_length() const {
    return std::sqrt(x * x + y * y);
}

Point Point::get_normal() const {
    return Point{-y, x};
}

void Point::rotate(const Point& center, double angle) {
    double x0 = center.x;
    double y0 = center.y;
    angle *= (M_PI/180);
    double _x = x;
    double _y = y;
    x = x0 + (_x - x0) * cos(angle) - (_y - y0) * sin(angle);
    y = y0 + (_x - x0) * sin(angle) + (_y - y0) * cos(angle);
}

void Point::scale(Point center, double coefficient) {
    *this -= center;
    *this *= coefficient;
    *this += center;
}

void Point::reflex(Point center) {
    *this += 2 * (center - *this);
}

bool operator == (const Point& lhs, const Point& rhs) {
    return isEqual(lhs.x, rhs.x) && isEqual(rhs.y, lhs.y);
}

Point& Point::operator = (const Point& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
}

bool operator != (const Point& lhs, const Point& rhs) {
    return !(lhs == rhs);
}

Point& Point::operator -= (const Point& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Point& Point::operator += (const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point& Point::operator *= (double num) {
    x *= num;
    y *= num;
    return *this;
}

Point operator - (const Point& lhs, const Point& rhs) {
    Point tmp(lhs.x, lhs.y);
    tmp -= rhs;
    return tmp;
}

Point operator + (const Point& lhs, const Point& rhs) {
    return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

Point operator * (const Point& v1, double num) {
    Point tmp = v1;
    tmp *= num;
    return tmp;
}

Point operator * (double num, const Point& v1) {
    Point tmp = v1;
    tmp *= num;
    return tmp;
}

//Скалярное произведение
double  operator * (const Point& v1, const Point& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

//Векторное произведение
double  operator % (const Point& v1, const Point& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

//__________________LINE__________________
class Line {
public:
    explicit Line(Point first, Point second);
    explicit Line(double angle, double shift);
    explicit Line(Point Point, double angle);
    
    Point get_normalV() const;
    double get_A() const;
    double get_B() const;
    double get_C() const;
    
    friend bool operator == (const Line& lhs, const Line& rhs);
    friend bool operator != (const Line& lhs, const Line& rhs);
private:
    Point _point;
    Point _directV;
};

Line::Line(Point first, Point second): _point(first),
                                       _directV(first, second) {}

Line::Line(double angle, double shift): Line({0., shift}, {{0, shift}, {- shift/angle, 0}}) {}

Line::Line(Point point, double angle): Line(angle, point.y - point.x * angle) {}

Point Line::get_normalV() const {
    return _directV.get_normal();
}

double Line::get_A() const {
    return _directV.y;
}

double Line::get_B() const {
    return -_directV.x;
}

double Line::get_C() const {
    return -(get_A() * _point.x
             + get_B() * _point.y);
}

bool operator == (const Line& lhs, const Line& rhs) {
    return isEqual(lhs.get_A() / rhs.get_A(), lhs.get_B() / rhs.get_B())
           && isEqual(lhs.get_B() / rhs.get_B(), lhs.get_C() / rhs.get_C());
}

bool operator != (const Line& lhs, const Line& rhs) {
    return !(lhs == rhs);
}

Point lineIntersectionPoint(const Line& L1, const Line& L2) {
    double det = L1.get_A() * L2.get_B() - L2.get_A() * L1.get_B();
    double x = (L1.get_B() * L2.get_C() - L2.get_B() * L1.get_C()) / det;
    double y = (L1.get_C() * L2.get_A() - L2.get_C() * L1.get_A()) / det;
    return {x,y};
}

bool areParallel(const Line& L1, const Line& L2) {
  return isEqual(L1.get_A() * L2.get_B(), L1.get_B() * L2.get_A());
}

Point lineIntersection(const Line& L1, const Line& L2) {
  if (areParallel(L1, L2)) {
      return {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
  }
  return lineIntersectionPoint(L1, L2);
}



//__________________SHAPE__________________
class Shape {
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(const Point& point) const = 0;
    virtual bool operator == (const Shape& another) const = 0;
    virtual bool operator != (const Shape& another) const = 0;
    virtual void rotate(const Point& center, double angle) = 0;
    virtual void reflex(const Point& center) = 0;
    virtual void reflex(const Line& axis) = 0;
    virtual void scale(const Point& center, double coefficient) = 0;
    virtual ~Shape() {}
protected:
    double _perimeter;
    double _area;
};

//__________________ELLIPSE__________________
class Ellipse : public Shape {
public:
    explicit Ellipse(const Point& f1, const Point& f2, double a);
    std::pair<Point,Point> focuses() const;
    std::pair<Line, Line> directrices() const;
    double eccentricity() const;
    double bigAxis() const;
    Point center() const;
    void reflexPoint(const Line& axis);
    double perimeter() const override;
    double area() const override;
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;
    bool containsPoint(const Point& point) const override;
    bool operator == (const Shape& another) const override;
    bool operator != (const Shape& another) const override;
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& axis) override;
    void scale(const Point& center, double coefficient) override;
    Point reflexPoint(const Point& point, const Line& axis);
protected:
    Point _f1;
    Point _f2;
    Point _center;
    double _a;
    double _c;
    double _b;
    double _eccentricity;
};

Ellipse::Ellipse(const Point& f1, const Point& f2, double a) : _f1(f1), _f2(f2),
                                                               _center(0.5 * (f1 + f2)),
                                                               _a(a/2), _c((f2 - f1).get_length()/2),
                                                               _b(sqrt(_a * _a - _c * _c)),
                                                               _eccentricity(_c/_a) {}

std::pair<Point,Point> Ellipse::focuses() const {
    return {_f1, _f2};
}

std::pair<Line, Line> Ellipse::directrices() const {
    Point v(_f1 - _f2);
    v *= (1/v.get_length());
    v *= (_a/_eccentricity);
    Line d1(_center + v, v.get_normal());
    Line d2(_center - v, v.get_normal());
    return {d1, d2};
}

double Ellipse::eccentricity() const {
    return _eccentricity;
}

double Ellipse::bigAxis() const {
    return _a * 2;
}

Point Ellipse::center() const {
    return _center;
}

double Ellipse::perimeter() const {
    return std::fabs(M_PI * (3 * (_a + _b) - std::sqrt((3 * _a + _b) * (_a + 3 * _b))));
}

double Ellipse::area() const {
    return std::fabs(M_PI * _a * _b);
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    try {
        const auto ellipse = dynamic_cast<const Ellipse&>(another);
        return isEqual((_f1 - _f2).get_length(),
                       (ellipse.focuses().first - ellipse.focuses().second).get_length())
               && isEqual(_a, ellipse.bigAxis());
    }
    catch (std::bad_cast& message) {
        return false;
    }
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    try {
        const auto ellipse = dynamic_cast<const Ellipse&>(another);
        return isEqual((_f1 - _f2).get_length() / (ellipse.focuses().first - ellipse.focuses().second).get_length(),
                       bigAxis() / ellipse.bigAxis());
    }
    catch (std::bad_cast& message) {
        return false;
    }
}

bool Ellipse::containsPoint(const Point& point) const {
    return ((point - _f1).get_length()
            + (point - _f2).get_length()) <= bigAxis();
}

bool Ellipse::operator == (const Shape& another) const {
    try {
        const auto ellipse = dynamic_cast<const Ellipse&>(another);
        Point f1 = ellipse.focuses().first;
        Point f2 = ellipse.focuses().second;
        return isEqual(bigAxis(), ellipse.bigAxis())
               && ((_f1 == f1 && _f2 == f2) || (_f2 == f1 && _f1 == f2));
    }
    catch (std::bad_cast& message) {
        return false;
    }
}

bool Ellipse::operator != (const Shape& another) const {
    return !(*this == another);
}

void Ellipse::rotate(const Point& center, double angle) {
    _f1.rotate(center, angle);
    _f2.rotate(center, angle);
}

void Ellipse::reflex(const Point& center) {
    _f1.reflex(center);
    _f2.reflex(center);
}

Point Ellipse::reflexPoint(const Point& point, const Line& axis) {
    Line L1(point, point + axis.get_normalV());
    Point intersectionPoint = lineIntersection(axis, L1);
    return intersectionPoint + (intersectionPoint - point);
}

void Ellipse::reflex(const Line& axis) {
    _f1 = reflexPoint(_f1, axis);
    _f2 = reflexPoint(_f2, axis);
}

void Ellipse::scale(const Point& center, double coefficient) {
    _f1.scale(center, coefficient);
    _f2.scale(center, coefficient);
    _a *= coefficient;
    _b *= coefficient;
    _c *= coefficient;
    _eccentricity = _c / _a;
}


//__________________CIRCLE__________________
class Circle : public Ellipse {
public:
    Circle(const Point& center, double radius);
    double radius() const;
};

Circle::Circle(const Point& center, double radius): Ellipse(center, center, 2*radius) {}

double Circle::radius() const {
    return Ellipse::_a;
}


//__________________POLYGON__________________
class Polygon: public Shape {
public:
    explicit Polygon(const std::vector<Point>& vertices);
    template<typename... T>
    Polygon(const T&... points);
    size_t verticesCount() const;
    std::vector<Point> getVertices() const;
    bool isConvex() const;
    double perimeter() const override;
    double area() const override;
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;
    bool containsPoint(const Point& point) const override;
    bool operator == (const Shape& another) const override;
    bool operator != (const Shape& another) const override;
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& axis) override;
    void scale(const Point& center, double coefficient) override;
protected:
    size_t _vCount;
    std::vector<Point> _vertices;
};

Polygon::Polygon(const std::vector<Point>& vertices): _vCount(vertices.size()),
                                               _vertices(vertices) {}

template<typename... T>
Polygon::Polygon(const T&... points) {
    for (auto&& point : std::initializer_list<Point>{points...}) {
        _vertices.push_back(point);
    }
    _vCount = _vertices.size();

}

size_t Polygon::verticesCount() const {
    return _vCount;
}

std::vector<Point> Polygon::getVertices() const {
    return _vertices;
}

bool Polygon::isConvex() const {
    size_t i,j,k;
    size_t flag = 0;
    double vector_mul_res;

    if (_vCount < 3)
       return false;

    for (i = 0; i < _vCount ; ++i) {
        j = (i + 1) % _vCount;
        k = (i + 2) % _vCount;
        vector_mul_res = (_vertices[j] - _vertices[i]) % (_vertices[k] - _vertices[j]);
        if (vector_mul_res < 0) {
            flag |= 1;
        }
        else if (vector_mul_res > 0) {
            flag |= 2;
        }
        if (flag == 3) {
            return false;
        }
    }
    
    if (flag != 0) {
        return true;
    }
    else {
        return(0);
    }
}

double Polygon::perimeter() const {
    double perimeter = 0;
    for (size_t i = 0; i < _vCount; ++i) {
        size_t j = i + 1;
        perimeter += (_vertices[j] - _vertices[i]).get_length();
    }
    return perimeter;
}

double Polygon::area() const {
    double area = 0;
    for (size_t i = 0; i < _vCount; ++i) {
        Point edge1(_vertices[i] - _vertices[0]);
        Point edge2(_vertices[(i + 1) % _vCount] - _vertices[0]);
        area += edge1 % edge2;
    }
    return std::fabs(area / 2);
}

bool Polygon::isCongruentTo(const Shape& another) const {
    try{
        const auto polygon = dynamic_cast<const Polygon&>(another);
        if (_vCount != polygon.verticesCount()) {
            return false;
        }
        return isSimilarTo(polygon) && isEqual(area(), polygon.area());
    }
    catch (std::bad_cast& message){
        return false;
    }
    return true;
}

bool Polygon::isSimilarTo(const Shape& another) const {
    try{
        const auto polygon = dynamic_cast<const Polygon&>(another);
        if (_vCount != polygon.verticesCount()) {
            return false;
        }
        double coeff = std::sqrt(area() / polygon.area());
        std::vector<double> sides;
        std::vector<double> _sides;
        sides.reserve(_vCount);
        _sides.reserve(_vCount);
        for (size_t i = 0; i < _vCount ; ++i) {
            size_t j = i + 1;
            j %= _vCount;
            sides.push_back((_vertices[j] - _vertices[i]).get_length());
            _sides.push_back((polygon.getVertices()[j]
                             - polygon.getVertices()[i]).get_length());
        }
        
        for (size_t i = 0; i < _vCount ; ++i) {
            size_t counter = 0;
            size_t r_counter = 0;
            for (size_t k = 0; k < _vCount; ++k) {
                size_t j = i + k;
                j %= _vCount;
                size_t l = _vCount + i - k;
                l %= _vCount;
                double _coeff = sides[k] / _sides[j];
                double r_coeff = sides[k] / _sides[l];
                if (isEqual(coeff, _coeff)) {
                    ++counter;
                }
                if (isEqual(coeff, r_coeff)) {
                    ++r_counter;
                }
            }
            if (counter == _vCount || r_counter == _vCount) {
                return true;
            }
        }
        return false;
    }
    catch (std::bad_cast& message){
        return false;
    }
    return true;
}

bool Polygon::containsPoint(const Point& point) const {
    double sum = 0.;
    for (size_t i = 0; i < verticesCount(); ++i) {
        size_t j = (i + 1) % verticesCount();
        Point edge1(_vertices[i] - point);
        Point edge2(_vertices[j] - point);
        sum += std::fabs(atan2(edge1 % edge2, edge1 * edge2));
    }
    return isEqual(sum, M_PI * 2);
}

bool Polygon::operator == (const Shape& another) const {
    try{
        const auto polygon = dynamic_cast<const Polygon&>(another);
        if (_vCount != polygon.verticesCount()) {
            return false;
        }
        size_t start = -1;
        size_t r_start = -1;
        for (size_t i = 0; i < _vCount ; ++i) {
            if (_vertices[i] == polygon.getVertices()[0]) {
                start = i;
                r_start = i;
            }
        }
        if (start < 0) {
            return false;
        }
        for (size_t i = 0; i < _vCount ; ++i) {
            if ((_vertices[i] != polygon.getVertices()[start]) &&
                (_vertices[i] != polygon.getVertices()[r_start])) {
                return false;
            }
            start = (start + 1) % verticesCount();
            r_start = (r_start + verticesCount() - 1) % verticesCount();
        }
        return true;
    }
    catch (std::bad_cast& message){
        return false;
    }
    return true;
}

bool Polygon::operator != (const Shape& another) const {
    return !(*this == another);
}

void Polygon::rotate(const Point& center, double angle) {
    for (Point& vertice : _vertices) {
        vertice.rotate(center, angle);
    }
}

void Polygon::reflex(const Point& center) {
    for (Point& vertice : _vertices) {
        vertice.reflex(center);
    }
}

Point reflexPoint(Point& point, const Line& axis) {
    Line L1(point, point + axis.get_normalV());
    return lineIntersection(axis, L1);
}

void Polygon::reflex(const Line& axis) {
    for (auto& v: _vertices) {
        Point normalV = axis.get_normalV();
        double dist = (v - reflexPoint(v, axis)).get_length();
        normalV = (1/std::sqrt(normalV * normalV) * normalV * dist );
        Point v1 = v + 2 * normalV;
        Point v2 = v - 2 * normalV;
        v = isEqual((v1 - reflexPoint(v1, axis)).get_length(), dist) ? v1 : v2;
    }
}

void Polygon::scale(const Point& center, double coefficient) {
    for (Point& vertice : _vertices) {
        vertice.scale(center, coefficient);
    }
}

//__________________TRIANGLE__________________
class Triangle : public Polygon {
public:
    explicit Triangle(const Point& p1, const Point& p2, const Point& p3);
    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
    Point centroid() const;
    Point orthocenter() const;
    Line EulerLine() const;
    Circle ninePointsCircle() const;
private:
};

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3): Polygon(p1, p2, p3) {}

Circle Triangle::circumscribedCircle() const{
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Line m_per1((A+B)*0.5, (A+B)*0.5 + (A-B).get_normal());
    Line m_per2((A+C)*0.5, (A+C)*0.5 + (A-C).get_normal());
    Point center = lineIntersection(m_per1, m_per2);
    return {center, (A - center).get_length()};
}

Circle Triangle::inscribedCircle() const{
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    double a = (B - C).get_length();
    double b = (A - C).get_length();
    double c = (A - B).get_length();
    double perimeter = a + b + c;
    Point center((a * A.x + b * B.x + c * C.x)
                 /perimeter,
                 (a * A.y + b * B.y + c * C.y)
                 /perimeter);
    return Circle(center, 2 * area() / perimeter);
}

Point Triangle::centroid() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    return {(A.x + B.x + C.x) / 3,
            (A.y + B.y + C.y) / 3};
}

Point Triangle::orthocenter() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Line h1(A, A + (B - C).get_normal());
    Line h2(B, B + (A - C).get_normal());
    return lineIntersection(h1, h2);
}

Line Triangle::EulerLine() const {
    return Line(orthocenter(), centroid());
}

Circle Triangle::ninePointsCircle() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Triangle tmp((A+B) * 0.5, (A+C) * 0.5, (B+C) * 0.5);
    return tmp.circumscribedCircle();
}

//__________________RECTANGLE__________________
class Rectangle : public Polygon {
public:
    explicit Rectangle(const Point& p1, const Point& p2, double k);
    Point center() const ;
    std::pair<Line, Line> diagonals() const;
    double perimeter() const override;
    double area() const override;
};

Rectangle::Rectangle(const Point& p1, const Point& p2, double k) {
    k = k < 1 ? k : 1 / k;
    _vCount = 4;
    _vertices.resize(4, Point(0, 0));
    _vertices[0] = p1;
    _vertices[2] = p2;
    Point diagV = p2 - p1;
    double diag = diagV.get_length();
    double cosinus =  std::sqrt(1 / (k * k + 1));
    double sinus = std::sqrt(1 - cosinus*cosinus);
    double minSide = diag * cosinus;
    double maxSide = diag * sinus;
    if (maxSide < minSide) {
        std::swap(maxSide, minSide);
    }
    Point h = diagV.get_normal();
    h *= 1 / h.get_length();
    Point p3 = p1 + diagV * cosinus * cosinus + (diag * k / (k * k + 1)) * h;
    Point p4 = p1 + diagV * cosinus * cosinus - (diag * k / (k * k + 1)) * h;
    _vertices[1] = isEqual((p3 - p1).get_length(), minSide) && (h % diagV > 0)? p3 : p4;
    _vertices[3] = _vertices[1];
    _vertices[3].reflex(p1 + 0.5 * diagV);
}

Point Rectangle::center() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Point D = _vertices[3];
    return lineIntersection(Line(A, C - A), Line(B, D - B));
}

std::pair<Line, Line> Rectangle::diagonals() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Point D = _vertices[3];
    return {Line(A, C - A), Line(B, D - B)};
}

double Rectangle::perimeter() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Point D = _vertices[3];
    return 2 * ((A - B).get_length() + (C - D).get_length());
}

double Rectangle::area() const {
    Point A = _vertices[0];
    Point B = _vertices[1];
    Point C = _vertices[2];
    Point D = _vertices[3];
    return (A - B).get_length() * (C - D).get_length();
}


//__________________SQUARE__________________
class Square : public Rectangle {
public:
    Square(const Point& A, const Point& C);
    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
};


Square::Square(const Point& A, const Point& C): Rectangle(A, C, 1) {}

Circle Square::circumscribedCircle() const {
    return {center(), (center() - _vertices[0]).get_length()};
}

Circle Square::inscribedCircle() const {
    return {center(), (_vertices[1] - _vertices[0]).get_length()};
}
#endif 
/* geometry_h */
