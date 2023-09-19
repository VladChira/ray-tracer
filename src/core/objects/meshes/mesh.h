#pragma once
#include <vector>
#include <memory>
#include "vector3.h"
#include "obj_loader.h"
namespace raytracer
{
    class Mesh
    {
    public:
        int nr_triangles, nr_vertices;
        std::vector<int> vertex_idx;
        std::unique_ptr<Point3[]> vertices;
        std::unique_ptr<Normal3[]> normals;

        Mesh();
    };
}