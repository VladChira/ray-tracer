#pragma once
#include <mutex>
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "console.h"
#include "buffered_image.h"

class RenderView
{
private:
    static RenderView *render_view_instance;
    static std::mutex singleton_mutex;

protected:
    RenderView();
    ~RenderView();

public:
    GLuint FBO;        // frame buffer object
    GLuint texture_id; // the texture id we'll need later to create a texture
                       // out of our framebuffer

    raytracer::BufferedImage *image;
    unsigned int pixel_count = 0; // used to track progress

    bool init = false;
    bool display_render = false;
    bool finished = false;

    RenderView(RenderView &other) = delete;
    void operator=(const RenderView &) = delete;

    static RenderView *GetInstance();

    static void DestroyInstance();

    void set_size(int w, int h)
    {
        image = new raytracer::BufferedImage(w, h);
        init = true;
    }

    void set_pixel_color(unsigned int x, unsigned int y, raytracer::Color color)
    {
        (*image).set(x, y, color);
        pixel_count++;
    }

    float get_progress()
    {
        return (pixel_count * 1.0) / (image->get_height() * image->get_width());
    }

    void create_framebuffer()
    {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->get_width(), image->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->bytes());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void update_framebuffer()
    {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->get_width(), image->get_height(), GL_RGB, GL_UNSIGNED_BYTE, image->bytes());
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
