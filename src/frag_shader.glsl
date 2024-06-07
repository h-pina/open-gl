#version 330 core

in vec2 texCoordsOut;
out vec4 FragColor;

uniform sampler2D woodenSampler;
uniform sampler2D smileySampler;

void main()
{
    //FragColor = mix(texture(woodenSampler, texCoordsOut), texture(smileySampler, vec2(1-texCoordsOut.x,1-texCoordsOut.y)), 0.2);
    FragColor = texture(woodenSampler, texCoordsOut);
} 
