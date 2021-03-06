#version 440

uniform sampler2D state;
uniform vec2 scale;

out vec4 color;

int get(int x, int y) {
    return int(floor((texelFetch(state, ivec2((gl_FragCoord.xy + vec2(x, y))), 0)).r));
}

void main() {
    int sum = get(-1, -1) +
              get(-1,  0) +
              get(-1,  1) +
              get( 0, -1) +
              get( 0,  1) +
              get( 1, -1) +
              get( 1,  0) +
              get( 1,  1);
    if (sum == 3) {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    } else if (sum == 2) {
        float current = float(get(0, 0));
        color = vec4(current, current, current, 1.0);
    } else {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }

}