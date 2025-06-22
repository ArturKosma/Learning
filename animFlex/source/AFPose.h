#pragma once
#include <sstream>

class AFPose
{
	
};

// AFPose will never get passed as a value through string, so we can just return the stream.
// It will be always passed through a connection, and supplied via node Evaluate().
inline std::istream& operator>>(std::istream& is, AFPose& pose)
{
    return is;
}
