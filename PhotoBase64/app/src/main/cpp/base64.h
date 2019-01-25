//
// Created by jesson on 2019/1/25.
//

#ifndef PHOTOBASE64_BASE64_H
#define PHOTOBASE64_BASE64_H


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <android/log.h>
#include <cctype>
#include <malloc.h>
#include <ctype.h>
#include <string>


std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif //PHOTOBASE64_BASE64_H
