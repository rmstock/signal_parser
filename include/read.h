#ifndef READ_H
#define READ_H

#include <vector>
#include <string>
#include "typetree.h"

void read_args(int argc, char* argv[], std::vector<std::string>& paths);

void read_file(std::string& path, std::vector<std::string>& lines);

void read_message_name(std::vector<std::string>& lines, typetree& head);

std::string read_signal_name(std::vector<std::string>& lines, unsigned int start, typetree& signal);

unsigned int read_signal_size(std::vector<std::string>& lines, unsigned int i, typetree& signal);

unsigned int read_signal_start(std::vector<std::string>& lines, unsigned int i, typetree& signal);

std::string read_signal_type(std::vector<std::string>& lines, unsigned int i, typetree& signal);

void read_signal_data(std::vector<std::string> lines, std::vector<typetree>& tree_head);

#endif
