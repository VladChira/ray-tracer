#include <iostream>
#include <thread>

#include "gui.h"
#include "clock_util.h"
#include "buffered_image.h"
#include "maths.h"
#include "ray.h"
#include "console.h"
#include "renderview.h"
#include "multijittered.h"
#include "thinlens.h"
#include "materials.h"
#include "objects.h"
#include "path_tracer.h"
#include "ray_caster.h"

// Image
const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 1000;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 50;
const int max_depth = 20;

// World
raytracer::HittableList world;

// Camera
raytracer::Pinhole *camera;

// Sampler
raytracer::Sampler *sampler;

// Tracer
raytracer::Tracer *tracer;

unsigned int num_threads = 6;

void render_region(raytracer::Point2 top_left, unsigned int width, unsigned int height)
{
    for (int i = top_left.y; i < top_left.y + height; ++i)
    {
        for (int j = top_left.x; j < top_left.x + width; ++j)
        {
            raytracer::Color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                raytracer::Point2 p = sampler->sample_unit_square();
                double pixel_size = camera->get_pixel_size();
                auto u = pixel_size * (j - 0.5 * image_width + p.x);
                auto v = pixel_size * (i - 0.5 * image_height + p.y);
                raytracer::Ray r = camera->get_ray(raytracer::Point2(u, v));

                pixel_color += tracer->trace_ray(r, world, max_depth);
            }
            pixel_color = pixel_color / samples_per_pixel * 255;
            RenderView::GetInstance()->set_pixel_color(image_height - i - 1, j, pixel_color);
        }
    }
}

void setup()
{
    auto material_ground = std::make_shared<raytracer::Matte>(0.8, raytracer::Color3(0.5, 0.5, 0.5));
    world.add(std::make_shared<raytracer::Plane>(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Normal3(0, 1.0, 0), material_ground));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            raytracer::Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - raytracer::Point3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<raytracer::Material> sphere_material;
                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto cd = raytracer::Color3(random_double(), random_double(), random_double());
                    // auto kd = random_double();
                    sphere_material = std::make_shared<raytracer::Matte>(1, cd);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto cr = raytracer::Color3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<raytracer::Reflective>(1, cr, fuzz);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<raytracer::Transparent>(1.5);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<raytracer::Transparent>(1.5);
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<raytracer::Matte>(1.0, raytracer::Color3(0.4, 0.2, 0.1));
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<raytracer::Reflective>(1, raytracer::Color3(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<raytracer::Sphere>(raytracer::Point3(4, 1, 0), 1.0, material3));

    // Camera
    camera = new raytracer::Pinhole(raytracer::Vector3(13, 2, 3), raytracer::Vector3(0, 0, 0));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();

    // Anti Aliasing Sampler
    sampler = new raytracer::MultiJittered(10);
    sampler->map_samples_to_sphere();

    // Tracer
    tracer = new raytracer::PathTracer();

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void multi_threaded_render()
{
    setup();

    HiResTimer timer;
    timer.start();

    std::vector<std::thread> threads;
    unsigned int region_width = image_width / num_threads;
    for (int i = 0; i < num_threads; ++i)
    {
        raytracer::Point2 top_left(i * region_width, 0);
        unsigned int width = (i == num_threads - 1) ? (image_width - top_left.x) : region_width;
        threads.emplace_back(render_region, top_left, width, image_height);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    RenderView::GetInstance()->image->apply_gamma_correction(1.2);

    timer.stop();
    std::cout << "Elapsed time: " << timer.elapsed_time_seconds() << " seconds \n";
    Console::GetInstance()->appendLine("")->appendLine("Render finished! Elapsed time: " + std::to_string(timer.elapsed_time_seconds()) + " seconds.");

    FILE *output_file = fopen("../output.png", "wb");
    save_image_png(*(RenderView::GetInstance()->image), output_file);
    fclose(output_file);
}

int main()
{
    Console::GetInstance()->appendLine("A WIP ray tracer with minimal UI elements")->appendLine("Version 0.1.0-alpha")->appendLine("--- Made by Vlad Chira ---")->appendLine("");

    GUI gui;
    gui.init();

    std::thread render_thread(multi_threaded_render);

    gui.run();

    render_thread.join();

    Console::GetInstance()->DestroyInstance();
    return 0;
}
