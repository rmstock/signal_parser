#include "read.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

std::vector<std::string> read_words(std::string line)
{
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);    
    return vstrings;
}
void read_args(int argc, char* argv[], std::vector<std::string>& paths)
{
    for (int i = 1; i < argc; i++)
    {
        paths.push_back(argv[i]);
    }
}

void read_file(std::string& path, std::vector<std::string>& lines)
{
    std::string line;
    std::ifstream infile;
    infile.open(path, std::ios_base::in);
    while(getline(infile, line, '\n'))
    {
        if (line.back() == '\r')
            line.pop_back();
        if (line.back() == '\n')
            line.pop_back();
        if (line.back() == ' ')
            line.pop_back();
        lines.push_back(line);
    }
}

void read_message_name(std::vector<std::string>& lines, typetree& head)
{
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("MESSAGE NETWORK NAME:") != std::string::npos)
        {
            std::stringstream ss(lines[ i + 1]);
            ss >> head.name;
        }
        if (lines[i].find("MESSAGE SIZE:") != std::string::npos)
        {
            std::string num;
            std::istringstream iss(lines[i + 1]);
            iss >> num;
            head.bytes = std::stoi(num);
            break;
        }
    }
    head.type = "int*";
}

const unsigned int DELIM_SIZE = 1;
std::string delimiters[DELIM_SIZE] = {" "};

std::string read_signal_name(std::vector<std::string>& lines, unsigned int i)
{
    std::string name;
    std::string::size_type end;
    do{
        unsigned d = 0;
        do
        {
            end = lines[i].find(delimiters[d++]);
            if (end != 0 && lines[i].find(delimiters[5]) != std::string::npos)
                end = std::string::npos;
        } while (d < DELIM_SIZE && end == std::string::npos);
        if (end != std::string::npos)
            name += lines[i].substr(0, end);
        else
            name += lines[i];
        i++;
    } while(end == std::string::npos);
    std::replace(name.begin(), name.end(), '[', '_');
    std::replace(name.begin(), name.end(), ']', '_');
    return name; 
}

unsigned int read_signal_size(std::vector<std::string>& lines, unsigned int i)
{
    unsigned int start = i;
    unsigned int bits = 0;
    std::vector<std::string> words = read_words(lines[i]);
    while (i <= start + 5)
    {
        if (words.size() >= 7)
        {
            std::string isnum = words[words.size() - 7];
            if (std::all_of(isnum.begin(), isnum.end(), ::isdigit))
            {
                bits = std::stoi(words[words.size() - 7]);
                if (bits & 7)
                {
                    printf("Signal starting at line %d not in bytes: %d\n", i, bits);
                    printf("Line: %s", lines[i].c_str());
                }
                return (bits + 7) >> 3;
            }
        }
        i++;
        words = read_words(lines[i]);
    }
    printf("Couldn't find signal size after line %d. %d\n", start, i);
    return 0;
}

unsigned int read_signal_start(std::vector<std::string>& lines, unsigned int i)
{
    unsigned int start = i;
    std::vector<std::string> words = read_words(lines[i]);
    while (i <= start + 5)
    {
        if (words.size() >= 7)
        {
            std::string isnum = words[words.size() - 2];
            if (std::all_of(isnum.begin(), isnum.end(), ::isdigit))
                return std::stoi(words[words.size() - 2]);
        }
        i++;
        words = read_words(lines[i]);
    }
    printf("Couldn't find signal start after line %d. %d\n", start, i);
    return 0;
}

std::map<std::string, std::string> type_map = {{" ", " "}};

std::string read_signal_type(std::vector<std::string>& lines, unsigned int i)
{
    unsigned int start = i;
    while(true)
    {
        for (std::map<std::string, std::string>::iterator type = type_map.begin(); type != type_map.end(); type++)
        {
            if (lines[i].find(type->first) != std::string::npos)
            {
                return type->second;
            }
        }
        i++;
        if (i - start >= 5)
        {
            //Check if there are letters on the line before the next "Signal" label.
            //If so, call this again with that line?
            return "MISSING";
        }
    }
}

void read_signal_data(std::vector<std::string> lines, std::vector<typetree>& tree_head)
{
    //std::stack<typetree*> parents;
    //parents.push(tree_head);
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("Signal") == 0)
        {
            i++;
            typetree attr;
            //Line ends on space, or newline with "CORBA"
            attr.name = read_signal_name(lines, i);
            attr.type = read_signal_type(lines, i);
            attr.bytes = read_signal_size(lines, i);
            attr.byte_start = read_signal_start(lines, i);
            std::replace(attr.name.begin(), attr.name.end(), '.', '_');
            //String split name, and check against parents depth.
            tree_head.push_back(attr);
        }
    }
}
