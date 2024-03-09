uniform sampler2D image;
uniform float transparency;

void main() {
	gl_FragColor = vec4(texture(image, vec2(gl_TexCoord[0].xy)).rgb, transparency);
}