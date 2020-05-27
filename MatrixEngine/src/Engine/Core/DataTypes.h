#pragma once
#include <memory>

#define ENGINE_UNUSED(p) ((void)(p))

template <typename T>
using sPtr = std::shared_ptr<T>;

template <typename T>
using uPtr = std::unique_ptr<T>;


class Component;
class GameObject;
class ComponentManager;
using ComponentManagerPtr = uPtr<ComponentManager>;
using ComponentPtr = uPtr<Component>;
using GameObjectPtr = uPtr<GameObject>;


typedef char				s8;
typedef unsigned char		u8;
typedef signed short		s16;
typedef unsigned short		u16;
typedef signed int			s32;
typedef unsigned int		u32;
typedef signed long long	s64;
typedef unsigned long long	u64;
typedef float				f32;
typedef double				f64;