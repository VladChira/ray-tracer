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
    if (ray.is_camera_ray && !this->visible_to_camera)
        return false;

    bool transformed = this->transform != nullptr;

    Ray tr;
    tr.origin = ray.origin;
    tr.direction = ray.direction;
    if (transformed)
    {
        tr.origin = Transform::Inverse(*transform).transform_point(ray.origin);
        tr.direction = Transform::Inverse(*transform).transform_vector(ray.direction);
    }

    Eigen::Vector3f v0 = mesh->vertices[v[0]];
    Eigen::Vector3f v1 = mesh->vertices[v[1]];
    Eigen::Vector3f v2 = mesh->vertices[v[2]];
    float a = v0.x() - v1.x(), b = v0.x() - v2.x(), c = tr.direction.x(), d = v0.x() - tr.origin.x();
    float e = v0.y() - v1.y(), f = v0.y() - v2.y(), g = tr.direction.y(), h = v0.y() - tr.origin.y();
    float i = v0.z() - v1.z(), j = v0.z() - v2.z(), k = tr.direction.z(), l = v0.z() - tr.origin.z();

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
    rec.p = tr.origin + t * tr.direction;

    rec.material = this->material;

    if (transformed)
    {
        rec.p = transform->transform_point(rec.p);
        rec.normal = transform->transform_normal(rec.normal);
    }

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

    if (this->transform != nullptr)
        return this->transform->transform_bounding_box(bbox);
    return bbox;
}

std::vector<std::shared_ptr<MeshTriangle>> raytracer::create_triangle_mesh(const tinyobj::attrib_t &attrib, const tinyobj::shape_t &shape, raytracer::ShadingType shading_type, std::shared_ptr<Material> mat, Transform *t)
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
            mesh->vertices[vertex_index] = Eigen::Vector3f(vx, vy, vz);
        }
        index_offset += fv;
    }

    for (size_t i = 0; i < nr_faces; i++)
    {
        MeshTriangle triangle(mesh, i, mat);
        triangle.set_transform(t);
        triangles.push_back(std::make_shared<MeshTriangle>(triangle));
    }

    // Compute the normals at each vertex, but don't bother if using flat shading
    if (shading_type == SMOOTH)
    {
        Console::GetInstance()->addWarningEntry("[Warning] Smooth shading currently has bad performance");
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
                if (triangle.v[0] == v || triangle.v[1] == v || triangle.v[2] == v)
                    weighted_normal += normal;
            }
            mesh->normals[v] = (weighted_normal).normalized();
        }
    }

    return triangles;
}
