#include <iostream>
#include "World.h"

// Singleton lets us create only one instance of a class.
// It's globally available which makes it bad.
// It encourages coupling.

int main()
{
	auto wrld = World::GetInstance();
	auto wrld2 = World::GetInstance();

	std::cout << World::GetInstance()->GetCounter() << std::endl;
}
