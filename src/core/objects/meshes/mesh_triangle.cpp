#include "mesh_triangle.h"
#include "matte.h"
using namespace raytracer;

MeshTriangle::MeshTriangle(const std::shared_ptr<Mesh> &mesh, int triangle_number, std::shared_ptr<Material> mat)
{
    this->mesh = mesh;
    v = &mesh->vertex_idx[3 * triangle_number];
    this->material = mat;
}

bool MeshTriangle::hit(const raytracer::Ray &ray, Interval t_range, HitInfo &rec) const
{
    Eigen::Vector3f v0 = mesh->vertices[v[0]];
    Eigen::Vector3f v1 = mesh->vertices[v[1]];
    Eigen::Vector3f v2 = mesh->vertices[v[2]];
    float a = v0.x() - v1.x(), b = v0.x() - v2.x(), c = ray.direction.x(), d = v0.x() - ray.origin.x();
    float e = v0.y() - v1.y(), f = v0.y() - v2.y(), g = ray.direction.y(), h = v0.y() - ray.origin.y();
    float i = v0.z() - v1.z(), j = v0.z() - v2.z(), k = ray.direction.z(), l = v0.z() - ray.origin.z();

    float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    float q = g * i - e * k, s = e * j - f * i;

    float inv_denom = 1.0 / (a * m + b * q + c * s);

    float e1 = d * m - b * n - c * p;
    float beta = e1 * inv_denom;

    if (beta < 0.0)
    {
        return false;
    }

    float r = e * l - h * i;
    float e2 = a * n + d * q + c * r;
    float gamma = e2 * inv_denom;

    if (gamma < 0.0)
    {
        return false;
    }

    if (beta + gamma > 1.0)
    {
        return false;
    }

    float e3 = a * p - b * r + d * s;
    float t = e3 * inv_denom;

    if (t < t_range.min || t > t_range.max)
        return false;

    rec.t = t;
    Eigen::Vector3f normal;

    // Interpolate normals to archieve smooth shading, if available
    if (mesh->shading_type == SMOOTH && mesh->has_normals)
        normal = ((1 - beta - gamma) * mesh->normals[v[0]] + beta * mesh->normals[v[1]] + gamma * mesh->normals[v[2]]).normalized();
    else
        normal = ((v1 - v0).cross(v2 - v0)).normalized();
        

    rec.normal = normal;
    rec.p = ray.origin + t * ray.direction;

    rec.material = this->material;

    return true;
}

Eigen::AlignedBox3f MeshTriangle::bounding_box() const
{
    Eigen::Vector3f v0 = mesh->vertices[v[0]];
    Eigen::Vector3f v1 = mesh->vertices[v[1]];
    Eigen::Vector3f v2 = mesh->vertices[v[2]];

    Eigen::AlignedBox3f bbox;
    bbox.extend(v0);
    bbox.extend(v1);
    bbox.extend(v2);

    // Displace the corners by a tiny amount to avoid degenerate bounding boxes
    bbox.min() -= Eigen::Vector3f(0.0001, 0.0001, 0.0001);
    bbox.max() += Eigen::Vector3f(0.0001, 0.0001, 0.0001);

    return bbox;
}

std::vector<std::shared_ptr<MeshTriangle>> raytracer::create_triangle_mesh(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape, raytracer::ShadingType shading_type, std::shared_ptr<Material> mat)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->shading_type = shading_type;
    std::vector<std::shared_ptr<MeshTriangle>> triangles;

    mesh->nr_vertices = attrib.vertices.size() / 3;
    int nr_faces = shape.mesh.num_face_vertices.size();
    mesh->nr_triangles = nr_faces;

    mesh->vertices = std::make_unique<Eigen::Vector3f[]>(mesh->nr_vertices);

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

            float vx = attrib.vertices[3 * vertex_index + 0];
            float vy = attrib.vertices[3 * vertex_index + 1];
            float vz = attrib.vertices[3 * vertex_index + 2];
            // mesh->vertices[vertex_index] = (10 * Eigen::Vector3f(vx, vy, vz)) + Eigen::Vector3f(7, -0.5, 2);
            mesh->vertices[vertex_index] = Eigen::Vector3f(vx, vy, vz);
        }
        index_offset += fv;
    }

    for (size_t i = 0; i < nr_faces; i++)
    {
        MeshTriangle triangle(mesh, i, mat);
        triangles.push_back(std::make_shared<MeshTriangle>(triangle));
    }

    // Compute the normals at each vertex, but don't bother if using flat shading
    if (shading_type == SMOOTH)
    {
        // TODO, precompute vertexes shared among triangles to make this run in O(nr_vertices)
        mesh->has_normals = true;
        mesh->normals = std::make_unique<Eigen::Vector3f[]>(mesh->nr_vertices);
        for (int v = 0; v < mesh->nr_vertices; v++)
        {
            Eigen::Vector3f weighted_normal(0, 0, 0);
            // For each triangle, does it have v as a vertex?
            for (int t = 0; t < triangles.size(); t++)
            {
                MeshTriangle triangle = *(triangles[t]);
                Eigen::Vector3f v0 = triangle.mesh->vertices[triangle.v[0]];
                Eigen::Vector3f v1 = triangle.mesh->vertices[triangle.v[1]];
                Eigen::Vector3f v2 = triangle.mesh->vertices[triangle.v[2]];
                Eigen::Vector3f normal = ((v1 - v0).cross(v2 - v0)).normalized();
                // std::cout << normal << "\n";
                if (triangle.v[0] == v || triangle.v[1] == v || triangle.v[2] == v)
                    weighted_normal += normal;
            }
            mesh->normals[v] = (weighted_normal).normalized();
        }
    }

    return triangles;
}

