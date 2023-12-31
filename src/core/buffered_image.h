#pragma once
#include "spng.h"
#include "utilities.h"
#include "console.h"

namespace raytracer
{
    class BufferedImage
    {
    private:
        uint8_t *image_data = NULL;
        unsigned int width;
        unsigned int height;

    public:
        BufferedImage(unsigned int w, unsigned int h)
        {
            this->width = w;
            this->height = h;
            image_data = new uint8_t[w * h * 3]();
        }

        ~BufferedImage()
        {
            delete[] image_data;
        }

        BufferedImage(u_int8_t *bytes, unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            delete[] image_data;
            image_data = bytes;
        }

        unsigned int get_width() const
        {
            return width;
        }

        unsigned int get_height() const
        {
            return height;
        }

        raytracer::Color get(unsigned int x, unsigned int y)
        {
            if (x < width && y < height)
            {

                size_t position = (y * width + x) * 3;
                raytracer::Color color;
                color.r = image_data[position];
                color.g = image_data[position + 1];
                color.b = image_data[position + 2];
                return color;
            }
            else
            {
                std::cerr << "Invalid image lookup. Retuned red\n";
                return Color::red;
            }
        }

        void set(unsigned int x, unsigned int y, raytracer::Color color)
        {
            assert(x >= 0 && x < this->height && y >= 0 && y < this->width);

            size_t position = (x * width + y) * 3;
            image_data[position] = (uint8_t)color.r;
            image_data[position + 1] = (uint8_t)color.g;
            image_data[position + 2] = (uint8_t)color.b;
        }

        /* Returns the byte array [0, 255] of the 8-bit image */
        uint8_t *bytes(size_t &length)
        {
            length = 3 * this->width * this->height;
            return this->image_data;
        }

        uint8_t *bytes()
        {
            return this->image_data;
        }

        void linear_to_srgb()
        {
            for (int i = 0; i < this->height; i++)
                for (int j = 0; j < this->width; j++)
                {
                    raytracer::Color p = this->get(i, j) / 255;
                    p.r = clip((float)std::max(1.055 * pow(p.r, 0.416666667) - 0.055, 0.0), 0.0, 1.0) * 255;
                    p.g = clip((float)std::max(1.055 * pow(p.g, 0.416666667) - 0.055, 0.0), 0.0, 1.0) * 255;
                    p.b = clip((float)std::max(1.055 * pow(p.b, 0.416666667) - 0.055, 0.0), 0.0, 1.0) * 255;
                    this->set(i, j, p);
                }
        }

        void save_to_ppm(raytracer::BufferedImage img, FILE *file)
        {
            fprintf(file, "P3\n%u %u\n255\n", img.get_width(), img.get_height());
            for (int i = 0; i < img.get_height(); i++)
                for (int j = 0; j < img.get_width(); j++)
                {
                    raytracer::Color pixel_color = img.get(i, j);
                    fprintf(file, "%hhu %hhu %hhu\n", (uint8_t)pixel_color.r, (uint8_t)pixel_color.g, (uint8_t)pixel_color.b);
                }
            Console::GetInstance()->addSuccesEntry("PPM image exported succesfully");
        }

        raytracer::BufferedImage uv_test_image(int size)
        {
            raytracer::BufferedImage image(size, size);

            for (int j = size - 1; j >= 0; --j)
            {
                for (int i = 0; i < size; ++i)
                {
                    float r = float(i) / (size - 1);
                    float g = float(j) / (size - 1);
                    float b = 0.25;

                    uint8_t ir = static_cast<uint8_t>(256.0 * r);
                    uint8_t ig = static_cast<uint8_t>(256.0 * g);
                    uint8_t ib = static_cast<uint8_t>(256.0 * b);

                    image.set(i, j, raytracer::Color(ir, ig, ib));
                }
            }
            return image;
        }

