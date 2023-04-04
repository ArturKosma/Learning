#include "Delegate.h"
#include <iostream>

class myclass
{
public:

	myclass()
	{
		
	}

	int somefun(int, double)
	{
		std::cout << "i work" << std::endl;
		return 10;
	}
};

int fun(int, double)
{
	std::cout << "static fun" << std::endl;
	return 666;
}

int main()
{
	Delegate<int, int, double> mydel;

	myclass* mc = new myclass();

	Delegate<int,int,double>::DelegateHandle* myhandle = mydel.Add(mc, &myclass::somefun);

	mc = nullptr; // In a bigger application making this a nullptr should allow garbage collector to delete pointed object from the object pool.
	// Here we simulate UE4's Destroy().
	// In a small application like this, the broadcast might success even after calling destructor on that object, but it's undefined behavior.
	// IsValid() should check if some object is pending to be deleted. I didn't implement such thing for a small application like this.
	// Debug mode will assert though unless we Compact() which gets rid of all invalid call objects.

	//mydel.Compact();

	if (mydel.IsValid(myhandle))
	{
		std::cout << "seems ok" << std::endl;
	}

	//mydel.Add(&fun);

	mydel.Broadcast(345, 44.5);

	getchar();
}
