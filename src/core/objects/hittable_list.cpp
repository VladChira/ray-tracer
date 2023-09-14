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
}

bool HittableList::hit(const raytracer::Ray &r, double t_min, double t_max, HitInfo &rec) const
{
    HitInfo temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
