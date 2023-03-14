#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#ifndef WGL_WGLEXT_PROTOTYPES
#define WGL_WGLEXT_PROTOTYPES
#endif

#include "ExternalLibraries/GL/glew.h"
#include "ExternalLibraries/GL/glut.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include "ExternalLibraries/GL/wglew.h"
#pragma comment(lib, "opengl32.lib")

#include <ExternalLibraries/glm/glm.hpp>
#include <ExternalLibraries/glm/ext.hpp>