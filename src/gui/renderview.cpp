#include "renderview.h"

RenderView *RenderView::render_view_instance{nullptr};
std::mutex RenderView::singleton_mutex;

RenderView::RenderView() {}

RenderView::~RenderView() {}


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
    delete render_view_instance->image;
    delete render_view_instance;
    render_view_instance = NULL;
}