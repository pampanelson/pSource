uniform float color_r;

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    fragColor = vec4(color_r,1.0 - color_r,1.0,1.0);

}