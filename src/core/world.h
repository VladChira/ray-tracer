#pragma once
#include <memory>
#include <vector>
#include <algorithm>

#include <Eigen/Geometry>

#include "utilities.h"
#include "geometric_object.h"
#include "hit_info.h"
#include "image_texture.h"

namespace raytracer
{
    class Material;
    class Light;
    class Camera;
    class Tracer;

    class World
    {
    public:
        std::vector<std::shared_ptr<GeometricObject>> objects;
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Light>> lights;

        std::shared_ptr<Camera> camera;
        Color background_color;
        Eigen::AlignedBox3f aabb; // the AABB of the entire scene

        std::shared_ptr<Tracer> tracer;

        std::shared_ptr<ImageTexture> hdri = nullptr;

        void add_object(std::shared_ptr<GeometricObject> object);

        void add_objects(std::vector<std::shared_ptr<GeometricObject>> objects);

        void add_light(std::shared_ptr<Light> light);

        void add_material(std::shared_ptr<Material> mat);

        void set_camera(std::shared_ptr<Camera> cam);

        void set_bg_color(Color c);

        bool hit_objects(const raytracer::Ray &r, Interval t_range, HitInfo &rec);
    };
}