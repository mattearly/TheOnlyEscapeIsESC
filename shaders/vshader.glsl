attribute vec4 aPos;
attribute vec4 aColor;

varying vec4 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int color;
uniform int timeofday;

void main()
{
  gl_Position = projection * view * model * aPos;

  if (aColor.x >= 0.01 || aColor.y >= 0.01 || aColor.z >= 0.01) {
    outColor = aColor;
  } else {
    outColor = vec4(0.48, 0.21, 0.1, 1.0);  // brown if left unspecified
  }

  if (color == 1) outColor = vec4(0.31, 0.15, 0.02, 1.0);  //brown
  if (color == 2) outColor = vec4(0.23, 0.23, 0.23, 1.0); //grey
  if (color == 3) outColor = vec4(0.75, 0.02, 0.02, 1.0); //red
  if (color == 4) outColor = vec4(0.10, 0.45, 0.01, 1.0); //green
  if (color == 5) outColor = vec4(0.14, 0.14, 0.90, 1.0); //BLUE
  if (color == 6) outColor = vec4(0.10, 0.14, 0.12, 1.0); //DARK GREEN/GREY
  if (color == 7) outColor = vec4(0.35, 0.25, 0.32, 1.0); //PURPLE
  if (color == 8) outColor = vec4(0.02, 0.58, 0.55, 1.0); //cyan
  if (color == 9) outColor = vec4(1.00, 0.50, 0.00, 1.0); //ORANGE
  if (color == 10) outColor = vec4(0.70, 0.70, 0.765, 1.0); // SILVER

  if (color == 98) outColor = vec4(0.95, 0.95, 0.90, 1.0); //white
  if (color == 99) outColor = vec4(0.03, 0.03, 0.03, 1.0); //black

//ambient daytime lighting
/*  if (timeofday == 1)
    outColor = outColor * vec4(0.22f, 0.17f, 0.60f, 1.0f);
  if (timeofday == 2)
    outColor = outColor * vec4(0.40f, 0.10f, 0.20f, 1.0f);
  if (timeofday == 3)
    outColor = outColor * vec4(0.18f, 0.10f, 0.10f, 1.0f);
  if (timeofday == 4)
    outColor = outColor * vec4(0.25f, 0.20f, 0.20f, 1.0f);
  if (timeofday == 5)
    outColor = outColor * vec4(0.35f, 0.30f, 0.40f, 1.0f);
  if (timeofday == 6)
    outColor = outColor * vec4(0.30f, 0.20f, 0.62f, 1.0f);
  if (timeofday == 7)
    outColor = outColor * vec4(0.24f, 0.17f, 0.72f, 1.0f);
  if (timeofday == 8)
    outColor = outColor * vec4(0.17, 0.24f, 0.85f, 1.0f);
    */


}
