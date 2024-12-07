#include "ShoppingScene.h"

ShoppingScene::ShoppingScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, InputManager* _pInpueManager, WorldManager* _worldManager
	, UIManager* _UIManager, EntityManager* _pEntityManager
	, ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	:Scene(_registry, _name, _pEventManager, _pRenderManager, _pInpueManager, _worldManager)
{
	m_pUIManager = _UIManager;
	m_pSoundManager = _pSoundManager;
	m_pEntityManager = _pEntityManager;
}

bool ShoppingScene::Initialize()
{
	return true;
}

void ShoppingScene::FixedUpdate(float _dTime)
{

}

void ShoppingScene::Update(float _dTime)
{

}

void ShoppingScene::LateUpdate(float _dTime)
{

}

void ShoppingScene::Finalize()
{

}

void ShoppingScene::AddResource()
{

}
