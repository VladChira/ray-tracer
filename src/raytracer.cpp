#include <iostream>
#include <thread>
#include <atomic>

// #include "Tracy.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

#include "gui.h"
#include "clock_util.h"
#include "utilities.h"
#include "console.h"
#include "renderview.h"
#include "multijittered.h"
#include "pure_random.h"
#include "materials.h"
#include "pinhole.h"
#include "path_tracer.h"
#include "direct_lighting_tracer.h"
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
#include "image_texture.h"
#include "spherical_mapping.h"

using namespace raytracer;

const auto aspect_ratio = 16.0 / 9.0;
const int image_width = 700;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 20;
const int max_depth = 10;

// World
World world;

// Sampler
std::shared_ptr<Sampler> sampler = nullptr;

// Tracer
std::shared_ptr<Tracer> tracer = nullptr;

// BVH
std::shared_ptr<BVH_Node> bvh = nullptr;

unsigned int num_threads = std::thread::hardware_concurrency() - 1;

std::atomic<bool> exit_requested(false);

void render_region(Eigen::Vector2f top_left, unsigned int width, unsigned int height)
{
    // ZoneScoped;
    for (int i = top_left.y(); i < top_left.y() + height; ++i)
    {
        for (int j = top_left.x(); j < top_left.x() + width; ++j)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                // Gracefully kill this thread
                if (exit_requested.load(std::memory_order_relaxed))
                    return;
                Eigen::Vector2f p = sampler->sample_unit_square();
                double pixel_size = world.camera->get_pixel_size();
                auto u = pixel_size * (j - 0.5 * image_width + p.x());
                auto v = pixel_size * (i - 0.5 * image_height + p.y());
                Ray r = world.camera->get_ray(Eigen::Vector2f(u, v));
                r.is_camera_ray = true;

                pixel_color += tracer->trace_ray(r, world, max_depth);
            }
            pixel_color = Clamp((pixel_color / samples_per_pixel), 0.0, 1.0).gamma_corrected() * 255.0;

            RenderView::GetInstance()->set_pixel_color(image_height - i - 1, j, pixel_color);
        }
    }
}

void test()
{

    auto solid_color = std::make_shared<ConstantTexture>(Color(0, 0.427, 0.435));
    auto material_ground = std::make_shared<Matte>(0.8, solid_color);
    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(-200, 0, -200), Eigen::Vector3f(400, 0, 0), Eigen::Vector3f(0, 0, 400), Eigen::Vector3f(0, 1.0, 0.0), material_ground));
    
    auto texture = std::make_shared<ImageTexture>(std::make_unique<SphericalMapping>(), "../src/uv_checker_texture.png");
    Transform *t = new Transform;
    *t = Transform::Scale(Eigen::Vector3f(1.0, 1.0, 1.0));
    auto sphere = std::make_shared<Sphere>(Eigen::Vector3f(0, 1, 0), 1, std::make_shared<Matte>(1.0, texture));
    sphere->set_transform(t);
    world.add_object(sphere);

    // auto difflight = std::make_shared<Emissive>(4.0, Color::white);
    // auto light_rect = std::make_shared<Rectangle>(Eigen::Vector3f(3, 1, -2), Eigen::Vector3f(2, 0, 0), Eigen::Vector3f(0, 2, 0), difflight);
    // world.add_object(light_rect);

    // auto area_light = std::make_shared<AreaLight>();
    // area_light->set_object(light_rect);
    // world.add_light(area_light);

    // auto light = std::make_shared<Directional>(4.0, Color::white, (Eigen::Vector3f(0, 2, -1).normalized()));
    // world.add_light(light);

    // Tracer
    tracer = std::make_shared<PathTracer>();

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(26, 3, 6), Eigen::Vector3f(0, 1, 0));
    camera->set_fov(10);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Sampler
    sampler = std::make_shared<MultiJittered>(100);

    Console::GetInstance()->addLogEntry("Constructing BVH...");
    bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void knob_test()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    bool loaded = LoadObj("../models/mori.obj", attrib, shapes, materials);

    auto floor = std::make_shared<Matte>(1.0, Color(0.1, 0.1, 0.1));

    auto outer = std::make_shared<Matte>(0.8, Color::cyan);

    auto inner = std::make_shared<Matte>(1.0, Color::grey);

    if (loaded)
    {

        auto floor_mesh = create_triangle_mesh(attrib, shapes[0], ShadingType::SMOOTH, floor, nullptr);
        for (int i = 0; i < floor_mesh.size(); i++)
        {
            world.add_object(floor_mesh[i]);
        }

        auto outer_mesh = create_triangle_mesh(attrib, shapes[1], ShadingType::SMOOTH, outer, nullptr);
        for (int i = 0; i < outer_mesh.size(); i++)
        {
            world.add_object(outer_mesh[i]);
        }

        auto inner_mesh = create_triangle_mesh(attrib, shapes[2], ShadingType::SMOOTH, inner, nullptr);
        for (int i = 0; i < inner_mesh.size(); i++)
        {
            world.add_object(inner_mesh[i]);
        }
    }

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(-30, 50, -80), Eigen::Vector3f(-4, 20, 0));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Anti Aliasing Sampler
    sampler = std::make_shared<MultiJittered>(100);

    Console::GetInstance()->addLogEntry("Constructing BVH...");
    bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Tracer
    tracer = std::make_shared<PathTracer>();

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}


