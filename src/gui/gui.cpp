#include <atomic>
#include "gui.h"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void ShowFileOptions()
{
    if (ImGui::MenuItem("Open sample scene", "Ctrl+O"))
    {
    }
    if (ImGui::MenuItem("Exit", "Alt-F4"))
    {
    }
}

void ShowEditOptions()
{
    if (ImGui::MenuItem("Render settings", "Ctrl+R"))
    {
    }
}

void ShowAssetsOptions()
{
    if (ImGui::MenuItem("View all assets"))
    {
    }
    if (ImGui::MenuItem("Disable all textures from scene"))
    {
    }
}

void ShowWindowOptions()
{
    if (ImGui::MenuItem("TODO"))
    {
    }
}

void ShowHelpOptions()
{
    if (ImGui::MenuItem("There is no help"))
    {
    }
}

void ConsoleLogWindow()
{
    const float bottom_space = ImGui::GetStyle().ItemSpacing.y;
    if (ImGui::BeginChild("ScrollRegion##", ImVec2(0, -bottom_space), false, 0))
    {
        // Wrap items.
        ImGui::PushTextWrapPos();

        std::vector<ConsoleLine> console_entries = Console::GetInstance()->getContents();

        for (int i = 0; i < console_entries.size(); i++)
        {
            ConsoleLine line = console_entries[i];
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            if (line.entry_type == SuccesEntry)
                color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            if (line.entry_type == LogEntry)
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            if (line.entry_type == WarningEntry)
                color = ImVec4(1.0f, 0.75f, 0.0f, 1.0f);
            if (line.entry_type == ErrorEntry)
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, color); 
            ImGui::TextUnformatted(line.text.c_str());  
            ImGui::PopStyleColor();
        }

        // Auto-scroll logs.
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
}

GUI::GUI()
    {
        if (!glfwInit())
            return;

        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // Create window with graphics context
        window = glfwCreateWindow(1280, 720, "Ray Tracer", nullptr, nullptr);
        if (window == nullptr)
            return;
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        if (glewInit() != GLEW_OK)
        {
            std::cout << "glew initialisation failed!\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

extern std::atomic<bool> exit_requested;
GUI::~GUI()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    // Console::DestroyInstance();
}

void GUI::init() {}

void GUI::run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        update();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Signal the rendering threads and other stuff to exit gracefully
    exit_requested.store(true, std::memory_order_relaxed);
}

void GUI::update()
{
    static bool show_file_options = false;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();

    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", NULL, ImGuiDir_Up, height, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ShowFileOptions();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ShowEditOptions();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Assets"))
            {
                ShowAssetsOptions();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ShowWindowOptions();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                ShowHelpOptions();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();

    // set the render window size such that it fits in 75% of the frame
    // with a 16:9 aspect ratio
    double window_width = ImGui::GetIO().DisplaySize.x;
    double window_height = ImGui::GetIO().DisplaySize.y;
    ImGui::SetNextWindowPos(ImVec2(1, 20));
    ImGui::SetNextWindowSize(ImVec2(1.77 * 0.75 * window_height, 0.75 * window_height));
    if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        auto instance = RenderView::GetInstance();
        if (instance->init)
        {
            instance->create_framebuffer();
            instance->init = false;
        }
        if (instance->display_render)
        {
            ImGui::Text("Render progress");
            ImGui::ProgressBar(instance->get_progress(), ImVec2(-FLT_MIN, 0.0f));

            if (!instance->finished)
                instance->update_framebuffer();
            ImGui::Image((void *)(intptr_t)instance->texture_id, ImVec2(instance->image->get_width(), instance->image->get_height()));
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1, window_height - 0.22 * window_height));
    ImGui::SetNextWindowSize(ImVec2(1.77 * 0.75 * window_height, 0.2 * window_height));
    if (ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        ConsoleLogWindow();
        // ImGui::Separator();
        // TODO Add input field?
    }
    ImGui::End();

    if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, height, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::Text("Ray Tracer");
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}