#include <iostream>
#include <vector>

#include <dlib/svm.h>

int main()
{
	using sampleType = dlib::matrix<double, 1, 1>;
	using kernelType = dlib::linear_kernel<sampleType>;

    std::vector<sampleType> temperatures = { {25.0f}, {27.0f}, {29.0f}, {33.0f}, {50.0f} };
    std::vector<double> iceCreamsSold = {10.0f, 11.0f, 12.0f, 14.0f, 20.0f};

	dlib::krr_trainer<kernelType> trainer;

	trainer.set_kernel(kernelType());
	dlib::decision_function<kernelType> df = trainer.train(temperatures, iceCreamsSold);

	std::vector<sampleType> toPredict = { {26.0f}, {28.0f}, {30.0f}, {40.0f} };
	for (sampleType& toPredictRecord : toPredict)
	{
		double prediction = df(toPredictRecord);

		std::cout << prediction << std::endl;
	}
}
