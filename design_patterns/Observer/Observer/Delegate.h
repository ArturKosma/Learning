#pragma once

#include <vector>
#include <algorithm>

#ifndef NDEBUG
#include <iostream>
#define AK_Assert(InExpr, InMsg) \
			__AK_Assert(#InExpr, InExpr, InMsg, __LINE__, __FILE__)

bool __AK_Assert(const char* InExprStr, bool bInExpr, const char* InMsg, int InLine, const char* InFile)
{
	if (!bInExpr)
	{
		std::cerr << "Assert failed: " << InExprStr << std::endl
			<< "Message: " << InMsg << std::endl
			<< "File: " << InFile << " (Line: " << InLine << ")" << std::endl;
		abort();
	}

	return false;
}
#else
#define AK_Assert(InExpr, InMsg) !(InExpr)
#endif

using UID = unsigned long long int;

// Base observer class. All constructed observers are derived from this class.
// They are being constructed depending on the input parameters like whether
// it's a lambda function or a member method.
template<typename RetType, typename... Params>
class Observer
{
public:

	virtual bool operator==(Observer<RetType, Params...>* InOther) const = 0;
	virtual bool IsValid() const = 0;
	virtual void Invoke(Params...) = 0;
};

// Observer of the static function type.
template<typename RetType, typename... Params>
class Observer_Fun : public Observer<RetType, Params...>
{
public:

	Observer_Fun(RetType(*InFunPtr)(Params...)) : FunPtr(InFunPtr)
	{
	}

	virtual bool operator==(Observer<RetType, Params...>* InOther) const override final
	{
		auto casted = dynamic_cast<Observer_Fun<RetType, Params...>*>(InOther);
		return casted && (FunPtr == *(casted->FunPtr));
	}

	virtual bool IsValid() const override final
	{
		return FunPtr != nullptr;
	}

	virtual void Invoke(Params... InParams) override final
	{
		FunPtr(InParams...);
	}

	RetType(*FunPtr)(Params...);
};

// Observer of the lambda/functor type.
template<typename LambdaFunctorType, typename RetType, typename... Params>
class Observer_LambdaFunctor : public Observer<RetType, Params...>
{
public:

	Observer_LambdaFunctor(LambdaFunctorType& InLambdaFunctor) : Callable(&InLambdaFunctor)
	{
	}

	virtual bool operator==(Observer<RetType, Params...>* InOther) const override final
	{
		auto casted = dynamic_cast<Observer_LambdaFunctor<LambdaFunctorType, RetType, Params...>*>(InOther);
		return casted && (Callable == casted->Callable);
	}

	virtual bool IsValid() const override final
	{
		return Callable != nullptr;
	}

	virtual void Invoke(Params... InParams) override final
	{
		(*Callable)(InParams...);
	}

	LambdaFunctorType* Callable;
};

// Observer of the member method type.
// Obj is a pointer to a pointer so that we pass the original pointer address
// somehow simulating how would it work in a bigger application like UE4
// so that we could =nullptr just like .Destroy() in UE4.
template<typename ObjType, typename RetType, typename... Params>
class Observer_Met : public Observer<RetType, Params...>
{
public:

	Observer_Met(ObjType*& InObj, RetType(ObjType::* InMetPtr)(Params...)) : Obj(&InObj), MetPtr(InMetPtr)
	{
	}

	virtual bool operator==(Observer<RetType, Params...>* InOther) const override final
	{
		auto casted = dynamic_cast<Observer_Met<ObjType, RetType, Params...>*>(InOther);
		return casted && (MetPtr == casted->MetPtr) && (*Obj == *casted->Obj);
	}

	virtual bool IsValid() const override final
	{
		return *Obj != nullptr && MetPtr != nullptr;
	}

	virtual void Invoke(Params... InParams) override final
	{
		(*Obj->*MetPtr)(InParams...);
	}

	ObjType** Obj;
	RetType(ObjType:: * MetPtr)(Params...);
};

// A static factory class which constructs a new observer based on the input parameters.
template<typename RetType, typename... Params>
class ObserverFactory
{
public:

	static Observer<RetType, Params...>* CreateObserver(RetType(*InFunPtr)(Params...))
	{
		return new Observer_Fun<RetType, Params...>(InFunPtr);
	}

	template<typename LambdaFunctorType>
	static Observer<RetType, Params...>* CreateObserver(LambdaFunctorType& InLambdaFunctor)
	{
		return new Observer_LambdaFunctor<LambdaFunctorType, RetType, Params...>(InLambdaFunctor);
	}

	template<typename ObjType>
	static Observer<RetType, Params...>* CreateObserver(ObjType*& InObj, RetType(ObjType::* InMetPtr)(Params...))
	{
		return new Observer_Met<ObjType, RetType, Params...>(InObj, InMetPtr);
	}
};

// Base delegate class.
// Contains whole invocation list and broadcasts to it.
template<typename RetType, typename... Params>
class Delegate
{

public:

	// Simple handle for a delegate.
	// Handle's default ID is 0. Can only receive a proper ID via copy-constructor
	// with the result of Delegate::Add() as input.
	// Has some useful functionality like comparing or invalidating handles.
	class DelegateHandle
	{

	public:

		DelegateHandle() : ID(0)
		{
			
		}

		bool operator==(const DelegateHandle& InOtherDelegate)
		{
			return ID == InOtherDelegate.ID;
		}

		unsigned long long GetID() const
		{
			return ID;
		}

		bool IsValid() const
		{
			return ID != 0;
		}

		void Invalidate()
		{
			ID = 0;
		}

		template<typename RetType, typename... Params>
		friend class Delegate;

	private:

		UID ID;
	};

	// Adds a new observer to the invocation list, if it's unique. Assertion otherwise.
	template<typename ObjType, typename... FactoryParams>
	DelegateHandle* Add(ObjType&& InObj, FactoryParams... InParams)
	{
		// Creates the observer object and returns a heap pointer to it.
		Observer<RetType, Params...>* newObserver = ObserverFactory<RetType, Params...>::CreateObserver(InObj, InParams...);

		auto it = std::find_if(InvocationList.begin(), InvocationList.end(),
			[&newObserver](auto pair) {return *(pair.first) == newObserver; });

		// Asserts if the observer is already added.
		// It won't assert on Release compilation mode, so we delete from the heap instead.
		if (AK_Assert(it == InvocationList.end(), "Observers have to be unique!"))
		{
			delete newObserver;
			return nullptr;
		}

		// Creates a new delegate handle.
		// We assume this new handle is valid so we give it a value of 1,
		// so that it won't be deleted immidiately in Compact().
		DelegateHandle* newHandle = new DelegateHandle();
		newHandle->ID = 1;

		InvocationList.emplace_back(std::make_pair(newObserver, newHandle));

		Compact();

		return newHandle;
	}

	// Invalidates DelegateHandle pointed by InHandle by setting it's ID to 0.
	// Both handle and the delegate get immidiately removed from the heap in Compact().
	// Using pointers to deleted handles without setting a new value for them is unspecified behavior.
	void Remove(DelegateHandle* InHandle)
	{
		if (InHandle == nullptr)
		{
			return;
		}

		auto it = std::find_if(InvocationList.begin(), InvocationList.end(),
			[&InHandle](auto pair) {return pair.second->GetID() == InHandle->GetID(); });

		if (it != InvocationList.end())
		{
			it->second->Invalidate();
			Compact();
		}
	}

	// Simply removes all elements in the InvocationList, invalidates the handles first.
	// Using pointers to deleted handles without setting a new value for them is unspecified behavior.
	void RemoveAll()
	{
		std::for_each(InvocationList.begin(), InvocationList.end(),
			[](auto pair) {pair.second->Invalidate(); });

		InvocationList.erase(std::remove_if(InvocationList.begin(), InvocationList.end(),
			[](auto pair) {return true; }), InvocationList.end());
	}

	// Broadcasts to every element in the invocation list.
	// If the observer object is invalid - will assert on debug mode.
	// Use IsValid() to make sure your observer object has correct state.
	void Broadcast(Params... InParams)
	{
		for (auto it = InvocationList.begin(); it != InvocationList.end(); ++it)
		{
			AK_Assert(it->first->IsValid(),
				"The observer object is invalid. Please use Compact() if you change the state of observers after adding to the invocation list.");

			if (it->second->IsValid())
			{
				it->first->Invoke(InParams...);
			}
		}
	}

	// Deletes any invalid pair in the invocation list.
	// Refreshes IDs for the handles.
	void Compact()
	{
		InvocationList.erase(std::remove_if(InvocationList.begin(), InvocationList.end(),
			[](auto pair) {return !pair.first->IsValid() || !pair.second->IsValid(); }), InvocationList.end());

		UID newID = 1;

		std::for_each(InvocationList.begin(), InvocationList.end(),
			[&newID](auto pair) {pair.second->ID = newID++;});
	}

	// Checks if observer object paired with InHandle parameter is valid.
	// In UE4 we would check if the object has bPendingToKill == true;
	bool IsValid(DelegateHandle* InHandle) const
	{
		if (InHandle == nullptr)
		{
			return false;
		}

		auto it = std::find_if(InvocationList.begin(), InvocationList.end(),
			[&InHandle](auto pair) {return pair.second->GetID() == InHandle->GetID(); });

		if (it != InvocationList.end())
		{
			return it->first->IsValid();
		}

		return false;
	}

private:

	using ObserverDelegatePair = std::pair<Observer<RetType, Params...>*, DelegateHandle*>;
	std::vector<ObserverDelegatePair> InvocationList;
};