#include "Skybox.h"

void Engine::Skybox::Initialise(std::vector<Component*> comp, D3d** d3d, ULONG EntityUUID)
{
    m_matPtr = (MaterialComponent*)comp[0];
    m_a = nullptr;
    m_b = nullptr;
    m_c = nullptr;
    m_d = nullptr;
    m_e = nullptr;
    m_f = nullptr;
}

void Engine::Skybox::Shutdown()
{

}

void Engine::Skybox::Update()
{
}

std::vector<const type_info*> Engine::Skybox::GetRequieredComponents()
{
    return {&typeid(Engine::MaterialComponent)};
}
