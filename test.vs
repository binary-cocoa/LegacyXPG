#version 120
uniform mat4 MVPM;
uniform mat4 MVM;
uniform mat4 NM;
uniform vec3 lightPosition;

attribute vec3 in_Position;
attribute vec4 in_Color;
attribute vec3 in_Normal;

varying vec4 ex_Color;
varying vec3 ex_Normal;
varying vec3 ex_Texture;
varying vec3 ex_LightDir;

void main(void)
{
    vec4 p = vec4(in_Position, 1.0);
    
    ex_Normal = vec3(NM * vec4(in_Normal, 1.0));
    ex_Texture = in_Normal;
    
    vec4 vPosition4 = MVM * p;
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
    
    ex_LightDir = normalize(lightPosition - vPosition3);
    
    gl_Position = MVPM * p;
    ex_Color = in_Color;
}

