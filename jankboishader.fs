#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform sampler2D frames1;
uniform sampler2D frames2;
uniform sampler2D frames3;
uniform sampler2D frames4;
uniform sampler2D frames5;
uniform sampler2D frames6;
uniform sampler2D frames7;
uniform sampler2D frames8;
uniform int order;





void main()
{
   if(order== 0){
       FragColor = texture(frames1, TexCoord);
   }else if(order==1){
       FragColor = texture(frames2, TexCoord);
   }else if(order==2){
       FragColor = texture(frames3, TexCoord);
   }else if(order==3){
       FragColor = texture(frames4, TexCoord);
   }else if(order==4){
       FragColor = texture(frames5, TexCoord);
   }else if(order==5){
       FragColor = texture(frames6, TexCoord);
   }else if(order==6){
       FragColor = texture(frames7, TexCoord);
   }else if(order==7){
       FragColor = texture(frames8, TexCoord);
   }
  //  FragColor = texture(frames[order],TexCoord);
	//linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}