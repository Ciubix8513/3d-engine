#ifndef _SKYBOX_H_
#define _SKYBOX_H_
#include "MaterialComponent.h"
#include "CameraComponent.h"
using namespace Engine;
using namespace EngineMath;


namespace Engine
{
	class Skybox : public Component
	{
		void Initialise(std::vector<Component*>, D3d** d3d, ULONG EntityUUID) override;
		void Shutdown() override;
		void Update() override;		
		std::vector< const type_info*> GetRequieredComponents() override;

		


	private:
		CameraComponent* m_camPtr;
		MaterialComponent* m_matPtr;
		Texture* m_a;
		Texture* m_b;
		Texture* m_c;
		Texture* m_d;
		Texture* m_e;
		Texture* m_f;
	};
};
#endif

