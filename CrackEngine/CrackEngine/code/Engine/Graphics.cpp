#include "Graphics.h"
namespace Crack
{
	void Graphics::RenderLists()
	{
		for (std::vector<GameObject*>::iterator it = BackgroundLayer.begin(); it != BackgroundLayer.end(); ++it)
		{
			if ((*it) != nullptr)
			(*it)->render();
		}
		for (std::vector<GameObject*>::iterator it = GameObjectLayer.begin(); it != GameObjectLayer.end(); ++it)
		{
			if ((*it) != nullptr)
			(*it)->render();
		}
		for (std::vector<GameObject*>::iterator it = UILayer.begin(); it != UILayer.end(); ++it)
		{
			if ((*it) != nullptr)
			(*it)->render();
		}
	}
	void Graphics::AddToGameObject(GameObject* obj)
	{
		GameObjectLayer.push_back(obj);
	}
	void Graphics::AddToBackground(GameObject* obj)
	{
		BackgroundLayer.push_back(obj);
	}
	void Graphics::AddToUI(GameObject* obj)
	{
		UILayer.push_back(obj);
	}
}