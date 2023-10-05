#pragma once
#include <memory>
#include <algorithm>
#include "geometric_object.h"
#include "world.h"
#include "utilities.h"

static bool hit_aabb(raytracer::Ray r, raytracer::Interval ray_t, Eigen::AlignedBox3f aabb)
{
    for (int a = 0; a < 3; a++)
    {
        float invD = 1 / r.direction[a];
        float orig = r.origin[a];

        float t0 = (aabb.min()[a] - orig) * invD;
        float t1 = (aabb.max()[a] - orig) * invD;

        if (invD < 0)
            std::swap(t0, t1);

        if (t0 > ray_t.min)
            ray_t.min = t0;
        if (t1 < ray_t.max)
            ray_t.max = t1;

        if (ray_t.max <= ray_t.min)
            return false;
    }
    return true;
}

namespace raytracer
{
    class BVH_Node : public GeometricObject
    {
    public:
        BVH_Node(const std::vector<std::shared_ptr<GeometricObject>> &objects) : BVH_Node(objects, 0, objects.size()) {}

        BVH_Node(const std::vector<std::shared_ptr<GeometricObject>> &src_objects, size_t start, size_t end)
        {
            // ZoneScoped;
            auto objects = src_objects; // Create a modifiable array of the source scene objects

            int axis = random_int(0, 2);
            auto comparator = (axis == 0)   ? box_x_compare
                              : (axis == 1) ? box_y_compare
                                            : box_z_compare;

            size_t object_span = end - start;

            if (object_span == 1)
            {
                left = right = objects[start];
            }
            else if (object_span == 2)
            {
                if (comparator(objects[start], objects[start + 1]))
                {
                    left = objects[start];
                    right = objects[start + 1];
                }
                else
                {
                    left = objects[start + 1];
                    right = objects[start];
                }
            }
            else
            {
                // std::sort(objects.begin() + start, objects.begin() + end, comparator);
                auto mid = start + object_span / 2;
                std::nth_element(objects.begin() + start, objects.begin() + object_span / 2, objects.begin() + end, comparator);

                left = std::make_shared<BVH_Node>(objects, start, mid);
                right = std::make_shared<BVH_Node>(objects, mid, end);
            }
            
            aabb.extend(left->bounding_box());
            aabb.extend(right->bounding_box());
        }

        bool hit(const Ray &r, Interval ray_t, HitInfo &rec) const override
        {
            if (!hit_aabb(r, ray_t, aabb))
                return false;

            bool hit_left = left->hit(r, ray_t, rec);
            bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        static bool box_compare(
            const std::shared_ptr<GeometricObject> a, const std::shared_ptr<GeometricObject> b, int axis_index)
        {
            return a->bounding_box().min()[axis_index] < b->bounding_box().min()[axis_index];
        }

        static bool box_x_compare(const std::shared_ptr<GeometricObject> a, const std::shared_ptr<GeometricObject> b)
        {
            return box_compare(a, b, 0);
        }

        static bool box_y_compare(const std::shared_ptr<GeometricObject> a, const std::shared_ptr<GeometricObject> b)
        {
            return box_compare(a, b, 1);
        }

        static bool box_z_compare(const std::shared_ptr<GeometricObject> a, const std::shared_ptr<GeometricObject> b)
        {
            return box_compare(a, b, 2);
        }

        Eigen::AlignedBox3f bounding_box() const override { return aabb; }

    private:
        std::shared_ptr<GeometricObject> left;
        std::shared_ptr<GeometricObject> right;
        Eigen::AlignedBox3f aabb;
    };
}