//
// Created by Luca Letroye on 13/03/2025.
//

#include "Figure.h"
#include "easy_image.h"

Matrix scaleFigure(const double scale){
    Matrix matrix;


    // Then apply scale factors
    matrix(1,1) = scale;
    matrix(2,2) = scale;
    matrix(3,3) = scale;
    // matrix(4,4) is already set to 1 by the identity initialization
    return matrix;
}

Matrix rotateX(const double angle){
    Matrix matrix;

    // Apply X rotation
    matrix(2,2) = cos(angle * M_PI/180);
    matrix(2,3) = sin(angle* M_PI/180);
    matrix(3,2) = -sin(angle* M_PI/180);
    matrix(3,3) = cos(angle* M_PI/180);
    // matrix(1,1) and matrix(4,4) remain 1.0 from identity initialization
    return matrix;
}

Matrix rotateY(const double angle){
    Matrix matrix;


    // Apply Y rotation
    matrix(1,1) = cos(angle* M_PI/180);
    matrix(1,3) = -sin(angle* M_PI/180);
    matrix(3,1) = sin(angle* M_PI/180);
    matrix(3,3) = cos(angle* M_PI/180);
    // matrix(2,2) and matrix(4,4) remain 1.0 from identity initialization
    return matrix;
}

Matrix rotateZ(const double angle){
    Matrix matrix;

    // Apply Z rotation
    matrix(1,1) = cos(angle* M_PI/180);
    matrix(1,2) = sin(angle* M_PI/180);
    matrix(2,1) = -sin(angle* M_PI/180);
    matrix(2,2) = cos(angle* M_PI/180);
    // matrix(3,3) and matrix(4,4) remain 1.0 from identity initialization
    return matrix;
}

Matrix translate(const Vector3D &vector){
    Matrix matrix;

    // Apply translation
    matrix(4,1) = vector.x;
    matrix(4,2) = vector.y;
    matrix(4,3) = vector.z;

    // matrix(4,4) remains 1.0 from identity initialization
    return matrix;
}

void applyTransformation(Figure &fig, const Matrix &m){
    for(auto &point: fig.points){
        point *= m;
    }
}



Matrix eyePointTrans(const Vector3D &eyepoint) {
    Matrix matrix;
    double r = sqrt(eyepoint.x * eyepoint.x + eyepoint.y * eyepoint.y + eyepoint.z * eyepoint.z);
    double theta = atan2(eyepoint.y, eyepoint.x);
    double phi = acos(eyepoint.z / r);

    matrix(1,1) = -sin(theta);
    matrix(1,2) = ((-cos(theta)) * (cos(phi)));
    matrix(1,2) = ((-cos(theta)) * (cos(phi)));
    matrix(1,3) = ((cos(theta)) * (sin(phi)));
    matrix(2,1) = ((cos(theta)));
    matrix(2,2) = ((-sin(theta)) * (cos(phi)));
    matrix(2,3) = ((sin(theta)) * (sin(phi)));
    matrix(3,2) = ((sin(phi)));
    matrix(3,3) = ((cos(phi)));
    matrix(4,3) = -r;


    return matrix;
}

std::vector<img::Line2D> doProjection(const std::vector<Figure> &figures){
    std::vector<img::Line2D> allLines;

    for (auto &fig : figures){
        for(auto &face: fig.faces){
            for(int i = 0; i < face.point_indexes.size(); i++){
                Vector3D point1 = fig.points[face.point_indexes[i]];
                Vector3D point2 = fig.points[face.point_indexes[(i+1)%face.point_indexes.size()]];
                img::Point2D projectedPoint1 = doProjection(point1, 1);
                img::Point2D projectedPoint2 = doProjection(point2, 1);
                img::Line2D line;
                line.p1 = projectedPoint1;
                line.p2 = projectedPoint2;
                line.color = fig.color;
                allLines.push_back(line);
            }
        }
    }

    return allLines;
}

img::Point2D doProjection(const Vector3D &point,const double d){
    img::Point2D projectedPoint;
    projectedPoint.x = (d * point.x) / -point.z;
    projectedPoint.y = (d * point.y) / -point.z;
    return projectedPoint;
}