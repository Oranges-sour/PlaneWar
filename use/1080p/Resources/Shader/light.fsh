#ifdef GL_ES
precision mediump float;//�еȾ���
#endif

varying vec2 v_texCoord;    //���ն�����ɫ�����ݵ�ֵ(����ֵ)

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