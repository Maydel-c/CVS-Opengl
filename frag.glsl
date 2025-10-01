#version 460 core

out vec4 fragColor;

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(1920.0, 1013.0);
  fragColor = vec4(uv, 0.0, 1.0);

}
