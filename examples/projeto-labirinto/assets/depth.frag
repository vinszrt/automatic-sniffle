#version 410

in vec4 fragColor;
out vec4 outColor;

void main() {
  if (gl_FrontFacing) {
    outColor = vec4(1.0f, 0, 0, 0.5f);
  } else {
    outColor = fragColor * 0.5;
  }
}