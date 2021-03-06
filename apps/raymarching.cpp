//
//  raymarching.cpp
//  apps
//
//  Created by 杨丰 on 2020/11/10.
//

#include "svpng.inc"
#include "../src.tracing/parallel.h"
#include <math.h>
#include <stdlib.h>
#define W 512
#define H 512

unsigned char img[W * H * 3];

#define TWO_PI 6.28318530718f
#define N 2048
#define MAX_DEPTH 3
#define MAX_STEP 64
#define MAX_DISTANCE 5.0f
#define EPSILON 1e-6f
#define BIAS 1e-4f

#define BLACK {0.0f, 0.0f, 0.0f}
typedef struct{float r, g, b; } Color;
Color colorAdd(Color a, Color b){
    Color c = {a.r+b.r, a.g+b.g, a.b+b.b};
    return c;
}
Color colorMultiply(Color a, Color b){
    Color c = {a.r*b.r, a.g*b.g, a.b*b.b};
    return c;
}
Color colorScale(Color a, float s){
    Color c = {a.r*s, a.g*s, a.b*s};
    return c;
}


typedef struct{
    float sd, reflectivity, eta;
    Color emissive, absorption;
} Result;
Result unionOp(Result a, Result b){
    return a.sd < b.sd ? a:b;
}
Result intersectOp(Result a, Result b){
    Result r = a.sd > b.sd ? b : a;
    r.sd = a.sd > b.sd ? a.sd : b.sd;
    return r;
}
Result subtractOp(Result a, Result b){
    Result r = a;
    r.sd = (a.sd > -b.sd) ? a.sd : -b.sd;
    return r;
}


void reflect(float ix, float iy, float nx, float ny, float *rx, float *ry){
    float idotn2 = (ix*nx + iy*ny) * 2.0f;
    *rx = ix - idotn2 * nx;
    *ry = iy - idotn2 * ny;
}
int refract(float ix, float iy, float nx, float ny, float eta, float *rx, float *ry){
    float idotn = ix*nx + iy*ny;
    float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
    if (k < 0.0f) {
        return 0;
    }
    float a = eta * idotn + sqrtf(k);
    *rx = eta * ix - a * nx;
    *ry = eta * iy - a * ny;
    return 1;
}
float fresnel(float cosi, float cost, float etai, float etat){
    float rs = (etat*cosi - etai*cost) / (etat*cosi + etai*cost);
    float rp = (etai*cosi - etat*cost) / (etai*cosi + etat*cost);
    return (rs*rs + rp*rp)*0.5f;
}
Color beerLambert(Color a, float d){
    Color c = {expf(-a.r*d), expf(-a.g*d), expf(-a.b*d)};
    return c;
}


//define constructive solid geometry-------------------------------------------
float circleSDF(float x, float y, float cx, float cy, float r) {
    float ux = x - cx, uy = y - cy;
    return sqrtf(ux * ux + uy * uy) - r;
}
float planeSDF(float x, float y, float px, float py, float nx, float ny) {
    return (x - px) * nx + (y - py) * ny;
}
float ngonSDF(float x, float y, float cx, float cy, float r, float n) {
    float ux = x - cx, uy = y - cy, a = TWO_PI / n;
    float t = fmodf(atan2f(uy, ux) + TWO_PI, a), s = sqrtf(ux * ux + uy * uy);
    return planeSDF(s * cosf(t), s * sinf(t), r, 0.0f, cosf(a * 0.5f), sinf(a * 0.5f));
}
Result scene(float x, float y){
    Result a = {circleSDF(x, y, 0.4f, 0.5f, 0.2f), 0.0f, 1.5f, BLACK, {4.0f, 4.0f, 10.0f} };
    Result b = {circleSDF(x, y, 0.6f, 0.5f, 0.2f), 0.0f, 1.5f, BLACK, {4.0f, 4.0f, 10.0f} };
    Result light_source = { circleSDF(x, y, -0.2f, 0.5f, 0.1f), 0.0f, 0.0f, {10.0f, 10.0f, 10.0f}, BLACK};

//    return unionOp(a, b);
//    return intersectOp(a, b);
//    return subtractOp(a, b);
    return unionOp(light_source, subtractOp(b, a));
}
//Result scene(float x, float y) {
//    Result a = { circleSDF(x, y, 0.5f, -0.2f, 0.1f), 0.0f, 0.0f, { 10.0f, 10.0f, 10.0f }, BLACK };
//    Result b = {   ngonSDF(x, y, 0.5f, 0.5f, 0.25f, 5.0f), 0.0f, 1.5f, BLACK, { 4.0f, 4.0f, 1.0f} };
//    return unionOp(a, b);
//}
//--------------------------------------------------------------------------------

