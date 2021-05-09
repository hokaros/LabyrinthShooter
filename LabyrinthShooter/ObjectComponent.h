#pragma once
#include <SDL.h>
#include "IUpdateable.h"
#include "GameObject.h"

class ObjectComponent : public IUpdateable
{
public:
	ObjectComponent(GameObject& owner);
protected:
	GameObject& gameObject;
};

