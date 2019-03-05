#ifndef CONVERT_H
#define CONVERT_H

#include <vector>
#include <iostream>
#include "node.h"

Node* StrToNode(const std::string &str);

std::string NodeToStr(const Node* node);

std::vector<std::string> split_str(const std::string& src, const std::string& delim);

#endif
