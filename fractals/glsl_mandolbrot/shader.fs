vec2 u_resolution;
uniform vec2 u_mouse;

uniform vec3 u_cameraPos;
uniform float u_time;



vec2 scaleX = vec2(-2.0,0.47);
vec2 scaleY = vec2(-1.12,1.12);
vec2 translate= vec2(0.0,0.0);

const int max_iteration = 255;
void zoom(float mag){
    scaleX = vec2(scaleX.x*mag,scaleX.y*mag);
    scaleY = vec2(scaleY.x*mag,scaleY.y*mag);
}
vec2 translate_pixel(float x0,float y0,vec3 coor){
    
    float Mx = x0-coor.x;
    float My = y0-coor.y;    
    return vec2(Mx,My);
}
float scale(float m, vec2 orig, vec2 new){
    return( ((m - orig.x)/(orig.y - orig.x)) * (new.y-new.x) + new.x);
}
int iterate(float x0, float y0){
	int iteration = 0;
    float x = 0.0;
    float y = 0.0;
    for(int i = 0; i<max_iteration;i++){
        float xtemp = (x*x) -(y*y) +x0;
        y = 2.0*x*y + y0;
        x = xtemp;
        if(x*x + y*y >4.0) return i;
    }
    return max_iteration;
}
float pallet(int iteration){
    if(iteration == max_iteration){
        return(0.0);
    }
    return (float(iteration)/float(max_iteration))+0.3;
}

void main() {
	u_resolution = vec2(800.0,600.0);
    // float time = sin(u_time);
    //zoom(-400/((u_cameraPos.z/5)-50));
	zoom(u_cameraPos.z);
    vec2 pixel = gl_FragCoord.xy;
    float x0 = scale(float(pixel.x),vec2(-1.0, u_resolution.x), scaleX);
    float y0 = scale(float(pixel.y), vec2(-1.0, u_resolution.y), scaleY);

	float translateX= scale(u_cameraPos.x,vec2(-1.0, u_resolution.x), scaleX);
	float translateY= scale(u_cameraPos.y, vec2(-1.0, u_resolution.y), scaleY);
    
    vec2 translate = translate_pixel(x0,y0,vec3(translateX,translateY,u_cameraPos.z));
    //zoom(0.5);
    
    x0=translate.x;
    y0=translate.y;
    
	int iteration = iterate(x0,y0);
    float color = pallet(iteration);

    gl_FragColor = vec4(0.0,0.0,color,1.0);
}