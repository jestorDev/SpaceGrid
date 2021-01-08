#version 430

in vec2 tc;
out vec4 color;
#define pi 3.14159265358 
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int background	;
uniform vec2 resolution;
uniform float time;
layout (binding=0) uniform sampler2D s;

void main(void)
{
	if (background == 1){
		
		vec2 uv = gl_FragCoord.xy / resolution;

		float forma = ( 
        sin(uv.x *10.0*pi + time*2
            + sin(uv.y *10.0*pi - time)) * 0.2+ 0.5)*0.8 
        + (sin(uv.y *10.0*pi + time*0.5) * 0.2 + 0.5)* 0.8
    ;
    	color = vec4(0.2*forma,0.2*forma,0.5 + 0.2*forma,1);

		//color = vec4(0.9 + 0.1*sin(time),0.9 + 0.1*sin(time*2),1,1);
	}
	else
		color = texture(s,tc);
}
