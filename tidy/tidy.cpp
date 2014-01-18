#include <tidy.h>
#include <buffio.h>

#include <string>
#include <iostream>

namespace tidy {

	void tidy(std::string &input)
	{
		TidyBuffer output = {0};
		TidyDoc tdoc = tidyCreate();   
		tidyOptSetBool(tdoc, TidyXhtmlOut, yes);

		tidyParseString(tdoc, input.c_str());
		tidyCleanAndRepair(tdoc);
		tidySaveBuffer(tdoc, &output);

		input = std::string((const char*)output.bp);
		
		tidyBufFree(&output);
		tidyRelease(tdoc);
	}
}

int main()
{
	std::string input("<title>Foo</title><p>Foo!");
	tidy::tidy(input);
	std::cout << input << std::endl;

}
