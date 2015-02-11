#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>

void print_element_names(xmlNode *a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("node type: Element, name: %s\n", cur_node->name);
		}

		print_element_names(cur_node->children);
	}
}

int parse_file(const char *f_doc) 
{
	xmlDocPtr doc = htmlParseFile(f_doc, NULL);
	if (!doc) {
		printf("libxml failed to parse file <%s>\n", f_doc);
		return 1;
	}

	xmlNode *doc_head = xmlDocGetRootElement(doc);
    print_element_names(doc_head);

	xmlFree(doc_head);
	return 0;
}

int main(int argc, char **argv)
{
    parse_file(argv[1]);
}
