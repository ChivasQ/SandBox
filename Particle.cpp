#include"Particle.h"

Particle::Particle(unsigned int type, Color3 color, bool isLiquid)
{
	this->color = color;
	this->type = type;
	this->isLiquid = isLiquid;
}
Particle::Particle() = default;

int Particle::getType()
{
	return this->type;
}

Color3 Particle::getColor()
{
	return this->color;
}

void Particle::setColor(Color3 color)
{
	this->color = color;
}



