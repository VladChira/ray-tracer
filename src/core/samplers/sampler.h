#pragma once
#include "maths.h"

namespace raytracer
{
    class Sampler
    {
    public:
        Sampler();

        Sampler(const int num);

        Sampler(const int num, const int num_sets);

        virtual ~Sampler();

        Sampler(const Sampler &s);

        Sampler(Sampler &&s) noexcept;

        Sampler &operator=(const Sampler &s);

        Sampler &operator=(Sampler &&s) noexcept;

        virtual Sampler *clone() const = 0;

        void set_num_sets(const int np);

        virtual void // generate sample patterns in a unit square
        generate_samples() = 0;

        int get_num_samples();

        void shuffle_x_coordinates();

        void shuffle_y_coordinates();

        void setup_shuffled_indices();

        void map_samples_to_unit_disk();

        void map_samples_to_hemisphere(const float p);

        void map_samples_to_sphere();

        // the following functions are not const because they change count and jump

        Point2 // get next sample on unit square
        sample_unit_square();

        Point2 // get next sample on unit disk
        sample_unit_disk();

        Point3 // get next sample on unit hemisphere
        sample_hemisphere();

        Point3 // get next sample on unit sphere
        sample_sphere();

        Point2           // only used to set up a vector noise table
        sample_one_set(); // this is not discussed in the book, but see the
                          // file LatticeNoise.cpp in Chapter 31

    protected:
        int num_samples;                         // the number of sample points in a set
        int num_sets;                            // the number of sample sets
        std::vector<Point2> samples;            // sample points on a unit square
        std::vector<int> shuffled_indices;       // shuffled samples array indices
        std::vector<Point2> disk_samples;       // sample points on a unit disk
        std::vector<Point3> hemisphere_samples; // sample points on a unit hemisphere
        std::vector<Point3> sphere_samples;     // sample points on a unit sphere
        unsigned long count;                     // the current number of sample points used
        int jump;                                // random index jump
    };

}