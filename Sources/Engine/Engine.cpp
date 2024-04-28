#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.hpp"

Engine::~Engine()
{
    Release();
}

void Engine::Initialize(const EngineInfo &info)
{
    if (initialized)
        Release();

    switch (info.renderingEngineInfo->renderingEngineType)
    {
    case RASTERIZATION:
        renderingEngine = new RasterizationEngine();
        break;
    case RAY_TRACING_CPU:
        throw runtime_error("Ray Tracing CPU not implemented");
        break;
    case RAY_TRACING_GPU:
        throw runtime_error("Ray Tracing GPU not implemented");
        break;
    }

    if (!renderingEngine)
        throw bad_alloc();
    renderingEngine->Initialize(*info.renderingEngineInfo);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(renderingEngine->GetRenderer()->GetWindow()->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    initialized = true;
    cout << "Engine initialize" << endl;
}

void Engine::Release()
{
    if (initialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if (renderingEngine)
        {
            renderingEngine->Release();
            delete renderingEngine;
            renderingEngine = nullptr;
        }

        initialized = false;
        cout << "Engine release" << endl;
    }
}

void Engine::Run()
{
    if (!initialized)
        throw runtime_error("Engine not initialized");

    GLFWwindow *glfwWindow = renderingEngine->GetRenderer()->GetWindow()->GetHandle();

    double lag = 0.0;
    double previous = glfwGetTime();
    double SECONDS_PER_UPDATE = 1.0 / 60.0;

    while (!glfwWindowShouldClose(glfwWindow))
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previous;
        previous = currentTime;
        lag += elapsedTime;

        glfwPollEvents();

        while (lag >= SECONDS_PER_UPDATE)
        {
            // Game Logic
            lag -= SECONDS_PER_UPDATE;
        }

        renderingEngine->Render(static_cast<float>(lag / SECONDS_PER_UPDATE));
    }
}
