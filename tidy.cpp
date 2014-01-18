#include <tidy.h>
#include <buffio.h>
#include <string>

namespace tidy {

	void tidy(std::string &input)
	{
		TidyBuffer output = {0};
		TidyBuffer errbuf = {0};
		TidyDoc tdoc = tidyCreate();   
		tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
		tidySetErrorBuffer(tdoc, &errbuf);      // Capture diagnostics

		tidyParseString(tdoc, input.c_str());
		tidyCleanAndRepair(tdoc);
		tidySaveBuffer(tdoc, &output);

		input = std::string((const char*)output.bp);
		
		tidyBufFree(&output);
		tidyBufFree(&errbuf);
		tidyRelease(tdoc);
	}
}
