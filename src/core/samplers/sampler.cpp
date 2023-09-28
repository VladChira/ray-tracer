#include "sampler.h"
#include <utility>
#include <algorithm>
using namespace raytracer;

Sampler::Sampler() : num_samples(1), num_sets(83), count(0), jump(0) {
    samples.reserve(num_samples * num_sets);
    setup_shuffled_indices();
}

Sampler::Sampler(const int ns) : num_samples(ns), num_sets(83), count(0), jump(0) {
    samples.reserve(num_samples * num_sets);
    setup_shuffled_indices();
}

Sampler::Sampler(const int ns, const int n_sets) : num_samples(ns), num_sets(n_sets), count(0), jump(0) {
    samples.reserve(num_samples * num_sets);
    setup_shuffled_indices();
}

Sampler::~Sampler() {}

Sampler::Sampler(const Sampler& s)
    : num_samples(s.num_samples),
      num_sets(s.num_sets),
      samples(s.samples),
      shuffled_indices(s.shuffled_indices),
      disk_samples(s.disk_samples),
      hemisphere_samples(s.hemisphere_samples),
      sphere_samples(s.sphere_samples),
      count(s.count),
      jump(s.jump) {}

Sampler::Sampler(Sampler&& s) noexcept
    : num_samples(std::exchange(s.num_samples, 1)),
      num_sets(std::exchange(s.num_sets, 83)),
      samples(std::move(s.samples)),
      shuffled_indices(std::move(s.shuffled_indices)),
      disk_samples(std::move(s.disk_samples)),
      hemisphere_samples(std::move(s.hemisphere_samples)),
      sphere_samples(std::move(s.sphere_samples)),
      count(std::exchange(s.count, 0)),
      jump(std::exchange(s.jump, 0)) {}

Sampler& Sampler::operator=(const Sampler& s) {
    num_samples = s.num_samples;
    num_sets = s.num_sets;
    samples = s.samples;
    shuffled_indices = s.shuffled_indices;
    disk_samples = s.disk_samples;
    hemisphere_samples = s.hemisphere_samples;
    sphere_samples = s.sphere_samples;
    count = s.count;
    jump = s.jump;

    return *this;
}

Sampler& Sampler::operator=(Sampler&& s) noexcept {
    num_samples = std::exchange(s.num_samples, 1);
    num_sets = std::exchange(s.num_sets, 83);
    samples = std::move(s.samples);
    shuffled_indices = std::move(s.shuffled_indices);
    disk_samples = std::move(s.disk_samples);
    hemisphere_samples = std::move(s.hemisphere_samples);
    sphere_samples = std::move(s.sphere_samples);
    count = std::exchange(s.count, 0);
    jump = std::exchange(s.jump, 0);

    return *this;
}

void Sampler::set_num_sets(const int np) { num_sets = np; }

int Sampler::get_num_samples() { return num_samples; }

// shuffle the x coordinates of the points within each set

void Sampler::shuffle_x_coordinates() {
    for (int p = 0; p < num_sets; p++) {
        for (int i = 0; i < num_samples - 1; i++) {
            int target = random_int() % num_samples + p * num_samples;
            double temp = samples[i + p * num_samples + 1].x;
            samples[i + p * num_samples + 1].x = samples[target].x;
            samples[target].x = temp;
        }
    }
}

// shuffle the y coordinates of the points within set

void Sampler::shuffle_y_coordinates() {
    for (int p = 0; p < num_sets; p++) {
        for (int i = 0; i < num_samples - 1; i++) {
            int target = random_int() % num_samples + p * num_samples;
            double temp = samples[i + p * num_samples + 1].y;
            samples[i + p * num_samples + 1].y = samples[target].y;
            samples[target].y = temp;
        }
    }
}

// sets up randomly shuffled indices for the samples array

void Sampler::setup_shuffled_indices() {
    shuffled_indices.reserve(num_samples * num_sets);
    std::vector<int> indices;
    std::random_device rd;
    std::mt19937 g(rd());

    for (int j = 0; j < num_samples; j++) {
        indices.push_back(j);
    }

    for (int p = 0; p < num_sets; p++) {
        std::shuffle(indices.begin(), indices.end(), g);

        for (int j = 0; j < num_samples; j++) {
            shuffled_indices.push_back(indices[j]);
        }
    }
}

