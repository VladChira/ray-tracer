#pragma once
#include <mutex>
#include "console.h"
#include "gui.h"
#include <iostream>
#include <string.h>
class RenderView
{
private:
    static RenderView *render_view_instance;
    static std::mutex singleton_mutex;

protected:
    RenderView() {}
    ~RenderView() {}

public:
    GLuint FBO;        // frame buffer object
    GLuint texture_id; // the texture id we'll need later to create a texture
                       // out of our framebuffer

    int width, height;
    uint8_t *pixel_data;

    bool init = false;
    bool display_render = false;

    RenderView(RenderView &other) = delete;
    void operator=(const RenderView &) = delete;

    static RenderView *GetInstance();

    static void DestroyInstance();

    void set_size(int w, int h)
    {
        width = w;
        height = h;
        pixel_data = (uint8_t *)calloc(width * height * 3, sizeof(uint8_t));
        init = true;
    }

    void create_framebuffer()
    {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
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
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

RenderView *RenderView::render_view_instance{nullptr};
std::mutex RenderView::singleton_mutex;

RenderView *RenderView::GetInstance()
{
    std::lock_guard<std::mutex> lock(singleton_mutex);
    if (render_view_instance == nullptr)
    {
        render_view_instance = new RenderView();
    }
    return render_view_instance;
}

void RenderView::DestroyInstance()
{
    delete render_view_instance;
    render_view_instance = NULL;
}