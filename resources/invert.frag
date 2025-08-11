uniform sampler2D texture; // SFML передаёт текущую картинку сюда

void main()
{
    vec4 c = texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor = vec4(1.0 - c.rgb, c.a);
}