// Maps the 2D sample points in the square [-1,1] X [-1,1] to a unit disk, using Peter Shirley's
// concentric map function

void Sampler::map_samples_to_unit_disk() {
    size_t size = samples.size();
    double r, phi;  // polar coordinates
    Point2 sp;    // sample point on unit disk

    disk_samples.reserve(size);

    for (int j = 0; j < size; j++) {
        // map sample point to [-1, 1] X [-1,1]

        sp.x = 2.0f * samples[j].x - 1.0f;
        sp.y = 2.0f * samples[j].y - 1.0f;

        if (sp.x > -sp.y) {     // sectors 1 and 2
            if (sp.x > sp.y) {  // sector 1
                r = sp.x;
                phi = sp.y / sp.x;
            } else {  // sector 2
                r = sp.y;
                phi = 2 - sp.x / sp.y;
            }
        } else {                // sectors 3 and 4
            if (sp.x < sp.y) {  // sector 3
                r = -sp.x;
                phi = 4 + sp.y / sp.x;
            } else {  // sector 4
                r = -sp.y;
                if (sp.y != 0.0f) {  // avoid division by zero at origin
                    phi = 6 - sp.x / sp.y;
                } else {
                    phi = 0.0f;
                }
            }
        }

        phi *= pi / 4.0f;

        disk_samples[j].x = r * (double)cos(phi);
        disk_samples[j].y = r * (double)sin(phi);
    }

    samples.erase(samples.begin(), samples.end());
}

// Maps the 2D sample points to 3D points on a unit hemisphere with a cosine power
// density distribution in the polar angle

void Sampler::map_samples_to_hemisphere(const double exp) {
    size_t size = samples.size();
    hemisphere_samples.reserve(num_samples * num_sets);

    for (int j = 0; j < size; j++) {
        double cos_phi = cos(2.0 * pi * samples[j].x);
        double sin_phi = sin(2.0 * pi * samples[j].x);
        double cos_theta = pow((1.0f - samples[j].y), 1.0f / (exp + 1.0f));
        double sin_theta = sqrt(1.0f - cos_theta * cos_theta);
        double pu = sin_theta * cos_phi;
        double pv = sin_theta * sin_phi;
        double pw = cos_theta;
        hemisphere_samples.push_back(Point3(pu, pv, pw));
    }
}

// Maps the 2D sample points to 3D points on a unit sphere with a uniform density
// distribution over the surface
// this is used for modelling a spherical light

void Sampler::map_samples_to_sphere() {
    sphere_samples.reserve(num_samples * num_sets);

    for (int j = 0; j < num_samples * num_sets; j++) {
        double r1 = samples[j].x;
        double r2 = samples[j].y;
        double z = 1.0f - 2.0f * r1;
        double r = sqrt(1.0f - z * z);
        double phi = 2 * pi * r2;
        double x = r * cos(phi);
        double y = r * sin(phi);
        sphere_samples.push_back(Point3(x, y, z));
    }
}

// the final version in Listing 5.13

Point2 Sampler::sample_unit_square() {
    if (count % num_samples == 0) {                      // start of a new pixel
        jump = (random_int() % num_sets) * num_samples;  // random index jump initialised to zero in constructor
    }
    int index = jump + shuffled_indices[jump + count++ % num_samples];
    return samples[index];
}

Point2 Sampler::sample_unit_disk() {
    if (count % num_samples == 0) {  // start of a new pixel
        jump = (random_int() % num_sets) * num_samples;
    }

    return disk_samples[jump + shuffled_indices[jump + count++ % num_samples]];
}

Point3 Sampler::sample_hemisphere() {
    if (count % num_samples == 0) {  // start of a new pixel
        jump = (random_int() % num_sets) * num_samples;
    }

    return hemisphere_samples[jump + shuffled_indices[jump + count++ % num_samples]];
}

Point3 Sampler::sample_sphere() {
    if (count % num_samples == 0) {  // start of a new pixel
        jump = (random_int() % num_sets) * num_samples;
    }

    return sphere_samples[jump + shuffled_indices[jump + count++ % num_samples]];
}

// This is a specialised function called in LatticeNoise::init_vector_table
// It doesn't shuffle the indices

Point2 Sampler::sample_one_set() { return samples[count++ % num_samples]; }
