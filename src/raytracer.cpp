#include <iostream>
#include <thread>
#include <atomic>

// #include "Tracy.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

#include "gui.h"
#include "clock_util.h"
#include "maths.h"
#include "console.h"
#include "renderview.h"
#include "multijittered.h"
#include "pure_random.h"
#include "materials.h"
#include "pinhole.h"
#include "path_tracer.h"
#include "ray_tracer.h"
#include "preview_tracer.h"
#include "world.h"
#include "bvh.h"
#include "sphere.h"
#include "rectangle.h"
#include "obj_loader.h"
#include "mesh_triangle.h"
#include "directional.h"
#include "point_light.h"
#include "emissive.h"
#include "area_light.h"

using namespace raytracer;

const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 700;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 10;
const int max_depth = 10;

// World
World world;

// Sampler
std::shared_ptr<Sampler> sampler;

// Tracer
std::shared_ptr<Tracer> tracer;

unsigned int num_threads = std::thread::hardware_concurrency() - 1;

std::atomic<bool> exit_requested(false);

void render_region(Point2 top_left, unsigned int width, unsigned int height)
{
    // ZoneScoped;
    for (int i = top_left.y; i < top_left.y + height; ++i)
    {
        for (int j = top_left.x; j < top_left.x + width; ++j)
        {
            Color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                // Gracefully kill the threads
                if (exit_requested.load(std::memory_order_relaxed))
                    return;
                Point2 p = sampler->sample_unit_square();
                double pixel_size = world.camera->get_pixel_size();
                auto u = pixel_size * (j - 0.5 * image_width + p.x);
                auto v = pixel_size * (i - 0.5 * image_height + p.y);
                Ray r = world.camera->get_ray(Point2(u, v));

                pixel_color += tracer->trace_ray(r, world, max_depth);
            }
            pixel_color = Clamp((pixel_color / samples_per_pixel), 0.0, 1.0).gamma_corrected() * 255.0;

            RenderView::GetInstance()->set_pixel_color(image_height - i - 1, j, pixel_color);
        }
    }
}

