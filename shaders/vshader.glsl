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
    outColor = vec4(1.0, 0.0, 0.0, 1.0);  // crazy red if left unspecified
  }

  if (color == 1) outColor = vec4(0.2941, 0.1569, 0.0471, 1.0);  //brown
  if (color == 2) outColor = vec4(0.2, 0.2, 0.2, 1.0); //grey
  if (color == 3) outColor = vec4(0.1922, 0.051, 0.051, 1.0); //red
  if (color == 4) outColor = vec4(0.1412, 0.2588, 0.1059, 1.0); //green
  if (color == 5) outColor = vec4(0.0627, 0.0627, 0.2392, 1.0); //BLUE
  if (color == 6) outColor = vec4(0.1529, 0.1529, 0.1529, 1.0); //DARK GREEN/GREY
  if (color == 7) outColor = vec4(0.1176, 0.0353, 0.1608, 1.0); //PURPLE
  if (color == 8) outColor = vec4(0.0, 0.2863, 0.2745, 1.0); //cyan
  if (color == 9) outColor = vec4(0.4314, 0.1412, 0.0549, 1.0); //ORANGE
  if (color == 10) outColor = vec4(0.7922, 0.7922, 0.8745, 1.0); // SILVER

  if (color == 98) outColor = vec4(1.0, 1.0, 1.0, 1.0); //white
  if (color == 99) outColor = vec4(0.0, 0.0, 0.0, 1.0); //black

//ambient daytime lighting
  if (timeofday == 1)
    outColor = outColor * vec4(1.0, 1.0, 1.0, 1.0);
  if (timeofday == 2)
    outColor = outColor * vec4(0.8275, 0.8275, 0.8275, 1.0);
  if (timeofday == 3)
    outColor = outColor * vec4(0.7216, 0.7216, 0.7216, 1.0);
  if (timeofday == 4)
    outColor = outColor * vec4(0.5804, 0.5804, 0.5804, 1.0);
  if (timeofday == 5)
    outColor = outColor * vec4(0.4, 0.4, 0.4, 1.0);
  if (timeofday == 6)
    outColor = outColor * vec4(0.5922, 0.5922, 0.5922, 1.0);
  if (timeofday == 7)
    outColor = outColor * vec4(0.8314, 0.8275, 0.8275, 1.0);
  if (timeofday == 8)
    outColor = outColor * vec4(0.9059, 0.9059, 0.9059, 1.0);

}
