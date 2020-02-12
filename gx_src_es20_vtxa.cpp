#include <gx_src_es20.h>

const char* gx::vtxa::get_glsl_type_name(const int& n)
{
    static std::map<int, const char*> t_name = {
        { GL_FLOAT             , "float"        },
        { GL_FLOAT_VEC2        , "vec2"         },
        { GL_FLOAT_VEC3        , "vec3"         },
        { GL_FLOAT_VEC4        , "vec4"         },
        { GL_INT               , "int"          },
        { GL_INT_VEC2          , "ivec2"        },
        { GL_INT_VEC3          , "ivec3"        },
        { GL_INT_VEC4          , "ivec4"        },
        { GL_UNSIGNED_INT      , "unsigned int" },
        { GL_BOOL              , "bool"         },
        { GL_BOOL_VEC2         , "bvec2"        },
        { GL_BOOL_VEC3         , "bvec3"        },
        { GL_BOOL_VEC4         , "bvec4"        },
        { GL_FLOAT_MAT2        , "mat2"         },
        { GL_FLOAT_MAT3        , "mat3"         },
        { GL_FLOAT_MAT4        , "mat4"         },
    };

    auto exists = t_name.find(n);

    return t_name.end() == exists ? "UNSUPPORTED_ATTRIBUTE_TYPE" : exists->second;
}


gx::vtxa* gx::vtxa::make_new(const int& gl_type_id)
{
    switch (gl_type_id)
    {
    case GL_FLOAT             : return new gx::av1f;  // "float"
    case GL_FLOAT_VEC2        : return new gx::av2f;  // "vec2"
    case GL_FLOAT_VEC3        : return new gx::av3f;  // "vec3"
    case GL_FLOAT_VEC4        : return new gx::av4f;  // "vec4"
    case GL_INT               : return new gx::av1i;  // "int"
    case GL_INT_VEC2          : return new gx::av2i;  // "ivec2"
    case GL_INT_VEC3          : return new gx::av3i;  // "ivec3"
    case GL_INT_VEC4          : return new gx::av4i;  // "ivec4"
    case GL_UNSIGNED_INT      : return new gx::av1u;  // "unsigned int"
    case GL_BOOL              : return new gx::av1b;  // "bool"
    case GL_BOOL_VEC2         : return new gx::av2b;  // "bvec2"
    case GL_BOOL_VEC3         : return new gx::av3b;  // "bvec3"
    case GL_BOOL_VEC4         : return new gx::av4b;  // "bvec4"
    case GL_FLOAT_MAT2        : return new gx::am2f;  // "mat2"
    case GL_FLOAT_MAT3        : return new gx::am3f;  // "mat3"
    case GL_FLOAT_MAT4        : return new gx::am4f;  // "mat4"
    };

    return nullptr;
}
