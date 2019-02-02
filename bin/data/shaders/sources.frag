// uniform float color_r;

// void mainImage( out vec4 fragColor, in vec2 fragCoord )
// {
//     fragColor = vec4(color_r,1.0 - color_r,1.0,1.0);

// }


//#define brightness .001    // use this and next ray_brightness to control global light ....
//#define ray_brightness 6.
//#define gamma 6.
#define spot_brightness 1.01
#define ray_density 5.5
//#define curvature 200.
//#define red   1.
//#define green 3.
//#define blue  .5
#define noisetype 1
#define sin_freq 6.
#define YO_DAWG


#define PROCEDURAL_NOISE
#define YO_DAWG


float hash( float n ){return fract(sin(n)*43758.5453);}

float noise( in vec2 x )
{
  #ifdef PROCEDURAL_NOISE
  x *= 1.75;
    vec2 p = floor(x);
    vec2 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0;

    float res = mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                    mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
    return res;
  #else
  return texture(iChannel0, x*.01).x;
  #endif
}

mat2 m2 = mat2( 0.80,  0.60, -0.60,  0.80 );
float fbm( in vec2 p )
{ 
  float z=2.;
  float rz = 0.;
  p *= 0.25;
  for (float i= 1.;i < 6.;i++ )
  {
    #if noisetype == 1
    rz+= abs((noise(p)-0.5)*2.)/z;
    #elif noisetype == 2
    rz+= (sin(noise(p)*sin_freq)*0.5+0.5) /z;
    #else
    rz+= noise(p)/z;
    #endif
    z = z*2.;
    p = p*2.*m2;
  }
  return rz;
}

uniform float brightness;
uniform float ray_brightness;// 1 - 8;
uniform float gamma;// control white brightness 0.1 - 9
uniform float curvature;// depth of hole 10 - 200;

uniform float colorChangerR;
uniform float colorChangerG;
uniform float colorChangerB;
uniform float rms;


// uniform float iTime;
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{

    //float brightness = 5.;    // 0.001 - 5. use this and next ray_brightness to control global light ....
    // float ray_brightness = 8.; // 1 - 8;
    // float gamma = 6.; // control white brightness 0.1 - 9
    
    // float curvature = 150.; // depth of hole 10 - 200;

    

  float t = -iTime*0.0312;
  vec2 uv = fragCoord.xy / iResolution.xy-0.5;
  uv.x *= iResolution.x/iResolution.y;
  uv*= curvature*.05+0.0001;
  
  float r  = sqrt(dot(uv,uv));
  float x = dot(normalize(uv), vec2(.5,0.)) + t;  
  float y = dot(normalize(uv), vec2(.0,.5)) + t;
  
  #ifdef YO_DAWG
  x = fbm(vec2(y*ray_density*0.5,r+x*ray_density*.2));
  y = fbm(vec2(r+y*ray_density*0.1,x*ray_density*.5));
  #endif
  
    float val;
    val = fbm(vec2(r+y*ray_density,r+x*ray_density-y));
    val = smoothstep(gamma*.02-.1,ray_brightness+(gamma*0.02-.1)+.001,val);
    val = sqrt(val);
      
    float red  = 1.;
    float green = 3.;
    float blue  = .5;
    
    
    //float red_fact = 1.;
    //float green_fact = 1.;
    //float blue_fact = 1.;
    
   // red *= red_fact;
   // green *= green_fact;
   // blue *= blue_fact;
   
    float colorChangerR = .20853;
    float colorChangerG = .10523;

    colorChangerR *= 1. + fract(rms);
    if(fract(rms) > 0.5){
        colorChangerG *= 1. - fract(rms);

    }else{
        colorChangerG *= 1. + fract(rms);

    }
        
    float my_r = sqrt(x*x + y*y) * 5. * (cos(iTime * colorChangerR) + 1.) * .5; ///
    float r_fac = my_r * 3.1; ///
    float b_fac = my_r * 0.7; ///
    float g_fac = sqrt(x*x + y*y) * 5. * (cos(iTime * colorChangerG) + 1.) * .2;
    vec3 col = val/vec3(red * r_fac,green * g_fac,blue * b_fac); ///
    
    
  //vec3 col = val/vec3(red,green,blue);
  col = clamp(1.-col,0.,1.);
  col = mix(col,vec3(1.),spot_brightness-r/0.1/curvature*200./brightness);
  
  fragColor = vec4(col,1.0);
}