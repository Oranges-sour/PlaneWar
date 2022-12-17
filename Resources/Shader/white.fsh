#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;    //���ն�����ɫ�����ݵ�ֵ(����ֵ)

void main()
{
	vec4 c = texture2D(CC_Texture0, v_texCoord);


	//��ɫֵ�İٷֱ�
	float white = 0.95;
	float r = white * c.a;
	float g = white * c.a;
	float b = white * c.a;

	c.rgb = vec3(r,g,b);

	gl_FragColor = c;
}