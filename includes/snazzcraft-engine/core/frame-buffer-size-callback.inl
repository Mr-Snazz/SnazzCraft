#include "snazzcraft-engine/core/core.hpp"

inline void FrameBufferSizeCallBack(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}