void test()
{
    world.add_object(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Matte>(0.7, Color3::orange)));
    world.add_object(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Matte>(0.7, Color3::blue)));

    auto difflight = std::make_shared<Emissive>(4.0, Color3::white);
    auto light_rect = std::make_shared<Rectangle>(Point3(3, 1, -2), Vector3(2, 0, 0), Vector3(0, 2, 0), difflight);
    // world.add_object(light_rect);

    // auto area_light = std::make_shared<AreaLight>();
    // area_light->set_object(light_rect);
    // world.add_light(area_light);

    auto light = std::make_shared<Directional>(4.0, Color3::white, Normalize(Vector3(0, 2, -1)));
    world.add_light(light);

    // Tracer
    tracer = std::make_shared<PathTracer>();

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Vector3(26, 3, 6), Vector3(0, 2, 0));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Sampler
    sampler = std::make_shared<MultiJittered>(100);

    Console::GetInstance()->addLogEntry("Constructing BVH...");
    auto bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void cornell_box()
{
    auto red = std::make_shared<Matte>(1.0, Color3(.65, .05, .05));
    auto white = std::make_shared<Matte>(1.0, Color3(.73, .73, .73));
    auto green = std::make_shared<Matte>(1.0, Color3(.12, .45, .15));
    auto glass = std::make_shared<Transparent>(1.5);

    auto light_mat = std::make_shared<Emissive>(15.0, Color3::white);
    auto light_rect = std::make_shared<Rectangle>(Point3(343, 554, 332), Vector3(-130, 0, 0), Vector3(0, 0, -105), light_mat);
    world.add_object(light_rect);

    auto area_light = std::make_shared<AreaLight>();
    area_light->set_object(light_rect);
    world.add_light(area_light);

    world.add_object(std::make_shared<Rectangle>(Point3(555, 0, 0), Vector3(0, 0, 555), Vector3(0, 555, 0), green));
    world.add_object(std::make_shared<Rectangle>(Point3(0, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), red));

    world.add_object(std::make_shared<Rectangle>(Point3(0, 0, 0), Vector3(0, 0, 555), Vector3(555, 0, 0), white));
    world.add_object(std::make_shared<Rectangle>(Point3(555, 555, 555), Vector3(-555, 0, 0), Vector3(0, 0, -555), white));
    world.add_object(std::make_shared<Rectangle>(Point3(0, 0, 555), Vector3(0, 555, 0), Vector3(555, 0, 0), white));

    world.add_objects(create_box(Point3(130, 0, 65), Point3(295, 165, 230), white));
    world.add_objects(create_box(Point3(265, 0, 295), Point3(430, 330, 460), white));

    // Tracer
    tracer = std::make_shared<PathTracer>();
    world.tracer = tracer;

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Vector3(278, 278, -800), Vector3(278, 278, 0));
    camera->set_fov(40);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Sampler
    sampler = std::make_shared<MultiJittered>(100);

    Console::GetInstance()->addLogEntry("Constructing BVH...");
    auto bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void setup4()
{
    auto mat = std::make_shared<Matte>(0.8, Color3::orange);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    bool loaded = LoadObj("../models/bunny/bunny.obj", attrib, shapes, materials);

    if (loaded)
    {
        auto triangles = create_triangle_mesh(attrib, shapes[0], ShadingType::SMOOTH, mat);
        for (int i = 0; i < triangles.size(); i++)
        {
            world.add_object(triangles[i]);
        }
    }

    world.add_object(std::make_shared<Sphere>(Vector3(-3.0, 0.0, 0.0), 1.0, mat));

    // Lights
    auto light1 = std::make_shared<Directional>(3.0, Color3::white, Vector3(0, 1, 0));
    world.add_light(light1);

    auto light2 = std::make_shared<Directional>(2.0, Color3::white, Vector3(0, -1, 0));
    world.add_light(light2);

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Vector3(13, 2, 3), Vector3(0, 1, 0));
    camera->set_fov(10);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Anti Aliasing Sampler
    sampler = std::make_shared<MultiJittered>(100);

    Console::GetInstance()->addLogEntry("Constructing BVH...");
    auto bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Tracer
    tracer = std::make_shared<PathTracer>();

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void setup3()
{
    // ZoneScoped;
    auto mat = std::make_shared<Matte>(1, Color3::grey);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    bool loaded = LoadObj("../models/bucatarie/buc2.obj", attrib, shapes, materials);

    if (loaded)
    {
        auto triangles = create_triangle_mesh(attrib, shapes[0], ShadingType::FLAT, mat);
        for (int i = 0; i < triangles.size(); i++)
        {
            world.add_object(triangles[i]);
        }
    }

    // auto light_mat = std::make_shared<Emissive>(15.0, Color3::white);
    // auto light_rect = std::make_shared<Rectangle>(Point3(-18, 250, -242), Vector3(-100, 0, 0), Vector3(0, 0, -100), light_mat);
    // world.add_object(light_rect);

    // auto area_light = std::make_shared<AreaLight>();
    // area_light->set_object(light_rect);
    // world.add_light(area_light);

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Vector3(163, 187, -730), Vector3(-18, 104, -242));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Construct the BVH
    HiResTimer timer;
    Console::GetInstance()->addLogEntry("Constructing BVH...");
    timer.start();
    auto bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);
    timer.stop();
    Console::GetInstance()->addSuccesEntry("BVH built in " + std::to_string(timer.elapsed_time_milliseconds()) + " ms.");

    // Anti Aliasing Sampler
    sampler = std::make_shared<MultiJittered>(100);

    // Tracer
    tracer = std::make_shared<PathTracer>();
    world.tracer = tracer;

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void setup2()
{
    // Lights
    auto light1 = std::make_shared<Directional>(1.0, Color3::white, Vector3(1, 0, 0));
    world.add_light(light1);

    // Tracer
    tracer = std::make_shared<PathTracer>();

    auto mat1 = std::make_shared<Matte>(1, Color3::orange);
    world.add_object(std::make_shared<Sphere>(Vector3(5, 3, 0), 30, mat1));

    auto mat2 = std::make_shared<Matte>(0.8, Color3::brown);
    world.add_object(std::make_shared<Sphere>(Point3(45, -7, -60), 20, mat2));

    auto mat3 = std::make_shared<Matte>(1, Color3::dark_green);
    world.add_object(std::make_shared<Sphere>(Point3(40, 43, -100), 17, mat3));

    auto mat4 = std::make_shared<Reflective>(0.8, Color3::red, 0.1);
    world.add_object(std::make_shared<Sphere>(Point3(-20, 28, -15), 20, mat4));

    auto mat5 = std::make_shared<Matte>(0.8, Color3::green);
    world.add_object(std::make_shared<Sphere>(Point3(-25, -7, -35), 27, mat5));

    auto mat6 = std::make_shared<Matte>(0.8, Color3::green);
    world.add_object(std::make_shared<Sphere>(Point3(20, -27, -35), 25, mat5));

    auto mat7 = std::make_shared<Transparent>(1.5);
    world.add_object(std::make_shared<Sphere>(Point3(35, 18, -35), 22, mat7));

    auto mat8 = std::make_shared<Reflective>(0.8, Color3::brown, 0.0);
    world.add_object(std::make_shared<Sphere>(Point3(-57, -17, -50), 15, mat8));

    auto mat9 = std::make_shared<Matte>(0.8, Color3::green);
    world.add_object(std::make_shared<Sphere>(Point3(-47, 16, -80), 23, mat9));

    auto mat10 = std::make_shared<Matte>(0.8, Color3::dark_green);
    world.add_object(std::make_shared<Sphere>(Point3(-15, -32, -60), 22, mat10));

    auto mat11 = std::make_shared<Matte>(0.8, Color3::yellow);
    world.add_object(std::make_shared<Sphere>(Point3(-35, -37, -80), 22, mat11));

    auto mat12 = std::make_shared<Matte>(0.8, Color3::yellow);
    world.add_object(std::make_shared<Sphere>(Point3(10, 43, -80), 22, mat12));

    auto mat13 = std::make_shared<Matte>(0.8, Color3::yellow);
    world.add_object(std::make_shared<Sphere>(Point3(30, -7, -80), 10, mat13));

    auto mat14 = std::make_shared<Transparent>(1.5);
    world.add_object(std::make_shared<Sphere>(Point3(-40, 48, -110), 18, mat14));

    auto mat15 = std::make_shared<Matte>(0.8, Color3::brown);
    world.add_object(std::make_shared<Sphere>(Point3(-10, 53, -120), 18, mat15));

    auto mat16 = std::make_shared<Matte>(0.8, Color3::blue);
    world.add_object(std::make_shared<Sphere>(Point3(-55, -52, -100), 10, mat16));

    auto mat17 = std::make_shared<Matte>(0.8, Color3::brown);
    world.add_object(std::make_shared<Sphere>(Point3(5, -52, -100), 15, mat17));

    auto mat18 = std::make_shared<Matte>(0.8, Color3::purple);
    world.add_object(std::make_shared<Sphere>(Point3(-20, -57, -120), 15, mat18));

    auto mat19 = std::make_shared<Matte>(0.8, Color3::green);
    world.add_object(std::make_shared<Sphere>(Point3(55, -27, -100), 17, mat19));

    auto mat20 = std::make_shared<Matte>(0.8, Color3::brown);
    world.add_object(std::make_shared<Sphere>(Point3(50, -47, -120), 15, mat20));

    auto mat21 = std::make_shared<Matte>(0.8, Color3::purple);
    world.add_object(std::make_shared<Sphere>(Point3(70, -42, -150), 10, mat21));

    auto mat22 = std::make_shared<Matte>(0.8, Color3::purple);
    world.add_object(std::make_shared<Sphere>(Point3(5, 73, -130), 12, mat22));

    auto mat23 = std::make_shared<Matte>(0.8, Color3::purple);
    world.add_object(std::make_shared<Sphere>(Point3(66, 21, -130), 13, mat23));

    auto mat24 = std::make_shared<Matte>(0.8, Color3::purple);
    world.add_object(std::make_shared<Sphere>(Point3(72, -12, -140), 12, mat24));

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Vector3(0, 0, 400), Vector3(5, 0, 0));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Construct the BVH
    auto bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Anti Aliasing Sampler
    sampler = std::make_shared<MultiJittered>(100);

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void setup()
{
    // ZoneScoped;
    // Tracer
    tracer = std::make_shared<PathTracer>();

    // Lights
    // auto light1 = std::make_shared<PointLight>(1.0, Color3::white, Vector3(5, 10, 0));
    // world.add_light(light1);

    auto mat = std::make_shared<Matte>(0.8, Color3::orange);
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    LoadObj("../models/bunny/bunny.obj", attrib, shapes, materials);

    auto triangles = raytracer::create_triangle_mesh(attrib, shapes[0], ShadingType::SMOOTH, mat);
    for (int i = 0; i < triangles.size(); i++)
    {
        world.add_object(triangles[i]);
    }

    auto material_ground = std::make_shared<Matte>(0.8, Color3(0.5, 0.5, 0.5));
    world.add_object(std::make_shared<Sphere>(Vector3(0.0, -1900.0, 0.0), -1900.0, material_ground));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto cd = Color3(random_double(), random_double(), random_double());
                    // auto kd = random_double();
                    sphere_material = std::make_shared<Matte>(1, cd);
                    world.add_object(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto cr = Color3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    auto fuzz = random_double(0, 0.2);
                    sphere_material = std::make_shared<Reflective>(1, cr, fuzz);
                    world.add_object(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // lights
                    sphere_material = std::make_shared<Emissive>(1, Color3::white);
                    world.add_object(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Transparent>(1.5);
    world.add_object(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Matte>(1.0, Color3(0.4, 0.2, 0.1));
    world.add_object(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Emissive>(1, Color3::white);
    world.add_object(std::make_shared<Sphere>(Point3(4, 1, 0), 0.5, material3));

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Vector3(13, 2, 3), Vector3(0, 0, 0));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    HiResTimer timer;
    Console::GetInstance()->addLogEntry("Constructing BVH...");
    timer.start();
    auto bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);
    timer.stop();
    Console::GetInstance()->addSuccesEntry("BVH built in " + std::to_string(timer.elapsed_time_milliseconds()) + " ms.");

    // Anti Aliasing Sampler
    sampler = std::make_shared<MultiJittered>(100);
    sampler->map_samples_to_sphere();

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void multi_threaded_render()
{
    // ZoneScoped;
    HiResTimer timer;
    timer.start();

    // Build a particular scene here
    Console::GetInstance()->addLogEntry("Building scene...");

    setup3();

    Console::GetInstance()->addLogEntry("Rendering...");
    std::vector<std::thread> threads;
    unsigned int region_width = image_width / num_threads;
    for (int i = 0; i < num_threads; ++i)
    {
        Point2 top_left(i * region_width, 0);
        unsigned int width = (i == num_threads - 1) ? (image_width - top_left.x) : region_width;
        threads.emplace_back(render_region, top_left, width, image_height);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    if (exit_requested.load(std::memory_order_relaxed))
        return;

    timer.stop();
    Console::GetInstance()->addEmptyLine()->addSuccesEntry("Render finished! Elapsed time: " + std::to_string(timer.elapsed_time_seconds()) + " seconds.");

    FILE *output_file = fopen("../output.png", "wb");
    int result = BufferedImage::save_image_png(*(RenderView::GetInstance()->image), output_file);
    fclose(output_file);
    if (result == 0)
        Console::GetInstance()->addSuccesEntry("Image saved to disk.");
    else
        Console::GetInstance()->addErrorEntry("Failed to save image to disk");
}

int main()
{
    Console::GetInstance()->addLogEntry("A WIP ray tracer with minimal UI elements")->addLogEntry("Version 0.1.1-alpha")->addLogEntry("--- Made by Vlad Chira ---")->addEmptyLine();

    GUI gui;
    gui.init();

    std::thread render_thread(multi_threaded_render);

    gui.run();

    render_thread.join();

    Console::GetInstance()->DestroyInstance();
    return 0;
}
