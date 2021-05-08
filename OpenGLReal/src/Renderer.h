#pragma once 

#include <GL/glew.h>


void ClearError();
bool CheckError();

#define ASSERT(x) if (x) __debugbreak();
#define GLLog(x) ClearError();\
                 x;\
                 ASSERT(CheckError())

