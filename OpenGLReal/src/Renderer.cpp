#include "Renderer.h"

void ClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool CheckError()
{
	if (glGetError() == GL_NO_ERROR)
	{
		return false;
	}
	return true;
}
