#pragma once

#define AFCLASS(Class, ClassStringName)
#define AFPARAM(Type, VarName, VarString, Direction) \
	Type VarName

/*struct FAFStaticGraphRegister

class AFStaticGraphRegister
{
	static void Add
};*/

class AFGraphNode
{
public:
	virtual ~AFGraphNode();

	virtual void Evaluate(float deltaTime) = 0;
};