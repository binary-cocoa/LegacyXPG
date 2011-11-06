#version 120
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform samplerCube cubeMap;

varying vec4 ex_Color;
varying vec3 ex_Normal;
varying vec3 ex_Texture;
varying vec3 ex_LightDir;

void main(void)
{
    float diff = max(0.0, dot(normalize(ex_Normal), normalize(ex_LightDir)));
    
    vec4 lightColor = diff * diffuseColor;
    
    lightColor += ambientColor;
    
    //lightColor *= ex_Color;
    lightColor *= textureCube(cubeMap, ex_Texture);
    
    vec3 vReflection = normalize(reflect(-normalize(ex_LightDir),
        normalize(ex_Normal)));
    float spec = max(0.0, dot(normalize(ex_Normal), vReflection));
    
    if (diff != 0)
    {
        float fSpec = pow(spec, 128.0);
        lightColor.rgb += vec3(fSpec, fSpec, fSpec);
    }
    
    gl_FragColor = lightColor;
}