void cornell_box()
{
    auto red = std::make_shared<Matte>(1.0, Color(.65, .05, .05));
    auto white = std::make_shared<Matte>(1.0, Color(.73, .73, .73));
    auto green = std::make_shared<Matte>(1.0, Color(.12, .45, .15));
    auto azure = std::make_shared<Matte>(1.0, Color(.176, .392, 0.96));
    auto glass = std::make_shared<Transparent>(1.5);
    auto reflect = std::make_shared<Reflective>(1.0, Color::white, 0.0);

    auto orange = std::make_shared<Matte>(1.0, Color::orange);

    auto light_mat = std::make_shared<Emissive>(15.0, Color::white);
    auto light_rect = std::make_shared<Rectangle>(Eigen::Vector3f(403, 554, 332), Eigen::Vector3f(-250, 0, 0), Eigen::Vector3f(0, 0, -120), light_mat);
    world.add_object(light_rect);

    auto area_light = std::make_shared<AreaLight>();
    area_light->set_object(light_rect);
    world.add_light(area_light);

    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(780, 0, 0), Eigen::Vector3f(0, 0, 555), Eigen::Vector3f(0, 555, 0), green));
    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(-224, 0, 0), Eigen::Vector3f(0, 555, 0), Eigen::Vector3f(0, 0, 555), red));

    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(-224, 0, 0), Eigen::Vector3f(0, 0, 555), Eigen::Vector3f(1004, 0, 0), white)); // floor
    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(780, 555, 555), Eigen::Vector3f(-1004, 0, 0), Eigen::Vector3f(0, 0, -555), white)); // ceiling
    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(-224, 0, 555), Eigen::Vector3f(0, 555, 0), Eigen::Vector3f(1004, 0, 0), white)); // back wall

    // The wall in front of the camera, invisible to it but contributes to all other light computation
    auto front_wall = std::make_shared<Rectangle>(Eigen::Vector3f(-224, 0, 0), Eigen::Vector3f(1004, 0, 0),Eigen::Vector3f(0, 555, 0), azure);
    front_wall->visible_to_camera = false;
    world.add_object(front_wall);

    Transform *rot = new Transform;
    *rot = Transform::Translate(Eigen::Vector3f(170, 130, 150)) * Transform::RotateY(35.0);
    world.add_objects(create_box(150, 260, 150, white, rot));

    Transform *rot2 = new Transform;
    *rot2 = Transform::Translate(Eigen::Vector3f(360, 200, 350)) * Transform::RotateY(-10.0);
    world.add_objects(create_box(150, 400, 150, orange, rot2));

    auto sphere1 = std::make_shared<Sphere>(Eigen::Vector3f::Zero(), 110, reflect);
    Transform *t1 = new Transform;
    *t1 = Transform::Translate(Eigen::Vector3f(600, 230, 250)) * Transform::Scale(Eigen::Vector3f(1, 1.8, 1));
    sphere1->set_transform(t1);
    world.add_object(sphere1);

    auto sphere2 = std::make_shared<Sphere>(Eigen::Vector3f::Zero(), 110, glass);
    Transform *t2 = new Transform;
    *t2 = Transform::Translate(Eigen::Vector3f(-90, 200, 250));
    sphere2->set_transform(t2);
    world.add_object(sphere2);

    // Tracer
    tracer = std::make_shared<DirectLightingTracer>();
    world.tracer = tracer;

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(278, 278, -800), Eigen::Vector3f(278, 278, 0));
    camera->set_fov(40);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    // Sampler
    sampler = std::make_shared<MultiJittered>(100);

    Console::GetInstance()->addLogEntry("Constructing BVH...");
    bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);

    // Start viewport preview
    RenderView::GetInstance()->set_size(image_width, image_height);
    RenderView::GetInstance()->display_render = true;
}

