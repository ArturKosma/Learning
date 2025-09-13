#include "AFDeltaObject.h"

AFDeltaObject& AFDeltaObject::Get()
{
	static AFDeltaObject deltaObject;
	return deltaObject;
}

void AFDeltaObject::ResetValue(const std::string& name)
{
	m_values[name] = 0.0f;
	m_deltas[name] = 0.0f;
}

float AFDeltaObject::SetValue(const std::string& name, float value)
{
	auto it = m_values.find(name);
	if (it == m_values.end())
	{
		m_values[name] = value;
		m_deltas[name] = value;
		return value;
	}

	const float oldValue = it->second;
	const float delta = value - oldValue;
	m_values[name] = value;
	m_deltas[name] = delta;

	return delta;
}

float AFDeltaObject::GetDelta(const std::string& name) const
{
	auto it = m_deltas.find(name);
	if (it == m_deltas.end())
	{
		return 0.0f;
	}

	return it->second;
}

AFDeltaObject::AFDeltaObject()
{

}
