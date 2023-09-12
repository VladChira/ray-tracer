#include "pure_random.h"

using namespace raytracer;

PureRandom::PureRandom(const int num) : Sampler(num) { generate_samples(); }

PureRandom *PureRandom::clone() const { return new PureRandom(*this); }

void PureRandom::generate_samples()
{
    for (int p = 0; p < num_sets; p++)
    {
        for (int q = 0; q < num_samples; q++)
        {
            samples.push_back(Point2(random_double(), random_double()));
        }
    }
}