void setup4()
{
    auto mat = std::make_shared<Matte>(0.8, Color::orange);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    bool loaded = LoadObj("../models/bunny/bunny.obj", attrib, shapes, materials);

    if (loaded)
    {
        auto triangles = create_triangle_mesh(attrib, shapes[0], ShadingType::SMOOTH, mat, nullptr);
        for (int i = 0; i < triangles.size(); i++)
        {
            world.add_object(triangles[i]);
        }
    }

    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-3.0, 0.0, 0.0), 1.0, mat));

    // Lights
    // auto light1 = std::make_shared<Directional>(3.0, Color::white, Eigen::Vector3f(0, 1, 0));
    // world.add_light(light1);

    // auto light2 = std::make_shared<Directional>(2.0, Color::white, Eigen::Vector3f(0, -1, 0));
    // world.add_light(light2);

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(13, 2, 3), Eigen::Vector3f(0, 1, 0));
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
    auto mat = std::make_shared<Matte>(1, Color::grey);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    bool loaded = LoadObj("../models/bucatarie/buc2.obj", attrib, shapes, materials);

    if (loaded)
    {
        auto triangles = create_triangle_mesh(attrib, shapes[0], ShadingType::FLAT, mat, nullptr);
        for (int i = 0; i < triangles.size(); i++)
        {
            world.add_object(triangles[i]);
        }
    }

    auto light_mat = std::make_shared<Emissive>(15.0, Color::white);
    auto light_rect = std::make_shared<Rectangle>(Eigen::Vector3f(-18, 250, -242), Eigen::Vector3f(-100, 0, 0), Eigen::Vector3f(0, 0, -100), light_mat);
    world.add_object(light_rect);

    auto area_light = std::make_shared<AreaLight>();
    area_light->set_object(light_rect);
    world.add_light(area_light);

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(163, 187, -730), Eigen::Vector3f(-18, 104, -242));
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
    // auto light1 = std::make_shared<Directional>(1.0, Color::white, Eigen::Vector3f(1, 0, 0));
    // world.add_light(light1);

    // Tracer
    tracer = std::make_shared<PathTracer>();

    auto mat1 = std::make_shared<Matte>(1, Color::orange);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(5, 3, 0), 30, mat1));

    auto mat2 = std::make_shared<Matte>(0.8, Color::brown);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(45, -7, -60), 20, mat2));

    auto mat3 = std::make_shared<Matte>(1, Color::dark_green);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(40, 43, -100), 17, mat3));

    auto mat4 = std::make_shared<Reflective>(0.8, Color::red, 0.1);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-20, 28, -15), 20, mat4));

    auto mat5 = std::make_shared<Matte>(0.8, Color::green);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-25, -7, -35), 27, mat5));

    auto mat6 = std::make_shared<Matte>(0.8, Color::green);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(20, -27, -35), 25, mat5));

    auto mat7 = std::make_shared<Transparent>(1.5);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(35, 18, -35), 22, mat7));

    auto mat8 = std::make_shared<Reflective>(0.8, Color::brown, 0.0);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-57, -17, -50), 15, mat8));

    auto mat9 = std::make_shared<Matte>(0.8, Color::green);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-47, 16, -80), 23, mat9));

    auto mat10 = std::make_shared<Matte>(0.8, Color::dark_green);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-15, -32, -60), 22, mat10));

    auto mat11 = std::make_shared<Matte>(0.8, Color::yellow);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-35, -37, -80), 22, mat11));

    auto mat12 = std::make_shared<Matte>(0.8, Color::yellow);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(10, 43, -80), 22, mat12));

    auto mat13 = std::make_shared<Matte>(0.8, Color::yellow);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(30, -7, -80), 10, mat13));

    auto mat14 = std::make_shared<Transparent>(1.5);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-40, 48, -110), 18, mat14));

    auto mat15 = std::make_shared<Matte>(0.8, Color::brown);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-10, 53, -120), 18, mat15));

    auto mat16 = std::make_shared<Matte>(0.8, Color::blue);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-55, -52, -100), 10, mat16));

    auto mat17 = std::make_shared<Matte>(0.8, Color::brown);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(5, -52, -100), 15, mat17));

    auto mat18 = std::make_shared<Matte>(0.8, Color::purple);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-20, -57, -120), 15, mat18));

    auto mat19 = std::make_shared<Matte>(0.8, Color::green);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(55, -27, -100), 17, mat19));

    auto mat20 = std::make_shared<Matte>(0.8, Color::brown);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(50, -47, -120), 15, mat20));

    auto mat21 = std::make_shared<Matte>(0.8, Color::purple);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(70, -42, -150), 10, mat21));

    auto mat22 = std::make_shared<Matte>(0.8, Color::purple);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(5, 73, -130), 12, mat22));

    auto mat23 = std::make_shared<Matte>(0.8, Color::purple);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(66, 21, -130), 13, mat23));

    auto mat24 = std::make_shared<Matte>(0.8, Color::purple);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(72, -12, -140), 12, mat24));

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(0, 0, 400), Eigen::Vector3f(5, 0, 0));
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
    // Tracer
    tracer = std::make_shared<PathTracer>();
    world.tracer = tracer;

    auto mat = std::make_shared<Matte>(0.8, Color::orange);
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    LoadObj("../models/bunny/bunny.obj", attrib, shapes, materials);

    Transform *bunny_transform = new Transform;
    *bunny_transform = Transform::Translate(Eigen::Vector3f(7, -0.5, 2)) * Transform::Scale(Eigen::Vector3f(10, 10, 10));
    auto triangles = raytracer::create_triangle_mesh(attrib, shapes[0], ShadingType::FLAT, mat, bunny_transform);
    for (int i = 0; i < triangles.size(); i++)
    {
        world.add_object(triangles[i]);
    }

    auto material_ground = std::make_shared<Matte>(0.8, Color(0.5, 0.5, 0.5));
    world.add_object(std::make_shared<Rectangle>(Eigen::Vector3f(-200, 0, -200), Eigen::Vector3f(400, 0, 0), Eigen::Vector3f(0, 0, 400), Eigen::Vector3f(0, 1.0, 0.0), material_ground));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_float();
            Eigen::Vector3f center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

            if ((center - Eigen::Vector3f(4, 0.2, 0)).norm() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto cd = Color(random_float(), random_float(), random_float());
                    // auto kd = random_float();
                    sphere_material = std::make_shared<Matte>(1, cd);
                    world.add_object(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto cr = Color(random_float(0.5, 1), random_float(0.5, 1), random_float(0.5, 1));
                    auto fuzz = random_float(0, 0.2);
                    sphere_material = std::make_shared<Reflective>(1, cr, fuzz);
                    world.add_object(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // lights
                    sphere_material = std::make_shared<Emissive>(1, Color::white);
                    world.add_object(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Transparent>(1.5);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Matte>(1.0, Color(0.4, 0.2, 0.1));
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Emissive>(1, Color::white);
    world.add_object(std::make_shared<Sphere>(Eigen::Vector3f(4, 1, 0), 0.5, material3));

    // Camera
    std::shared_ptr<Pinhole> camera = std::make_shared<Pinhole>(Eigen::Vector3f(13, 2, 3), Eigen::Vector3f(0, 0, 0));
    camera->set_fov(20);
    camera->compute_pixel_size(image_width, image_height);
    camera->compute_uvw();
    world.set_camera(camera);

    HiResTimer timer;
    Console::GetInstance()->addLogEntry("Constructing BVH...");
    timer.start();
    bvh = std::make_shared<BVH_Node>(world.objects);
    world.objects.clear();
    world.objects.push_back(bvh);
    timer.stop();
    Console::GetInstance()->addSuccesEntry("BVH built in " + std::to_string(timer.elapsed_time_milliseconds()) + " ms.");

    // Anti Aliasing Sampler
    sampler = std::make_shared<MultiJittered>(100);

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

    test();


    if (sampler == nullptr)
    {
         Console::GetInstance()->addErrorEntry("[Error] No sampler object found. Aborting render...");
         return;
    }

    if (tracer == nullptr)
    {
         Console::GetInstance()->addErrorEntry("[Error] No tracer object found. Aborting render...");
         return;
    }

    if (bvh == nullptr)
        Console::GetInstance()->addWarningEntry("[Warning] No Bounding Volume Hierarchy found. Rendering performance will be severely affected!");


    Console::GetInstance()->addLogEntry("Rendering...");
    std::vector<std::thread> threads;
    unsigned int region_width = image_width / num_threads;
    for (int i = 0; i < num_threads; ++i)
    {
        Eigen::Vector2f top_left(i * region_width, 0);
        unsigned int width = (i == num_threads - 1) ? (image_width - top_left.x()) : region_width;
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
    
    // Wait a little to finish opengl buffer update
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    RenderView::GetInstance()->finished = true;

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
