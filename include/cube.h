#ifndef CUBE_H
#define CUBE_H

static float cubeVertices[] =
    {
        // FACE
        -0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 0.0f, 0.0f, -1.0f, /* Normal */
        0.5f, -0.5f, -0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 0.0f, 0.0f, -1.0f,  /* Normal */
        0.5f, 0.5f, -0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 0.0f, 0.0f, -1.0f,   /* Normal */
        0.5f, 0.5f, -0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 0.0f, 0.0f, -1.0f,   /* Normal */
        -0.5f, 0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 0.0f, 0.0f, -1.0f,  /* Normal */
        -0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 0.0f, 0.0f, -1.0f, /* Normal */
        // FACE
        -0.5f, -0.5f, 0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 0.0f, 0.0f, 1.0f, /* Normal */
        0.5f, -0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 0.0f, 0.0f, 1.0f,  /* Normal */
        0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 0.0f, 0.0f, 1.0f,   /* Normal */
        0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 0.0f, 0.0f, 1.0f,   /* Normal */
        -0.5f, 0.5f, 0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 0.0f, 0.0f, 1.0f,  /* Normal */
        -0.5f, -0.5f, 0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 0.0f, 0.0f, 1.0f, /* Normal */
        // FACE
        -0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ -1.0f, 0.0f, 0.0f,   /* Normal */
        -0.5f, 0.5f, -0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ -1.0f, 0.0f, 0.0f,  /* Normal */
        -0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ -1.0f, 0.0f, 0.0f, /* Normal */
        -0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ -1.0f, 0.0f, 0.0f, /* Normal */
        -0.5f, -0.5f, 0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ -1.0f, 0.0f, 0.0f,  /* Normal */
        -0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ -1.0f, 0.0f, 0.0f,   /* Normal */
        // FACE
        0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 1.0f, 0.0f, 0.0f,   /* Normal */
        0.5f, 0.5f, -0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 1.0f, 0.0f, 0.0f,  /* Normal */
        0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 1.0f, 0.0f, 0.0f, /* Normal */
        0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 1.0f, 0.0f, 0.0f, /* Normal */
        0.5f, -0.5f, 0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 1.0f, 0.0f, 0.0f,  /* Normal */
        0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 1.0f, 0.0f, 0.0f,   /* Normal */
        // FACE
        -0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 0.0f, -1.0f, 0.0f, /* Normal */
        0.5f, -0.5f, -0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 0.0f, -1.0f, 0.0f,  /* Normal */
        0.5f, -0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 0.0f, -1.0f, 0.0f,   /* Normal */
        0.5f, -0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 0.0f, -1.0f, 0.0f,   /* Normal */
        -0.5f, -0.5f, 0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 0.0f, -1.0f, 0.0f,  /* Normal */
        -0.5f, -0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 0.0f, -1.0f, 0.0f, /* Normal */
        // FACE
        -0.5f, 0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 0.0f, 1.0f, 0.0f, /* Normal */
        0.5f, 0.5f, -0.5f, /*V*/ 1.0f, 1.0f, /*Tex*/ 0.0f, 1.0f, 0.0f,  /* Normal */
        0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 0.0f, 1.0f, 0.0f,   /* Normal */
        0.5f, 0.5f, 0.5f, /*V*/ 1.0f, 0.0f, /*Tex*/ 0.0f, 1.0f, 0.0f,   /* Normal */
        -0.5f, 0.5f, 0.5f, /*V*/ 0.0f, 0.0f, /*Tex*/ 0.0f, 1.0f, 0.0f,  /* Normal */
        -0.5f, 0.5f, -0.5f, /*V*/ 0.0f, 1.0f, /*Tex*/ 0.0f, 1.0f, 0.0f, /* Normal */
};

#endif