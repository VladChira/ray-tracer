#pragma once
#include <memory>
#include <vector>
#include <algorithm>

#include "hittable_list.h"
#include "material.h"
#include "light.h"
#include "camera.h"

namespace raytracer
{
    class World
    {
    public:
        HittableList objects;
        std::vector<std::shared_ptr<raytracer::Material>> materials;
        std::vector<std::shared_ptr<raytracer::Light>> lights;
        std::shared_ptr<raytracer::Camera> camera;
        Color3 background_color;

        void add_object(std::shared_ptr<GeometricObject> object)
        {
            objects.add(object);
            add_material(object->material);
        }

        void add_light(std::shared_ptr<Light> light)
        {
            lights.push_back(light);
        }

        void add_material(std::shared_ptr<Material> mat)
        {
            // Check if the material already exists in the list
            if (std::find(materials.begin(), materials.end(), mat) == materials.end())
            {
                // Material doesn't exist, add it to the list
                materials.push_back(mat);
            }
        }

        void set_camera(std::shared_ptr<Camera> cam)
        {
            camera = cam;
        }

        void set_bg_color(Color3 c)
        {
            background_color = c;
        }
    };
}