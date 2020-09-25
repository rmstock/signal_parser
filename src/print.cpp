#include "print.h"

void print_header(typetree& tree_head, std::vector<typetree>& tree)
{
    unsigned int byte_pos = 0;
    unsigned int pad_count = 0;

    FILE *header = fopen((tree_head.name + ".h").c_str(), "w");
    if (header)
    {
        fprintf(header, "#ifndef %s_HEADER\n", tree_head.name.c_str());
        fprintf(header, "#define %s_HEADER\n", tree_head.name.c_str());
        fprintf(header, "typdef struct %s\n{\n", tree_head.name.c_str());
        for (unsigned int def = 0; def < tree.size(); def++)
        {
            if (tree[def].byte_start != byte_pos)
            {
                fprintf(header, "\tchar pad_%d[%d];\n", pad_count++, tree[def].byte_start - byte_pos);
            }
            fprintf(header, "\t%s %s;//Start: %d Size: %d\n", tree[def].type.c_str(), tree[def].name.c_str(),
                    tree[def].byte_start, tree[def].bytes);
            byte_pos = tree[def].byte_start + tree[def].bytes;
        }
        fprintf(header, "}%s;\n#endif\n", tree_head.name.c_str());
    }
    fclose(header);
}