void gradient(float x, float y, float *nx, float *ny){
    *nx = (scene(x+EPSILON, y).sd - scene(x-EPSILON, y).sd) * (0.5f/EPSILON);
    *ny = (scene(x, y+EPSILON).sd - scene(x, y-EPSILON).sd) * (0.5f/EPSILON);
}



Color trace(float ox, float oy, float dx, float dy, int depth) {
    float t = 1.e-3f;
    float sign = scene(ox, oy).sd > 0.0f ? 1.0f : -1.0f;
    for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; i++) {
        float x = ox + dx*t, y = oy + dy * t;
        Result r = scene(x, y);
        if (r.sd * sign< EPSILON){
            Color sum = r.emissive;
            if (depth < MAX_DEPTH && r.eta > 0.0f) {
                float nx, ny, rx, ry, refl = r.reflectivity;
                gradient(x, y, &nx, &ny);
                float s = 1.0f / (nx * nx + ny * ny);
                nx *= sign * s;
                ny *= sign * s;
                if (r.eta > 0.0f) {
                    if (refract(dx, dy, nx, ny, sign<0.0f ? r.eta : 1.0f/r.eta, &rx, &ry)) {
                        float cosi = -(dx*nx + dy*ny);
                        float cost = -(rx*nx + ry*ny);
                        refl = sign < 0.0f? fresnel(cosi, cost, r.eta, 1.0f) : fresnel(cosi, cost, 1.0f, r.eta);
                        refl = fmaxf(fminf(refl, 1.0f), 0.0f);
                        sum = colorAdd(sum, colorScale(trace(x - nx*BIAS, y - ny*BIAS, rx, ry, depth+1), 1.0f-refl));
                    }else{
                        refl = 1.0f;
                    }
                }
                if (refl > 0.0f) {
                    reflect(dx, dy, nx, ny, &rx, &ry);
                    sum = colorAdd(sum, colorScale(trace(x+ nx*BIAS, y+ ny*BIAS, rx, ry, depth+1), refl));
                }
            }
            return colorMultiply(sum, beerLambert(r.absorption, t));
        }
        t += r.sd * sign;
    }
    Color black = BLACK;
    return black;
}

Color sample(float x, float y) {
    Color sum = BLACK;
    std::vector<Color> colors(N);
    parallelFor(0, N, [&](size_t i){
        //        float a = TWO_PI * rand() / RAND_MAX;
        //        float a = TWO_PI * i/N;
        float a = TWO_PI * (i + (float)rand()/RAND_MAX)/N;
        colors[i] = trace(x, y, cosf(a), sinf(a), 0);
    }, ExecutionPolicy::kParallel);
    for (const auto& color : colors) {
        sum = colorAdd(sum, color);
    }    
    return colorScale(sum, 1.0f/N);
}

int main() {
    unsigned char* p = img;
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++, p += 3){
            Color c = sample((float)x/W, (float)y/H);
            p[0] = (int)fminf(c.r * 255.0f, 255.0f);
            p[1] = (int)fminf(c.g * 255.0f, 255.0f);
            p[2] = (int)fminf(c.b * 255.0f, 255.0f);
        }
    svpng(fopen("basic.png", "wb"), W, H, img, 0);
}
