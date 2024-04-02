#ifndef PARTICLE_CLASS
#include"Color3.h"
#include<string>

class Particle
{
public:
	unsigned int type;
	Color3 color;
	bool isLiquid;
	Particle(unsigned int type, Color3 color, bool isLiquid);
	Particle();
	int getType();
	Color3 getColor();
	void setColor(Color3 color);
};


#endif // !PARTICLE_CLASS
