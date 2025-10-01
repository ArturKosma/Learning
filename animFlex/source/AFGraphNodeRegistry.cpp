#include "AFGraphNodeRegistry.h"

#include "AFStateClass.h"
#include "AFUtility.h"
#include "AFStateClass_Pivot.h"

std::shared_ptr<AFStateClass> AFGraphNodeRegistry::CreateStateClass(const std::string& className)
{
	switch (AFUtility::StringSwitch(className.c_str()))
	{
		case AFUtility::StringSwitch("Pivot"):
		{
			return std::make_shared<AFStateClass_Pivot>();
		}
		default:
		{

		}
	}

	return nullptr;
}

AFGraphNodeRegistry::AFGraphNodeRegistry()
{

}
