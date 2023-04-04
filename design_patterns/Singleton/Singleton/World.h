#pragma once
class World
{
public:

	~World()
	{

	}

	static World* GetInstance()
	{
		// Local static variables are initialized only once, even in recurrency.
		static World* WorldInst = new World();
		return WorldInst;
	}

	unsigned GetCounter() const
	{
		return Counter;
	}

private:

	World()
	{
		Counter++;
	}

	static unsigned Counter;
};
