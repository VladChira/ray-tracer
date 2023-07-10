#include "multijittered.h"

using namespace raytracer;

MultiJittered::MultiJittered(const int num_samples) : Sampler(num_samples) { generate_samples(); }

MultiJittered::MultiJittered(const int num_samples, const int m) : Sampler(num_samples, m) { generate_samples(); }

MultiJittered* MultiJittered::clone() const { return new MultiJittered(*this); }

void MultiJittered::generate_samples() {

    int n = (int)sqrt((double)num_samples);
    double subcell_width = 1.0f / ((double)num_samples);

    // fill the samples array with dummy points to allow us to use the [ ] notation when we set the
    // initial patterns

    Point2 fill_point;
    for (int j = 0; j < num_samples * num_sets; j++) {
        samples.push_back(fill_point);
    }

    // distribute points in the initial patterns

    for (int p = 0; p < num_sets; p++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                samples[i * n + j + p * num_samples].x = (i * n + j) * subcell_width + random_double(0, subcell_width);
                samples[i * n + j + p * num_samples].y = (j * n + i) * subcell_width + random_double(0, subcell_width);
            }
        }
    }

    // shuffle x coordinates

    for (int p = 0; p < num_sets; p++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int k = random_uint(j, n - 1);
                double t = samples[i * n + j + p * num_samples].x;
                samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
                samples[i * n + k + p * num_samples].x = t;
            }
        }
    }

    // shuffle y coordinates

    for (int p = 0; p < num_sets; p++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int k = random_uint(j, n - 1);
                double t = samples[j * n + i + p * num_samples].y;
                samples[j * n + i + p * num_samples].y = samples[k * n + i + p * num_samples].y;
                samples[k * n + i + p * num_samples].y = t;
            }
        }
    }
}
