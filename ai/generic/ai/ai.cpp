#include <iostream>
#include <chrono>
#include <cmath>
#include <Eigen/Dense>
#include <vector>
#include <thread>

int main()
{
	const int numIterations = 100;
	const int vectorSize = 1'000'000;

	Eigen::VectorXd data = Eigen::VectorXd::Random(vectorSize);
	Eigen::MatrixXd dataMatrix;
	dataMatrix.resize(vectorSize, 1);
	dataMatrix.row(0) = data;
	std::vector<double> data_std(vectorSize);
	for (int i = 0; i < vectorSize; ++i)
	{
		data_std[i] = data[i];
	}

    auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numIterations; ++i)
	{
		for (int j = 0; j < vectorSize; ++j)
		{
			data_std[j] = std::sqrt(data_std[j]);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);


	Eigen::setNbThreads(std::thread::hardware_concurrency());


	auto startEigen = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numIterations; ++i)
	{
		Eigen::MatrixXd result = dataMatrix.array().sqrt();
	}

	auto endEigen = std::chrono::high_resolution_clock::now();


	auto durationEigen = std::chrono::duration_cast<std::chrono::microseconds>(endEigen - startEigen);

	std::cout << "Elapsed time std: " << duration.count() << " microseconds\n";
	std::cout << "Elapsed time eigen: " << durationEigen.count() << " microseconds\n";


    std::cin.get();
}
