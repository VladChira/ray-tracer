#pragma once
#include <iostream>

#include <Eigen/Geometry>

#include "geometric_object.h"
#include "mesh.h"

namespace raytracer
{
    /*
    *   A Mesh Triangle is different from a regular triangle primitive.
        It stores a pointer to the mesh that it is part of and
        a pointer to the first vertex in the mesh's array of vertices.
        The second and third vertices can be accessed based on the first.
    */

    class MeshTriangle : public GeometricObject
    {
    public:
        MeshTriangle(const std::shared_ptr<Mesh> &mesh, int triangle_number, std::shared_ptr<Material> mat);
        bool hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const override;
        Eigen::AlignedBox3f bounding_box() const override;

        std::ostream &operator<<(std::ostream &out)
        {
            out << "Vertex 1: " << mesh->vertices[v[0]] << "\n";
            out << "Vertex 2: " << mesh->vertices[v[0]] << "\n";
            out << "Vertex 3: " << mesh->vertices[v[0]] << "\n\n";
            return out;
        }

    public:
        std::shared_ptr<Mesh> mesh;
        const int *v;
    };

    std::vector<std::shared_ptr<MeshTriangle>> create_triangle_mesh(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape, ShadingType shading_type, std::shared_ptr<Material> mat);
}