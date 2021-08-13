#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

//in values
uniform vec2 resolution;//图片大小

uniform float light;//色彩变亮

uniform float blurRadius;//发光半径

vec4 blur(vec2);
vec4 outGlow(vec4);

void main(void)
{
    vec4 col = blur(v_texCoord) * v_fragmentColor;
    gl_FragColor = outGlow(col);
}

vec4 blur(vec2 p)
{
	float sampleNum = blurRadius + 0.5;

    if (blurRadius > 0.0 && sampleNum > 1.0)
    {
        vec4 col = vec4(0);
        vec2 unit = 1.0 / resolution.xy;
        
        float r = blurRadius;
        float sampleStep = r / sampleNum;
        
        float count = 0.0;
        
        for(float x = -r; x < r; x += sampleStep)
        {
            for(float y = -r; y < r; y += sampleStep)
            {
                float weight = (r - abs(x)) * (r - abs(y));
                col += texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y)) * weight;
                count += weight;
            }
        }
        
        return col / count;
    }
    
    return texture2D(CC_Texture0, p);
}

vec4 outGlow(vec4 c2)
{
	vec4 result = c2;
	
	float r = light * result.r;
	float g = light * result.g;
	float b = light * result.b;

	result.rgb = vec3(r,g,b);
	

	vec4 temp = texture2D(CC_Texture0, v_texCoord);
	if(temp.a < 0.95)
	{
		return result;
	}

	return temp;
}