#pragma once
#include <memory>
#include <vector>

class AFEvaluator
{
public:
	static AFEvaluator& Get()
	{
		static AFEvaluator evaluator;
		return evaluator;
	}

	void EvaluateNode(std::shared_ptr<class AFGraphNode> node);

	void ClearEvaluationState();

private:

	std::vector<std::shared_ptr<class AFGraphNode>> m_evaluated = {};
};
