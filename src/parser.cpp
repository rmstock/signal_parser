#include "typetree.h"
#include "read.h"
#include "print.h"

int main(int argc, char* argv[])
{
    std::vector<std::string> paths;
    read_args(argc, argv, paths);
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        std::vector<std::string> lines;
        std::vector<std::vector<std::string>> signal_lines;
        read_file(paths[i], lines);
        typetree tree_head;
        std::vector<typetree> tree;
        read_message_name(lines, tree_head);
        read_signal_data(lines, tree);
        print_header(tree_head, tree);
    }
}
