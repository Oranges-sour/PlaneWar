#ifdef GL_ES
precision mediump float;//中等精度
#endif

varying vec2 v_texCoord;    //接收顶点着色器传递的值(接收值)

uniform float light;

void main()
{
	vec4 c = texture2D(CC_Texture0, v_texCoord);

	float r = light * c.r;
	float g = light * c.g;
	float b = light * c.b;

	c.rgb = vec3(r,g,b);

	gl_FragColor = c;
}