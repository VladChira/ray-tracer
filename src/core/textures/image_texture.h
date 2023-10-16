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

            Eigen::Vector2f mapped_point = mapping->map(rec.local_p, image->get_width(), image->get_height());
            
            if (mapped_point.x() > image->get_width() || mapped_point.y() > image->get_height() || mapped_point.x() < 0.0 || mapped_point.y() < 0.0)
                return Color::red;
        
            // TODO this conversion should definetely not be done here
            Color raw_color = image->get(mapped_point.x(), mapped_point.y()) / 255.0;
            Color texel_color = 0.012522878 * raw_color + 0.682171111 * raw_color * raw_color + 0.305306011 * raw_color * raw_color * raw_color;
            return texel_color;
        }
    private:
        std::unique_ptr<TextureMapping2D> mapping = nullptr;
        BufferedImage *image;
    };
}