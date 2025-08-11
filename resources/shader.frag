uniform sampler2D texture; // Картинка игры
uniform float time;        // Время в секундах
uniform float noiseAlpha;  // Прозрачность шума (0..1)
uniform float scanAlpha;   // Прозрачность полос (0..1)
uniform float breathAmp;   // Амплитуда "дыхания" (0..1)
uniform float breathPeriod; // Период полного цикла (сек)

// Псевдослучай
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;

    // ===== 1. Лёгкое дрожание только иногда =====
    float shake = sin(time * 2.0) * 0.0003;
    if (mod(time, 23.0) < 0.25) { // редкий толчок
        shake *= 5.0;
    }
    uv.x += shake;
    uv.y += shake;

    // ===== 2. CRT color bleed =====
    float shift = 0.0015;
    float r = texture2D(texture, uv + vec2(shift, 0.0)).r;
    float g = texture2D(texture, uv).g;
    float b = texture2D(texture, uv - vec2(shift, 0.0)).b;
    vec4 crtColor = vec4(r, g, b, 1.0);

    // ===== 3. Шум =====
    float n = rand(gl_FragCoord.xy + time * 60.0);
    vec4 noise = vec4(vec3(n), 1.0);

    // ===== 4. Сканлайны =====
    float scan = sin(uv.y * 800.0) * 0.5 + 0.5;
    vec4 scanline = vec4(vec3(scan), 1.0);

    // ===== 5. Редкие всплески яркости =====
    float flash = 1.0;
    if (mod(time, 47.0) < 0.1) {
        flash = 1.15;
    }

    // ===== 6. Редкие мелкие помехи =====
    if (mod(time * 3.0, 71.0) < 0.05) {
        crtColor.rgb += vec3(0.12, 0.12, 0.12);
    }

    // ===== 7. Дыхание =====
    float breath = (sin((time / breathPeriod) * 6.2831853) * 0.5 + 0.5) * breathAmp;
    // breathAmp можно подавать в диапазоне 0..1, но можно и напрямую 0..75/255 для альфы
    // Здесь дыхание будет влиять на яркость
    crtColor.rgb *= (1.0 - breath);

    // ===== 8. Смешивание =====
    vec4 finalColor = mix(crtColor, noise, noiseAlpha);
    finalColor = mix(finalColor, scanline, scanAlpha);
    finalColor.rgb *= flash;

    gl_FragColor = finalColor;
}