#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormWorld;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUBO{
	mat4 projectionViewMatrix;
	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
}ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

void main()
{
	vec3 directionToLight = ubo.lightPosition - fragPosWorld;
	float attenuation = 1.0 / dot(directionToLight, directionToLight);
	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 diffuseLight = lightColor * max(dot(normalize(fragNormWorld), normalize(directionToLight)), 0);
	outColor = vec4((diffuseLight + ambientLight) * fragColor,1.0);
}

//#version 450
//
//layout (location = 0) in vec2 in_uv;
//
//layout (location = 0) out vec4 color;
//
//layout (set = 0, binding = 0) uniform Parameters {
//	int max_iter;
//	float size;
//	float time;
//	float factor;
//	float width;
//	float height;
//} parameters;
//
//float hue_to_rgb(float p, float q, float t) {
//	if (t < 0.0) {
//		t += 1.0;
//	}
//
//	if (t > 1.0) {
//		t -= 1.0;
//	}
//
//	if (t < (1.0 / 6.0)) {
//		return p + (q - p) * 6.0 * t;
//	} else if (t < (1.0 / 2.0)) {
//		return q;
//	} else if (t < (2.0 / 3.0)) {
//		return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
//	}
//
//	return p;
//}
//
//vec3 hsl_to_rgb(float h, float s, float l) {
//	float r, g, b;
//
//	if (s == 0.0) {
//		r = l;
//		g = l;
//		b = l;
//	} else {
//		float q, p;
//
//		if (l < 0.5) {
//			q = l * (1. + s);
//		} else {
//			q = l + s - l * s;
//		}
//
//		p = 2. * l - q;
//		r = hue_to_rgb(p, q, h + 1.0 / 3.0);
//		g = hue_to_rgb(p, q, h);
//		b = hue_to_rgb(p, q, h - 1.0 / 3.0);
//	}
//
//	return vec3(r, g, b);
//}
//
//void mandelbrot_iter(float px, float py, out float ox, out int iter) {
//    float x = 0.0;
//    float y = 0.0;
//    float xx = 0.0;
//    float yy = 0.0;
//    float xy = 0.0;
//
//    for (int i = 0; i < parameters.max_iter; i++) {
//        xx = x * x;
//        yy = y * y;
//        xy = x * y;
//
//        if ((xx + yy) > 4.0) {
//            ox = xx + yy;
//            iter = i;
//
//            return;
//        }
//
//        x = xx - yy + px;
//        y = 2.0 * xy + py;
//    }
//
//    ox = xx + yy;
//    iter = parameters.max_iter;
//}
//
//vec3 paint(float nx, float ny) {
//    float r;
//    int n;
//
//    mandelbrot_iter(parameters.size * nx, parameters.size * ny, r, n);
//
//    if (r > 4.0) {
//        return hsl_to_rgb(float(n) / 800.0 * r, 1.0, 0.5);
//    } else {
//        return vec3(1.0, 1.0, 1.0);
//    }
//}
//
//// void main() {
////     color = vec4(paint(gl_FragCoord.x, gl_FragCoord.y), 1.0);
//// }
//
//vec2 square_imaginary(vec2 number) {
//	return vec2(
//		pow(number.x, 2) - pow(number.y, 2),
//		2 * number.x * number.y
//	);
//}
//
//float iterate_mandelbrot(vec2 coord) {
//	vec2 z = vec2(0,0);
//
//	for(int i = 0; i < parameters.max_iter; i++){
//		z = square_imaginary(z) + coord;
//
//		if ( length(z) > 2 ) {
//			return i / parameters.max_iter;
//		}
//	}
//
//	return parameters.max_iter;
//}
//
//void main() {
//	vec2 coord = gl_FragCoord.xy / vec2(parameters.width, parameters.height);
//
//	float c = iterate_mandelbrot(coord);
//
//	color = vec4(vec3(c, c * 0.3, c * 0.7), 1.0);
//
//	// vec2 tc = vec2(in_uv.x) * parameters.time * parameters.factor;
//
//    // float x0 = (tc.x * 3 - 2);
//    // float y0 = (tc.y * 2 - 1);
//
//    // float x = 0;
//    // float y = 0;
//    // int iteration = 0;
//
//    // while ((x * x + y * y <= 4) && (iteration < parameters.max_iter))
//    // {
//    // 	float xtemp = x * x - y * y + x0;
//    // 	y = 2 * x * y + y0;
//    // 	x = xtemp;
//
//    // 	iteration += 1;
//    // }
//
//    // if (iteration == parameters.max_iter)
//    // {
//
//	//   	// Hier wird der Farbwert f�r das Innere der Mandelbrotmenge gesetzt
//
//    // 	color = vec4(0, 0, 0, 1);
//    // }
//    // else
//    // {
//
//	// 	// Hier kann eine beliebige Farbwahl stattfinden(z.B. mit einer Farbtabelle).
//	// 	// als Index verwendet man die Variable "iteration", die einen Wert zwischen 1 und parameters.max_iter hat.
//
//    // 	x = sin(float(iteration));
//	// 	color = vec4(x, x, x, 1);
//    // }
//}