// TODO Implement these with Mesh and MeshTriangle instead
// std::vector<std::shared_ptr<SmoothTriangle>> raytracer::tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps, std::shared_ptr<Material> mat)
// {
//     std::vector<std::shared_ptr<SmoothTriangle>> objects;
//     int k = 1;

//     for (int j = 0; j <= horizontal_steps - 1; j++)
//     {
//         // define vertices

//         Point3 v0(0, 1, 0); // top

//         Point3 v1(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // bottom left
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//         Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // bottom  right
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

//         SmoothTriangle triangle_ptr(v0, v1, v2, mat);
//         triangle_ptr.n0 = v0;
//         triangle_ptr.n1 = v1;
//         triangle_ptr.n2 = v2;
//         objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr));
//     }

//     // define the bottom triangles

//     k = vertical_steps - 1;

//     for (int j = 0; j <= horizontal_steps - 1; j++)
//     {
//         // define vertices

//         Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//         Point3 v1(0, -1, 0); // bottom

//         Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

//         SmoothTriangle triangle_ptr(v0, v1, v2, mat);
//         triangle_ptr.n0 = v0;
//         triangle_ptr.n1 = v1;
//         triangle_ptr.n2 = v2;
//         objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr));
//     }

//     //  define the other triangles

//     for (int k = 1; k <= vertical_steps - 2; k++)
//     {
//         for (int j = 0; j <= horizontal_steps - 1; j++)
//         {
//             // define the first triangle

//             // vertices

//             Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom left, use k + 1, j
//                       cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

//             Point3 v1(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
//                       cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

//             Point3 v2(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
//                       cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//             SmoothTriangle triangle_ptr1(v0, v1, v2, mat);
//             triangle_ptr1.n0 = v0;
//             triangle_ptr1.n1 = v1;
//             triangle_ptr1.n2 = v2;
//             objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr1));

//             // define the second triangle

//             // vertices

//             v0 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right, use k, j + 1
//                         cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

//             v1 = Point3(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
//                         cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//             v2 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
//                         cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

//             SmoothTriangle triangle_ptr2(v0, v1, v2, mat);
//             triangle_ptr2.n0 = v0;
//             triangle_ptr2.n1 = v1;
//             triangle_ptr2.n2 = v2;
//             objects.push_back(std::make_shared<SmoothTriangle>(triangle_ptr2));
//         }
//     }
//     return objects;
// }

// std::vector<std::shared_ptr<Triangle>> raytracer::tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps, std::shared_ptr<Material> mat)
// {
//     std::vector<std::shared_ptr<Triangle>> objects;
//     // define the top triangles which all touch the north pole
//     int k = 1;

//     for (int j = 0; j <= horizontal_steps - 1; j++)
//     {
//         // define vertices

//         Point3 v0(0, 1, 0); // top (north pole)

//         Point3 v1(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // bottom left
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//         Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // bottom  right
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

//         Triangle triangle_ptr(v0, v1, v2, mat);
//         objects.push_back(std::make_shared<Triangle>(triangle_ptr));
//     }

//     // define the bottom triangles which all touch the south pole

//     k = vertical_steps - 1;

//     for (int j = 0; j <= horizontal_steps - 1; j++)
//     {
//         // define vertices

//         Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//         Point3 v1(0, -1, 0); // bottom (south pole)

//         Point3 v2(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right
//                   cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

//         Triangle triangle_ptr(v0, v1, v2, mat);
//         objects.push_back(std::make_shared<Triangle>(triangle_ptr));
//     }

//     //  define the other triangles

//     for (int k = 1; k <= vertical_steps - 2; k++)
//     {
//         for (int j = 0; j <= horizontal_steps - 1; j++)
//         {
//             // define the first triangle

//             // vertices

//             Point3 v0(sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom left, use k + 1, j
//                       cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

//             Point3 v1(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
//                       cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

//             Point3 v2(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
//                       cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//             Triangle triangle_ptr1(v0, v1, v2, mat);
//             objects.push_back(std::make_shared<Triangle>(triangle_ptr1));

//             // define the second triangle

//             // vertices

//             v0 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps), // top right, use k, j + 1
//                         cos(pi * k / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps));

//             v1 = Point3(sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps), // top left,     use k, j
//                         cos(pi * k / vertical_steps), cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps));

//             v2 = Point3(sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps), // bottom  right, use k + 1, j + 1
//                         cos(pi * (k + 1) / vertical_steps), cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps));

//             Triangle triangle_ptr2(v0, v1, v2, mat);
//             objects.push_back(std::make_shared<Triangle>(triangle_ptr2));
//         }
//     }
//     return objects;
// }
