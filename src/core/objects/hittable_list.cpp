#include "hittable_list.h"
using namespace raytracer;

HittableList::HittableList()
{
}

HittableList::HittableList(std::shared_ptr<raytracer::GeometricObject> object)
{
    add(object);
}

void HittableList::clear()
{
    objects.clear();
}

void HittableList::add(std::shared_ptr<raytracer::GeometricObject> object)
{
    objects.push_back(object);
    // Add the bounding box of the new object to the bounding box of all objects
    aabb = AABB(aabb, object->bounding_box());
}

bool HittableList::hit(const raytracer::Ray &r, Interval t_range, HitInfo &rec) const
{
    double t_min = t_range.min;
    double t_max = t_range.max;
    HitInfo temp_rec;
    temp_rec.lights = rec.lights;
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
