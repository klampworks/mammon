#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>

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

int parse_file(const char *f_doc) 
{
	xmlDocPtr doc = htmlParseFile(f_doc, NULL);
	if (!doc) {
		printf("libxml failed to parse file <%s>\n", f_doc);
		return 1;
	}

	xmlNode *doc_head = xmlDocGetRootElement(doc);
    print_element_names(doc_head, 0);

	xmlFree(doc_head);
	return 0;
}

int main(int argc, char **argv)
{
    parse_file(argv[1]);
}
