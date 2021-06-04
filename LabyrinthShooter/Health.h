#pragma once
#include "ObjectComponent.h"
#include <list>

using std::function;

class Health :
	public ObjectComponent
{
public:
	Health(GameObject& owner, int maxHealth);

	void Hurt(int hp);

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(function<void(Health*)> handler);

	virtual ObjectComponent* Copy(GameObject& newOwner) override;

private:
	int maxHealth;
	int currHealth;

	std::list<function<void(Health*)>> onDeath;

private:
	void OnDeath();
};

