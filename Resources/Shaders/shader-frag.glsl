#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 boxColor;
uniform bool doText;

void main()
{
    if(doText == true){        
        FragColor = texture(texture1, TexCoord);
    }else{
        FragColor = vec4(boxColor,1.0f);
    }
}