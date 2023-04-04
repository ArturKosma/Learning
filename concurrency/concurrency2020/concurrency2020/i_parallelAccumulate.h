#pragma once

#include <vector>
#include <algorithm>
#include <thread>
#include <numeric>

template<typename TIterator, typename TVectorType>
struct AccumulateStruct
{
	void operator()(TIterator InItBegin, TIterator InItEnd, TVectorType& InTarget)
	{
		InTarget = std::accumulate(InItBegin, InItEnd, 0);
	}
};

class i_parallelAccumulate
{
public:
	i_parallelAccumulate() = delete;
	~i_parallelAccumulate() = delete;

	template<typename TVectorType>
	static TVectorType ParallelAccumulate(const std::vector<TVectorType>& InVector)
	{
		if (InVector.empty())
		{
			return (TVectorType)0;
		}

		const size_t maxThreads = std::thread::hardware_concurrency();
		const size_t minThreads = InVector.size() / maxThreads;
		const size_t numThreads = std::min(minThreads, maxThreads);
		const size_t sizeChunk = numThreads > 0 ? InVector.size() / numThreads : InVector.size();
		const size_t numChunk = InVector.size() / sizeChunk;

		std::vector<TVectorType> results(numThreads);
		std::vector<std::thread> threads(numThreads - 1);

		auto itBegin = InVector.begin();
		auto itEnd = InVector.begin();

		for (size_t i = 0; i < numThreads - 1; ++i)
		{
			itBegin = itEnd;
			std::advance(itEnd, sizeChunk);

			threads[i] = std::thread(AccumulateStruct<decltype(itBegin), TVectorType>(), itBegin, itEnd, std::ref(results[i]));

			itBegin = itEnd;
		}

		for (auto& entry : threads)
		{
			entry.join();
		}

		results[numThreads - 1] = std::accumulate(itBegin, InVector.end(), 0);

		return std::accumulate(results.begin(), results.end(), 0);
	}
};

// #include "pch.h"
// #include "i_parallelAccumulate.h"
// 
// #include <iostream>
// 
// template<typename TFunctionRetType, typename ...TFunctionParams>
// void testFun(TFunctionRetType(*InFunctionPtr)(TFunctionParams...), const TFunctionParams&... InFunctionParams)
// {
// 	return;
// }
// 
// int main()
// {
// 	std::vector<int> myvec(100, 1);
// 
// 	auto addressParallel = &i_parallelAccumulate::ParallelAccumulate<int>;
// 	auto addressNoParallel = &std::accumulate<std::vector<int>::iterator, int>;
// 
// 	auto infoParallel = Concurrency20Utility::GetExecutionTime(addressParallel, myvec);
// 	auto infoNoParallel = Concurrency20Utility::GetExecutionTime(addressNoParallel, myvec.begin(), myvec.end(), 0);
// 
// 	printf("parallel: %d\n", infoParallel.ExecutionTime);
// 	printf("no parallel: %d\n", infoNoParallel.ExecutionTime);
// 
// 	std::cin.get();
// }
