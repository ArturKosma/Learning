#include <iostream>
#include <vector>

#include <dlib/svm.h>

int main()
{
	// Typedef some names for easier usage.
	using sampleType = dlib::matrix<double, 1, 1>;
	using kernelType = dlib::linear_kernel<sampleType>;

	// Define samples upon which we'll train our linear regression.
    std::vector<sampleType> temperatures = { {25.0f}, {27.0f}, {29.0f}, {33.0f}, {50.0f} };
    std::vector<double> iceCreamsSold = {10.0f, 11.0f, 12.0f, 14.0f, 20.0f};

	// Create trainer, set it's kernel (type) and train it with samples.
	dlib::krr_trainer<kernelType> trainer;
	trainer.set_kernel(kernelType());
	dlib::decision_function<kernelType> df = trainer.train(temperatures, iceCreamsSold);

	// Test the prediction with some new values.
	std::vector<sampleType> toPredict = { {26.0f}, {28.0f}, {30.0f}, {40.0f} };
	for (sampleType& toPredictRecord : toPredict)
	{
		double prediction = df(toPredictRecord);

		std::cout << prediction << std::endl;
	}
}
