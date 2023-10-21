#pragma once
#include "texture.h"
#include "texture_mapping2D.h"
#include "buffered_image.h"

namespace raytracer
{
    class ImageTexture : public Texture
    {
    public:
        ImageTexture(std::unique_ptr<TextureMapping2D> mapping, const std::string filename)
        {
            this->mapping = std::move(mapping);
            image = BufferedImage::load_image(filename);
        }

        Color evaluate(const HitInfo &rec) const override
        {
            assert(mapping != nullptr);

            Eigen::Vector2f mapped_point = mapping->map(rec);

            // fit these points to the texture
            Eigen::Vector2f texel(mapped_point.x() * (image->get_width() - 1), (1 - mapped_point.y()) * (image->get_height() - 1));

            if ((int)texel.x() >= image->get_width() || (int)texel.y() >= image->get_height() || (int)texel.x() < 0.0 || (int)texel.y() < 0.0)
                return Color::red;

            // TODO this conversion should definetely not be done here
            Color raw_color = image->get((int)texel.x(), (int)texel.y()) / 255.0;
            // std::cout << raw_color << "\n";    
            Color texel_color = 0.012522878 * raw_color + 0.682171111 * raw_color * raw_color + 0.305306011 * raw_color * raw_color * raw_color;
            return texel_color;
        }

    private:
        std::unique_ptr<TextureMapping2D> mapping = nullptr;
        BufferedImage *image;
    };
}