#pragma once

#include <chrono>
#include <string>
#include <functional>

template<typename TRetType>
struct ExecutionTimeInfo
{
	long long ExecutionTime = -1;
	TRetType ReturnValue;

	std::string GetExecutionTimeAsString() const
	{
		std::string retStr;
		retStr += std::to_string(ExecutionTime);
		retStr += " microseconds.";

		return retStr;
	}
};

class Concurrency20Utility
{
public:

	template<typename TObjType, typename TMethodRetType, typename ...TMethodParamTypes, typename ...TMethodParams>
	static ExecutionTimeInfo<TMethodRetType> GetExecutionTime(TObjType* const InObj, TMethodRetType(*InMethodPtr)(TMethodParamTypes...), const TMethodParams&... InMethodParams)
	{
		std::function<TMethodRetType(TObjType&, TMethodParamTypes...)> fun = [InObj](TMethodParamTypes... InMethodParams) {return (*InObj)->InMethodPtr(InMethodParams...); };

		return Execute<TMethodRetType>(fun, InMethodParams...);
	}

	template<typename TFunctionRetType, typename ...TFunctionParamTypes, typename ...TFunctionParams>
	static ExecutionTimeInfo<TFunctionRetType> GetExecutionTime(TFunctionRetType(*InFunctionPtr)(TFunctionParamTypes...), const TFunctionParams&... InFunctionParams)
	{
		std::function<TFunctionRetType(TFunctionParamTypes...)> fun = InFunctionPtr;

		return Execute<TFunctionRetType>(fun, InFunctionParams...);
	}

private:

	template<typename TRetType, typename TSTDFunctionType, typename ...TParams>
	static ExecutionTimeInfo<TRetType> Execute(TSTDFunctionType InCallable, TParams... InParams)
	{
		ExecutionTimeInfo<TRetType> info;

		auto start = std::chrono::steady_clock::now();
		TRetType ret = InCallable(InParams...);
		auto end = std::chrono::steady_clock::now();

		info.ExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		info.ReturnValue = ret;

		return info;
	}

	Concurrency20Utility() = delete;
	~Concurrency20Utility() = delete;

};
