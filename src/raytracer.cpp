#include <iostream>
#include "gui.h"
#include "clock_util.h"
#include "buffered_image.h"
#include "maths.h"
#include "ray.h"
#include "sphere.h"
#include "console.h"
#include "hittable_list.h"
#include <thread>
#include "renderview.h"
#include "pure_random.h"
#include "thinlens.h"

raytracer::Color3 ray_color(const raytracer::Ray &r, const raytracer::HittableList &world, raytracer::Sampler *sampler, int depth)
{
    if (depth <= 0)
        return raytracer::Color3(0, 0, 0);
    raytracer::hit_info rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        raytracer::Point3 target = rec.p + rec.normal + sampler->sample_sphere();
        return 0.5 * ray_color(raytracer::Ray(rec.p, target - rec.p), world, sampler, depth - 1);
    }

    raytracer::Vector3 unit_direction = raytracer::Normalize(r.direction);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * raytracer::Color3(1.0, 1.0, 1.0) + t * raytracer::Color3(0.5, 0.7, 1.0);
}

void render()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 850;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 20;
    raytracer::BufferedImage *image = new raytracer::BufferedImage(image_width, image_height);

    // World
    raytracer::HittableList world;
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(0.0, -100.5, -1.0), 100.0));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(-1.0, 0.0, -1.0), 0.5));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(-1.0, 0.0, -1.0), -0.45));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(1.0, 0.0, -1.0), 0.5));

    // Camera
    raytracer::Pinhole *camera = new raytracer::Pinhole(raytracer::Vector3(0, 0, 0), raytracer::Vector3(0, 0, -1));
    camera->set_view_distance(200);
    camera->compute_uvw();

    raytracer::Sampler *sampler = new raytracer::PureRandom(10);
    sampler->map_samples_to_sphere();

    HiResTimer timer;
    timer.start();

    int x = 0, y = 0;
    for (int j = image_height - 1; j >=0; --j)
    {
        y = 0;
        for (int i = 0; i < image_width; ++i)
        {
            raytracer::Color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                raytracer::Point2 p = sampler->sample_unit_square();
                auto u = i - 0.5 * image_width + p.x;
                auto v = j - 0.5 * image_height + p.y;
                raytracer::Ray r = camera->get_ray(raytracer::Point2(u, v));
                pixel_color += ray_color(r, world, sampler, max_depth);
            }
            image->at(x, y) = pixel_color / samples_per_pixel * 255.0;
            y++;
        }
        x++;
    }
    timer.stop();
    std::cout << "Elapsed time: " << timer.elapsed_time_seconds() << " seconds \n";
    Console::GetInstance()->appendLine("")->appendLine("Render finished! Elapsed time: " + std::to_string(timer.elapsed_time_seconds()) + " seconds.");

    // Perform gamma correction on the image
    image->apply_gamma_correction(2);

    FILE *output_file = fopen("../output.png", "wb");
    save_image_png(*image, output_file);
    fclose(output_file);

    // Display the image after rendering. TODO: Fix this abysmal implementation!
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->pixel_data = image->to_bytes();
    RenderView::GetInstance()->init = true;
    RenderView::GetInstance()->display_render = true;
}

int main()
{

    Console::GetInstance()->appendLine("A WIP ray tracer with minimal UI elements")->appendLine("Version 0.1.0-alpha")->appendLine("--- Made by Vlad Chira ---")->appendLine("");

    GUI gui;
    gui.init();

    std::thread render_thread(render);

    gui.run();

    render_thread.join();

    Console::GetInstance()->DestroyInstance();
    return 0;
}
