#pragma once
#include <vector>
#include <memory>
#include "utilities.h"
#include "obj_loader.h"
namespace raytracer
{
    enum ShadingType
    {
        FLAT,
        SMOOTH
    };
    
    class Mesh
    {
    public:
        int nr_triangles, nr_vertices;
        bool has_normals = false;
        std::vector<int> vertex_idx;
        std::unique_ptr<Eigen::Vector3f[]> vertices;
        std::unique_ptr<Eigen::Vector3f[]> normals;
        ShadingType shading_type = ShadingType::SMOOTH;

        // std::vector<std::vector<int> > vertex_faces;  // a list of all the triangles that share a particular vertex

        Mesh() {}
    };
}