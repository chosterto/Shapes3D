#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "render.h"

#define FOV 90


class Cube
{
private:
    std::vector<Vector3D> p3D;
    std::vector<Vector3D> transformed;
    std::vector<Vector2D> p2D;

    // Have the camera set at the origin 
    double Cx = 0.0;
    double Cy = 0.0;
    double magnify = 1.0;
    // No rotations upon initialization
    double a_x = 0.0;
    double a_y = 0.0;
    double a_z = 0.0;
    
    std::vector< std::vector<double> > projection = {
        {1.0/std::tan((FOV / 2.0) * (M_PI / 180.0)), 0, 0},
        {0, 1.0/std::tan((FOV / 2.0) * (M_PI / 180.0)), 0},
        {0, 0, 1}
    };

    auto MatrixMultiply(std::vector<std::vector<double>> A, std::vector<std::vector<double>> B)
    {
        // int n = A.size();
        // int m = A[0].size();
        // int p = B[0].size();
        std::vector<std::vector<double>> C;

        C.push_back( {B[0][0]*A[0][0] + B[1][0]*A[0][1] + B[2][0]*A[0][2]} );
        C.push_back( {B[0][0]*A[1][0] + B[1][0]*A[1][1] + B[2][0]*A[1][2]} );
        C.push_back( {B[0][0]*A[2][0] + B[1][0]*A[2][1] + B[2][0]*A[2][2]} );

        // for (int i = 0; i < n; i++)
        // {
        //     std::vector<double> v;
        //     C.push_back(v);
        //     for (int j = 0; j < p; j++)
        //     {
        //         double sum = 0;
        //         for (int k = 0; k < m; k++)
        //         {
        //             sum += A[i][k] * B[k][j];
        //         }
        //         C[i].push_back(sum);
        //     }
        // }

        return C;
    }

    std::vector<std::vector<double>> VecToMatrix(Vector3D v)
    {
        std::vector<std::vector<double>> matrix = {{0.0}, {0.0}, {0.0}};
        matrix[0][0] = v.x;
        matrix[1][0] = v.y;
        matrix[2][0] = v.z;
        return matrix;
    }

    void Update2D()
    {
        p2D.clear();
        for (Vector3D p : transformed)
        {
            Vector2D v2D;
            std::vector<std::vector<double>> _2D = MatrixMultiply(projection, VecToMatrix(p));
            // Scale the z value down
            double F = _2D[2][0] / 200.0;

            // Calculate x y coordinates relative to camera position
            // Translate to center of canvas
            v2D.x = ( (_2D[0][0] - Cx) / ( (1.0 / magnify) + F) ) + ( (double)WIDTH / 2.0);
            v2D.y = ( (_2D[1][0] - Cy) / ( (1.0 / magnify) + F) ) + ( (double)HEIGHT / 2.0);
            p2D.push_back(v2D);
        }
    }

    void RotateAxis()
    {
        std::vector< std::vector<double> > transformX = {
            {1, 0, 0},
            {0, std::cos(a_x), std::sin(a_x)},
            {0, -std::sin(a_x), std::cos(a_x)}
        };

        std::vector< std::vector<double> > transformY = {
            {std::cos(a_y), 0, -std::sin(a_y)},
            {0, 1, 0},
            {std::sin(a_y), 0, std::cos(a_y)}
        };

        std::vector< std::vector<double> > transformZ = {
            {std::cos(a_z), std::sin(a_z), 0},
            {-std::sin(a_z), std::cos(a_z), 0},
            {0, 0, 1}
        };

        std::vector<Vector3D> ptsRotated;
        for (Vector3D p : p3D)
        {
            std::vector<std::vector<double>> rotated = MatrixMultiply(transformX, VecToMatrix(p));
            rotated = MatrixMultiply(transformY, rotated);
            rotated = MatrixMultiply(transformZ, rotated);

            Vector3D pRotated;
            pRotated.x = rotated[0][0];
            pRotated.y = rotated[1][0];
            pRotated.z = rotated[2][0];
            ptsRotated.push_back(pRotated);
        }
        transformed = ptsRotated;
        Update2D();
    }

public:
    void Create_Cube(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        if (p3D.size() != 0)
            p3D.clear();

        // Vertices
        //     5------6
        // 1-------2  |
        // |   |   |  |
        // |   |   |  |
        // |   7---|--8
        // 3-------4
        for (int i = 0; i < 8; i++)
        {
            Vector3D v3D;
            v3D.x = (i % 2) ? x2 : x1;
            // v3D.y = ( (i >> 1) & 1) ? y2 : y1;
            v3D.y = (i & 2) ? y2 : y1;
            v3D.z = (i >> 2) ? z2 : z1;

            p3D.push_back(v3D);
        }

        transformed = p3D;
        Update2D();
    }

    std::vector<Vector2D> GetPoints2D()
    {
        return p2D;
    }

    std::vector<Vector3D> GetPoints3D()
    {
        return transformed;
    }

    void SetCamera(double x, double y)
    {
        Cx = x;
        Cy = y;
        Update2D();
    }

    void SetZoom(double amount)
    {
        magnify = amount;
        // Clamp magnification
        magnify = std::max(0.1, std::min(4.0, magnify));

        Update2D();
    }

    void RotateX(double a)
    {
        a_x += a;
        RotateAxis();
    }

    void RotateY(double a)
    {
        a_y += a;
        RotateAxis();
    }

    void RotateZ(double a)
    {
        a_z += a;
        RotateAxis();
    }

};