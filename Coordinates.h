//
// Created by Gabriel Spranger Rojas on 10/19/19.
//

#ifndef GRAPHOS_COORDINATES_H
#define GRAPHOS_COORDINATES_H

struct Coordinate2D {
    float x, y;
    Coordinate2D(): x{0}, y{0} {}
    Coordinate2D(float x, float y): x{x}, y{y} {}
};

struct Coordinate3D {
    float x, y, z;
    Coordinate3D(): x{0}, y{0}, z{0} {}
    Coordinate3D(float x, float y, float z): x{x}, y{y}, z{z} {}
};

#endif //GRAPHOS_COORDINATES_H
