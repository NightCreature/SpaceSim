//uniform vec3 spotpos;
uniform vec3 pointpos;

varying vec3 position;
varying vec3 normal;
varying float constatt, linatt, quadatt;
varying vec2 texcoords0, texcoords1;
varying vec3 tspos, tppos;

void main()
{
   
	normal = normalize(gl_Normal);
    position = gl_Vertex.xyz;
    constatt = gl_LightSource[1].constantAttenuation;
    linatt = gl_LightSource[1].linearAttenuation;
	quadatt = gl_LightSource[1].quadraticAttenuation;
	vec4 texcoord0 = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    vec4 texcoord1 = gl_TextureMatrix[1] * gl_MultiTexCoord1;
    texcoords0 = texcoord0.xy;
    texcoords1 = texcoord1.xy;
   
   tspos = gl_LightSource[0].position;
   tppos = gl_LightSource[1].position;
   
    gl_Position = ftransform();
}