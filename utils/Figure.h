//
// Created by Luca Letroye on 13/03/2025.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H
#include "easy_image.h"

class Point2D {
public:
    double x;
    double y;
};

class Line2D {
public:
    Point2D p1;
    Point2D p2;
    img::Color color;
};


class Face{
public:
    std::vector<int> point_indexes;
};
class Figure{
public:
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    img::Color color;
};

static Matrix scaleFigure(const double scale);
static Matrix rotateX(const double angle);
static Matrix rotateY(const double angle);
static Matrix rotateZ(const double angle);
static Matrix translate(const Vector3D &vector);
static void applyTransformation(Figure &fig, const Matrix &m);
static Matrix eyePointTrans(const Vector3D &eyepoint);
static std::vector<Line2D> doProjection(const std::vector<Figure> &figures);
static Point2D doProjection(const Vector3D &point,const double d);

#endif //ENGINE_FIGURE_H
