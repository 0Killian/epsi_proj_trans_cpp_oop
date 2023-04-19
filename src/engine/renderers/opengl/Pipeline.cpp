//
// Created by Killian on 18/04/2023.
//
#include "engine/renderers/opengl/Pipeline.h"

uint32_t Engine::OpenGL::s_boundProgramId = 0;

GLenum Engine::OpenGL::PrimitiveTopologyToOpenGLTopology(PrimitiveTopology topology)
{
    switch(topology)
    {
    case PrimitiveTopology::Points:return GL_POINTS;
    case PrimitiveTopology::Lines:return GL_LINES;
    case PrimitiveTopology::LineStrip:return GL_LINE_STRIP;
    case PrimitiveTopology::TriangleStrip:return GL_TRIANGLE_STRIP;
    case PrimitiveTopology::TriangleFan:return GL_TRIANGLE_FAN;
    default:return GL_TRIANGLES;
    }
}