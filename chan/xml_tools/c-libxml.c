#include <stdio.h>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/xpath.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int file_exists(const char *filename)
{
	struct stat buf;
	if (stat(filename, &buf)) {
		printf("Could not stat file <%s> - %s\n", 
			filename, strerror(errno));
		return 0;
	}

	return 1;
}

void print_usage(const char *argv_0)
{
	printf("Usage: %s <input file>\n"
	       "If input file is not specified then \"input.html\" will be used"
	       " by default.\n", argv_0);
}

xmlChar* run_xpath (const char *input_file, const char *xpath)
{
	xmlDocPtr doc = NULL;
	xmlXPathContext *xpath_ctx = NULL;
	xmlChar *xml_xpath = NULL;
	xmlXPathObject *xpath_obj = NULL;
	xmlNode *xpath_result = NULL;

	/* Parse the file into a DOM tree stored as doc. */
	doc = htmlParseFile(input_file, NULL);
	if (!doc) {
		printf("Failed to parse input file <%s>\n", input_file);
		goto FAIL;
	}

	xpath_ctx = xmlXPathNewContext(doc);
	if (!xpath_ctx) {
		printf("Could not create new xpath context.\n");
		goto FAIL;
	}

	/* Convert normal string into a special libxml one. */
	xml_xpath = xmlCharStrdup(xpath);

	xpath_obj = xmlXPathEvalExpression(xml_xpath, xpath_ctx);
	if (!xpath_obj) {
		printf("Could not evaluate xpath expression <%s>\n", xpath);
		goto FAIL;
	}

	xpath_result = xpath_obj->nodesetval->nodeTab[0];
	if (!xpath_result) {
		printf("xpath expression <%s> did not evaluate to anything.\n",
			xpath);
		goto FAIL;
	}
	
	xmlChar *result = xmlNodeGetContent(xpath_result);

	FAIL:
	xmlXPathFreeObject(xpath_obj);
	xmlFree(xml_xpath);
	xmlXPathFreeContext(xpath_ctx);
	xmlFreeDoc(doc);

	return result;
}

int main(int argc, char **argv)
{
	const char *input_file;

	if (argc > 1)
		input_file = argv[1];
	else
		input_file = "input.html";

	if (!file_exists(input_file)) {
		printf("File <%s> does not exist.\n", input_file);
		print_usage(*argv);
		exit(1);
	}

	const char *xpath = "(//h1)[1]";
	xmlChar *res = run_xpath(input_file, xpath);
	printf("Xpath: <%s>\nResult: <%s>\n", xpath, (const char*)res);

	return 0;
}
