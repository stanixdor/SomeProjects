//#include "ComponentManager.h"
//#include "GameObject.h"
//
//
//ComponentManagerPtr ComponentManager::s_instance = nullptr;
//
//ComponentManager::ComponentManager(void)
//{
//
//}
//ComponentManager::~ComponentManager(void)
//{
//
//}
//void ComponentManager::Clear()
//{
//	m_components.clear();
//}
//void ComponentManager::Create()
//{
//	
//}
//void ComponentManager::Destroy()
//{
//	Clear();
//}
//Component* ComponentManager::AddComponent(Component* l_component)
//{
//	this->m_components.push_back(l_component);
//	int l_position = m_components.size() - 1;
//	return m_components[l_position];
//}
//
//void ComponentManager::RemoveComponentWithGameObject(GameObject* l_objects)
//{
//	for (component_vector_const_itr itr = this->m_components.begin(); itr != this->m_components.end();)
//	{
//		if ((*itr)->m_GameObject->m_guid == l_objects->m_guid)
//		{
//			itr = this->m_components.erase(itr);
//			--m_currentComponentCount;
//		}
//		else
//		{
//			++itr;
//		}
//	}
//}
//void ComponentManager::FixedUpdate()
//{
//	for (component_vector_itr itr = this->m_components.begin(); itr != this->m_components.end(); ++itr)
//	{
//		(*itr)->FixedUpdate();
//	}
//}
//void ComponentManager::Update()
//{
//	for (component_vector_itr itr = this->m_components.begin(); itr != this->m_components.end(); ++itr)
//	{
//		(*itr)->Update();
//	}
//}
//void ComponentManager::LateUpdate()
//{
//	for (component_vector_itr itr = this->m_components.begin(); itr != this->m_components.end(); ++itr)
//	{
//		(*itr)->LateUpdate();
//	}
//}
//
////void ComponentManager::Render()
////{
////	/*for (render_component_vector_itr itr = this->m_renderComponents.begin(); itr != this->m_renderComponents.end(); ++itr)
////	{
////		if ((*itr)->m_isEnabled)
////		{
////			(*itr)->Render();
////		}
////	}*/
////	for (component_vector_itr itr = this->m_components.begin(); itr != this->m_components.end(); ++itr)
////	{
////		if ((*itr)->m_isEnabled)
////		{
////			(*itr)->Render();
////		}
////	}
////}
