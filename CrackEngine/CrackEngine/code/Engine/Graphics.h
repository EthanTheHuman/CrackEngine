#pragma once
#include <vector>
#include "../GameObject.h"

namespace Crack {
	class Graphics
	{
	public:
		void RenderLists();
		void AddToGameObject(GameObject* obj);
		void AddToBackground(GameObject* obj);
		void AddToUI(GameObject* obj);
	private:
		std::vector<GameObject*> GameObjectLayer;
		std::vector<GameObject*> BackgroundLayer;
		std::vector<GameObject*> UILayer;
	};
}