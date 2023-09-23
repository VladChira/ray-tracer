#include "world.h"

void raytracer::World::add_object(std::shared_ptr<GeometricObject> object)
{
    objects.push_back(object);
    aabb = AABB(aabb, object->bounding_box());
    add_material(object->material);
}

void raytracer::World::add_light(std::shared_ptr<Light> light)
{
    lights.push_back(light);
}

void raytracer::World::add_material(std::shared_ptr<Material> mat)
{
    // Check if the material already exists in the list
    if (std::find(materials.begin(), materials.end(), mat) == materials.end())
    {
        // Material doesn't exist, add it to the list
        materials.push_back(mat);
    }
}

void raytracer::World::set_camera(std::shared_ptr<Camera> cam)
{
    camera = cam;
}

void raytracer::World::set_bg_color(Color3 c)
{
    background_color = c;
}

bool raytracer::World::hit_objects(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    double t_min = t_range.min;
    double t_max = t_range.max;
    HitInfo temp_rec(rec.world);
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(r, Interval(t_min, closest_so_far), temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}