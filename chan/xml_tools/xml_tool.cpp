#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <cassert>

void print_element_names(xmlNode *a_node, int indent)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
            for (int i = 0; i < indent; ++i)
                printf(".");

			//printf("node type: Element, name: %s\n", cur_node->name);
            if (xmlStrEqual(BAD_CAST "br", cur_node->name))
                continue;

            printf("<%s", cur_node->name);
            for (auto a = cur_node->properties; a; a = a->next) {
                printf(" ");
                printf("%s", a->name);
                for (auto b = a->children; b; b = b->next) {
                    auto t = xmlNodeGetContent(b);
                    printf("='%s'", t);
                    free(t);
                }
            }
            printf(">\n");
		}

		print_element_names(cur_node->children, indent+1);
		if (cur_node->type != XML_TEXT_NODE && cur_node->name) {
            for (int i = 0; i < indent; ++i)
                printf(".");
            printf("</%s>\n", cur_node->name);
        }
	}
}

int run_xpath(xmlDoc *doc, const char *xpath)
{
	xmlXPathContext *xpath_ctx = xmlXPathNewContext(doc);
	if (!xpath_ctx) {
		printf("Could not create new xpath context.\n");
		//xmlFreeDoc(doc);
		return 1;
	}

	xmlChar *xml_xpath = xmlCharStrdup(xpath);
	xmlXPathObject *xpath_obj = xmlXPathEvalExpression(xml_xpath, xpath_ctx);
	if (!xpath_obj) {
		puts("Could not evaluate xpath expression.\n");
		free(xml_xpath);
		return 1;
	}

    assert(xpath_obj->nodesetval->nodeTab);
    for (auto n = *xpath_obj->nodesetval->nodeTab; n; n = n->next) {
        printf("----------------------------------------"
                "----------------------------------------\n");
        print_element_names(n, 0);
        printf("----------------------------------------"
                "----------------------------------------\n");
    }

	//xmlXPathFreeObject(xpath_obj);
	free(xml_xpath);

	//xmlXPathFreeContext(xpath_ctx); 
	//xmlFreeDoc(doc);
	return 0;
}

#include <string>
#include <fstream>
#include <streambuf>
#include "../../tidy.hpp"
int main(int argc, char **argv)
{
    assert(argv[1]); 
    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    //tidy::tidy(str);
	//xmlDocPtr doc = htmlParseFile(f_doc, NULL);
	xmlDocPtr doc = htmlParseDoc(BAD_CAST str.c_str(), NULL);
	if (!doc) {
		printf("libxml failed to parse file <%s>\n", argv[1]);
		return 1;
	}

	xmlNode *doc_head = xmlDocGetRootElement(doc);
    if (argc < 3) {
        print_element_names(doc_head, 0);
    } else {
        run_xpath(doc, argv[2]);
    } 

	//xmlFree(doc_head);
	return 0;
}
