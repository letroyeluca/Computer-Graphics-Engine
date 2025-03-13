//
// Created by Luca Letroye on 13/03/2025.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H
#include "easy_image.h"
#include "vector3d.h"
namespace img {
    class Color;
    class Point2D;
    class Line2D;
}


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

 Matrix scaleFigure(const double scale);
 Matrix rotateX(const double angle);
 Matrix rotateY(const double angle);
 Matrix rotateZ(const double angle);
 Matrix translate(const Vector3D &vector);
 void applyTransformation(Figure &fig, const Matrix &m);
 Matrix eyePointTrans(const Vector3D &eyepoint);
 std::vector<img::Line2D> doProjection(const std::vector<Figure> &figures);
 img::Point2D doProjection(const Vector3D &point,const double d);

#endif //ENGINE_FIGURE_H