        static int save_bytes_to_png(void *image, size_t length, uint32_t width, uint32_t height, FILE *output_file)
        {
            int fmt;
            int ret = 0;
            spng_ctx *ctx = NULL;
            struct spng_ihdr ihdr = {0};

            ctx = spng_ctx_new(SPNG_CTX_ENCODER);
            spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

            ihdr.width = width;
            ihdr.height = height;
            ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR;
            ihdr.bit_depth = 8;

            spng_set_ihdr(ctx, &ihdr);
            fmt = SPNG_FMT_PNG;

            ret = spng_encode_image(ctx, image, length, fmt, SPNG_ENCODE_FINALIZE);
            if (ret)
            {
                Console::GetInstance()->addErrorEntry("Image encoding failed.");
                printf("spng_encode_image() error: %s\n", spng_strerror(ret));
                spng_ctx_free(ctx);
                return ret;
            }

            size_t png_size;
            void *png_buf = NULL;

            /* Get the internal buffer of the finished PNG */
            png_buf = spng_get_png_buffer(ctx, &png_size, &ret);
            if (png_buf == NULL)
            {
                Console::GetInstance()->addErrorEntry("Image encoding failed.");
                printf("spng_get_png_buffer() error: %s\n", spng_strerror(ret));
            }
            else
            {
                /* Write the PNG data to the output file */
                if (output_file != NULL)
                    fwrite(png_buf, 1, png_size, output_file);
                else
                {
                    free(png_buf);
                    ret = -1;
                }

                /* User owns the buffer after a successful call */
                free(png_buf);
            }
            spng_ctx_free(ctx);

            return ret;
        }

        static int save_image_png(raytracer::BufferedImage &image, FILE *output_file)
        {
            uint32_t width = image.get_width();
            uint32_t height = image.get_height();

            size_t length = 0;
            uint8_t *image_data = image.bytes(length);
            int result = save_bytes_to_png(image_data, length, width, height, output_file);

            if (result != 0)
            {
                Console::GetInstance()->addErrorEntry("Image encoding failed.");
                printf("Encoding failed with error code: %d\n", result);
                return result;
            }
            return 0;
        }

        static const char *color_type_str(uint8_t color_type)
        {
            switch (color_type)
            {
            case SPNG_COLOR_TYPE_GRAYSCALE:
                return "grayscale";
            case SPNG_COLOR_TYPE_TRUECOLOR:
                return "truecolor";
            case SPNG_COLOR_TYPE_INDEXED:
                return "indexed color";
            case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA:
                return "grayscale with alpha";
            case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA:
                return "truecolor with alpha";
            default:
                return "(invalid)";
            }
        }

        static raytracer::BufferedImage *load_image(std::string filename)
        {
            // assert(filename != NULL);

            // Open the file
            FILE *file = fopen(filename.c_str(), "r"); // Victor says  :)

            assert(file != nullptr);

            int ret = 0;
            spng_ctx *ctx = NULL;
            unsigned char *image = NULL;

            ctx = spng_ctx_new(0);
            assert(ctx != NULL);

            /* Ignore and don't calculate chunk CRC's */
            spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

            /* Set memory usage limits for storing standard and unknown chunks,
               this is important when reading untrusted files! */
            size_t limit = 1024 * 1024 * 64;
            spng_set_chunk_limits(ctx, limit, limit);

            /* Set source PNG */
            ret = spng_set_png_file(ctx, file);
            assert(ret == 0);

            struct spng_ihdr ihdr;
            ret = spng_get_ihdr(ctx, &ihdr);
            assert(ret == 0);

            const char *color_name = color_type_str(ihdr.color_type);

            size_t image_size, image_width;
            int fmt = SPNG_FMT_RGB8;

            ret = spng_decoded_image_size(ctx, fmt, &image_size);
            assert(ret == 0);

            image = (unsigned char *)malloc(image_size);
            ret = spng_decode_image(ctx, image, image_size, fmt, 0);
            assert(ret == 0);

            spng_ctx_free(ctx);

            BufferedImage *img = new BufferedImage(image, ihdr.width, ihdr.height);
            return img;
        }
    };
}
