#include <iostream>
#include <thread>

#include "gui.h"
#include "clock_util.h"
#include "buffered_image.h"
#include "maths.h"
#include "ray.h"
#include "console.h"
#include "renderview.h"
#include "pure_random.h"
#include "thinlens.h"
#include "materials.h"
#include "objects.h"
#include "path_tracer.h"
#include "ray_caster.h"

void render()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 20;
    const int max_depth = 10;

    // World
    std::shared_ptr<raytracer::Material> material_ground = std::make_shared<raytracer::Matte>(1.0, raytracer::Color3(0.8, 0.8, 0.0));
    std::shared_ptr<raytracer::Material> material1 = std::make_shared<raytracer::Matte>(1.0, raytracer::Color3(0.7, 0.3, 0.3));
    std::shared_ptr<raytracer::Material> material2 = std::make_shared<raytracer::Matte>(1.0, raytracer::Color3(0.098, 0.3529, 0.69));

    raytracer::HittableList world;
    
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(0.0, -190.5, -1.0), 190.0, material_ground));
    // world.add(std::make_shared<raytracer::Plane>(raytracer::Point3(0.0, -0.4, 0), raytracer::Normal3(0.0, 1.0, 0.0), material_ground));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(-1.0, 0.0, -1.8), 0.5, material1));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(2.3, 0.0, -0.4), 0.5, material2));

    // Camera
    auto *camera = new raytracer::Pinhole(raytracer::Vector3(0, 0.8, -5), raytracer::Vector3(0, 0, -1));
    // camera->initialize(aspect_ratio, image_width);
    camera->set_view_distance(500);
    // camera->set_focal_dist(2000);
    // camera->set_lens_radius(5);
    camera->compute_uvw();

    // Anti Aliasing Sampler
    raytracer::Sampler *sampler = new raytracer::PureRandom(10);
    sampler->map_samples_to_sphere();

    // Tracer
    raytracer::Tracer *tracer = new raytracer::PathTracer();

    // Timer
    HiResTimer timer;
    timer.start();

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;

    int pixel_counter = 0;
    for (int i = 0; i < image_height; ++i)
    {
        for (int j = 0; j < image_width; ++j)
        {
            raytracer::Color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                raytracer::Point2 p = sampler->sample_unit_square();
                auto u = j - 0.5 * image_width + p.x;
                auto v = i - 0.5 * image_height + p.y;
                raytracer::Ray r = camera->get_ray(raytracer::Point2(u, v));

                pixel_color += tracer->trace_ray(r, world, max_depth);
            }
            pixel_color = pixel_color / samples_per_pixel * 255;
            RenderView::GetInstance()->set_pixel_color(image_height - i - 1, j, pixel_color);
        }
    }
    timer.stop();
    std::cout << "Elapsed time: " << timer.elapsed_time_seconds() << " seconds \n";
    Console::GetInstance()->appendLine("")->appendLine("Render finished! Elapsed time: " + std::to_string(timer.elapsed_time_seconds()) + " seconds.");

    // Perform gamma correction on the image
    // image->apply_gamma_correction(2);

    FILE *output_file = fopen("../output.png", "wb");
    save_image_png(*(RenderView::GetInstance()->image), output_file);
    fclose(output_file);
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
