#include <gx_src_es20.h>


const char* gx::unfa::get_glsl_type_name(const int& n)
{
    static std::map<int, const char*> t_name =
    {
        { GL_FLOAT                                     , "float"                },
        { GL_FLOAT_VEC2                                , "vec2"                 },
        { GL_FLOAT_VEC3                                , "vec3"                 },
        { GL_FLOAT_VEC4                                , "vec4"                 },
        { GL_INT                                       , "int"                  },
        { GL_INT_VEC2                                  , "ivec2"                },
        { GL_INT_VEC3                                  , "ivec3"                },
        { GL_INT_VEC4                                  , "ivec4"                },
        { GL_UNSIGNED_INT                              , "unsigned int"         },
        { GL_BOOL                                      , "bool"                 },
        { GL_BOOL_VEC2                                 , "bvec2"                },
        { GL_BOOL_VEC3                                 , "bvec3"                },
        { GL_BOOL_VEC4                                 , "bvec4"                },
        { GL_FLOAT_MAT2                                , "mat2"                 },
        { GL_FLOAT_MAT3                                , "mat3"                 },
        { GL_FLOAT_MAT4                                , "mat4"                 },
        { GL_SAMPLER_2D                                , "sampler2D"            },
        { GL_SAMPLER_CUBE                              , "samplerCube"          },
    };

    auto exists = t_name.find(n);

    return t_name.end() == exists ? "UNSUPPORTED_UNIFORM_TYPE" : exists->second;
}


gx::unfa* gx::unfa::make_new(const int& gl_type_id)
{
    switch (gl_type_id)
    {
    case GL_FLOAT             : return new gx::uv1f;  // "float"
    case GL_FLOAT_VEC2        : return new gx::uv2f;  // "vec2"
    case GL_FLOAT_VEC3        : return new gx::uv3f;  // "vec3"
    case GL_FLOAT_VEC4        : return new gx::uv4f;  // "vec4"
    case GL_INT               : return new gx::uv1i;  // "int"
    case GL_INT_VEC2          : return new gx::uv2i;  // "ivec2"
    case GL_INT_VEC3          : return new gx::uv3i;  // "ivec3"
    case GL_INT_VEC4          : return new gx::uv4i;  // "ivec4"
    case GL_UNSIGNED_INT      : return new gx::uv1u;  // "unsigned int"
    case GL_BOOL              : return new gx::uv1b;  // "bool"
    case GL_BOOL_VEC2         : return new gx::uv2b;  // "bvec2"
    case GL_BOOL_VEC3         : return new gx::uv3b;  // "bvec3"
    case GL_BOOL_VEC4         : return new gx::uv4b;  // "bvec4"
    case GL_FLOAT_MAT2        : return new gx::um2f;  // "mat2"
    case GL_FLOAT_MAT3        : return new gx::um3f;  // "mat3"
    case GL_FLOAT_MAT4        : return new gx::um4f;  // "mat4"
    case GL_SAMPLER_2D        : return new gx::tx2d;  // "sampler2D"
    case GL_SAMPLER_CUBE      : return new gx::txcb;  // "samplerCube"
    };

    return nullptr;
}
