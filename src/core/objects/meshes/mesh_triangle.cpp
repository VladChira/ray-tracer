#include "mesh_triangle.h"
#include "matte.h"
using namespace raytracer;

MeshTriangle::MeshTriangle(const std::shared_ptr<Mesh> &mesh, int triangle_number, std::shared_ptr<Material> mat)
{
    this->mesh = mesh;
    v = &mesh->vertex_idx[3 * triangle_number];
    this->material = mat;

    // Compute the Normal - TODO this is not efficient
    Point3 v0 = mesh->vertices[v[0]];
    Point3 v1 = mesh->vertices[v[1]];
    Point3 v2 = mesh->vertices[v[2]];
}

bool MeshTriangle::hit(const raytracer::Ray &ray, Interval t_range, HitInfo &rec) const
{
    Point3 v0 = mesh->vertices[v[0]];
    Point3 v1 = mesh->vertices[v[1]];
    Point3 v2 = mesh->vertices[v[2]];
    double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;

    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, s = e * j - f * i;

    double inv_denom = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;

    if (beta < 0.0)
    {
        return false;
    }

    double r = e * l - h * i;
    double e2 = a * n + d * q + c * r;
    double gamma = e2 * inv_denom;

    if (gamma < 0.0)
    {
        return false;
    }

    if (beta + gamma > 1.0)
    {
        return false;
    }

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;
    

    if (t < std::numeric_limits<double>::epsilon())
    {
        return false;
    }

    rec.t = t;
    Normal3 normal = Normalize(Cross(v1 - v0, v2 - v0)); // TODO smooth shading
    
    rec.normal = normal;
    rec.p = ray.origin + t * ray.direction;

    rec.material = this->material;

    return true;
}

AABB MeshTriangle::bounding_box() const
{
    Point3 v0 = mesh->vertices[v[0]];
    Point3 v1 = mesh->vertices[v[1]];
    Point3 v2 = mesh->vertices[v[2]];
    const double delta = std::numeric_limits<double>::epsilon();
    Interval ix = Interval(std::min(std::min(v0.x, v1.x), v2.x) - delta, std::max(std::max(v0.x, v1.x), v2.x) + delta);
    Interval iy = Interval(std::min(std::min(v0.y, v1.y), v2.y) - delta, std::max(std::max(v0.y, v1.y), v2.y) + delta);
    Interval iz = Interval(std::min(std::min(v0.z, v1.z), v2.z) - delta, std::max(std::max(v0.z, v1.z), v2.z) + delta);

    return AABB(ix, iy, iz);
}

std::vector<std::shared_ptr<MeshTriangle>> raytracer::create_triangle_mesh(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape, std::shared_ptr<Material> mat)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    std::vector<std::shared_ptr<MeshTriangle>> triangles;

    mesh->nr_vertices = attrib.vertices.size() / 3;
    int nr_faces = shape.mesh.num_face_vertices.size();
    mesh->nr_triangles = nr_faces;

    mesh->vertices = std::make_unique<Point3[]>(mesh->nr_vertices);
    mesh->normals = std::make_unique<Normal3[]>(mesh->nr_vertices);
    
    mesh->vertex_idx.reserve(nr_faces * 3);
    size_t index_offset = 0;
    for (int f = 0; f < nr_faces; f++)
    {
        int fv = shape.mesh.num_face_vertices[f];
        assert(fv == 3);
        // Loop through the vertices of the current face
        for (int v = 0; v < fv; v++)
        {
            tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
            int vertex_index = idx.vertex_index;
            mesh->vertex_idx[index_offset + v] = vertex_index;

            double vx = attrib.vertices[3 * vertex_index + 0];
            double vy = attrib.vertices[3 * vertex_index + 1];
            double vz = attrib.vertices[3 * vertex_index + 2];
            mesh->vertices[vertex_index] = (10 * Point3(vx, vy, vz)) + Point3(7, -0.5, 2);
            // mesh->vertices[vertex_index] = Point3(vx, vy, vz);
        }
        index_offset += fv;
    }

    for (size_t i = 0; i < nr_faces; i++)
    {
        MeshTriangle triangle(mesh, i, mat);
        triangles.push_back(std::make_shared<MeshTriangle>(triangle));
    }

    // Compute the normals at each vertex
    // for (int v = 0; v < mesh->nr_vertices; v++)
    // {
    //     Normal3 weighted_normal(0, 0, 0);
    //     // For each triangle, does it have v as a vertex?
    //     for (int t = 0; t < triangles.size(); t++)
    //     {
    //         MeshTriangle triangle = *(triangles[t]);
    //         Point3 v0 = triangle.mesh->vertices[triangle.v[0]];
    //         Point3 v1 = triangle.mesh->vertices[triangle.v[1]];
    //         Point3 v2 = triangle.mesh->vertices[triangle.v[2]];
    //         Normal3 normal = Normalize(Cross(v1 - v0, v2 - v0));
    //         // std::cout << normal << "\n";
    //         if (triangle.v[0] == v || triangle.v[1] == v || triangle.v[2] == v)
    //             weighted_normal += normal;
    //     }
    //     mesh->normals[v] = Normalize(weighted_normal);
    //     // std::cout << "Vertex "<< v+1 << ": " << Normalize(weighted_normal) << "\n";
    // }

    return triangles;
}