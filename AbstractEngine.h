#pragma once
class AbstractEngine;

class AbstractEntity {
public:
	virtual void react(AbstractEngine& ab)=0;
};

class Projectile;
class SnakeBase;
class Entity;
class DynamicEntity;

class AbstractEngine
{
public:
	virtual void react(Projectile* p)=0;
	virtual void react(SnakeBase* sb)=0;
	virtual void react(Entity* e)=0;
	virtual void react(DynamicEntity* de)=0;
};