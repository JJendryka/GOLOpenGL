#version 330

uniform sampler2D state;
uniform vec2 scale;
uniform ivec2 position;
uniform bool grid;

out vec4 color;

void main() {
    int x = ivec2(gl_FragCoord.xy).x;
    int y = ivec2(gl_FragCoord.xy).y;
    if (grid && (x - (scale * floor(x/scale)) == 0 || (y - (scale * floor(y/scale))) == 0)) {
        color = vec4(1, 1, 1, 1);
    }
    else {
        color = texelFetch(state, ivec2((gl_FragCoord.xy / scale) + position),0);
    }
}

