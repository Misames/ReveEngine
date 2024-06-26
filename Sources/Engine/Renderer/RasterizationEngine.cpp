#include "OpenGL/OpenGLRenderer.hpp"

#include "RasterizationEngine.hpp"

RasterizationEngine::~RasterizationEngine()
{
    Release();
}

void RasterizationEngine::Initialize(const RenderingEngineInfo &renderingEngineInfo)
{
    if (initialized)
        Release();

    switch (renderingEngineInfo.rendererInfo->rendererType)
    {
    case OPENGL:
        renderer = new OpenGLRenderer();
        break;
    case VULKAN:
        throw runtime_error("Vulkan renderer not implemented");
        break;
    case DIRECTX:
        throw runtime_error("DirectX renderer not implemented");
        break;
    }

    if (!renderer)
        throw bad_alloc();
    renderer->Initialize(*renderingEngineInfo.rendererInfo);

    scene = new Scene();
    if (!scene)
        throw bad_alloc();
    scene->Initialize();

    glfwSetWindowTitle(renderer->GetWindow()->GetHandle(), scene->name);

    initialized = true;
    cout << "RasterizationEngine initialize" << endl;
}

void RasterizationEngine::Release()
{
    if (initialized)
    {
        if (renderer)
        {
            renderer->Release();
            delete renderer;
            renderer = nullptr;
        }

        if (scene)
        {
            scene->Release();
            delete scene;
            scene = nullptr;
        }

        initialized = false;
        cout << "RasterizationEngine release" << endl;
    }
}

void RasterizationEngine::Render(float lag)
{
    GLFWwindow *glfwWindow = renderer->GetWindow()->GetHandle();
    scene->RenderScene(glfwWindow);
}