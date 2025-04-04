//#include "tigr.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH _320
#define WINDOW_HEIGHT _240

#if 0
#define tigrMin(a,b) ((a) < (b) ? (a) : (b))
#define tigrMax(a,b) ((a) > (b) ? (a) : (b))
#define tigrClamp(v,a,b) ((v) < (a) ? (a) : (v) > (b) ? (b) : (v))
#else
double tigrMin(double a, double b) { return a < b ? a : b; }
double tigrMax(double a, double b) { return a > b ? a : b; }
double tigrClamp(double v, double m, double M) { return v<m ? m : v>M ? M : v; }
#endif

// Helper function to calculate cubic interpolation weight
float cubicWeight(float x) {
    x = x < 0 ? -x : x; // absolute value
    
    // Cubic interpolation kernel
    float x2 = x * x;
    float x3 = x2 * x;
    
    if (x < 1) {
        return 1.5 * x3 - 2.5 * x2 + 1.0;
    } else if (x < 2) {
        return -0.5 * x3 + 2.5 * x2 - 4.0 * x + 2.0;
    } else {
        return 0.0;
    }
}
// Bicubic sampler
TPixel bicubicSample(Tigr* src, float x, float y) {
    // Get integer and fractional parts of coordinates
    int ix = (int)x;
    int iy = (int)y;
    float fx = x - ix;
    float fy = y - iy;
    
    // Result color components
    float r = 0, g = 0, b = 0, a = 0;
    
    // For bicubic interpolation, we need to sample a 4x4 neighborhood
    for (int offY = -1; offY <= 2; offY++) {
        for (int offX = -1; offX <= 2; offX++) {
            // Get sample coordinates with boundary checking
            int sampleX = ix + offX;
            int sampleY = iy + offY;
            
            // Clamp to image boundaries
            sampleX = sampleX < 0 ? 0 : (sampleX >= src->w ? src->w - 1 : sampleX);
            sampleY = sampleY < 0 ? 0 : (sampleY >= src->h ? src->h - 1 : sampleY);
            
            // Get pixel at sampled position
            TPixel p = src->pix[sampleY * src->w + sampleX];
            
            // Calculate bicubic weight for this pixel
            float wx = cubicWeight(offX - fx);
            float wy = cubicWeight(offY - fy);
            float weight = wx * wy;
            
            // Accumulate weighted color components
            r += p.r * weight;
            g += p.g * weight;
            b += p.b * weight;
            a += p.a * weight;
        }
    }
    
    // Clamp results to valid range
    r = r < 0 ? 0 : (r > 255 ? 255 : r);
    g = g < 0 ? 0 : (g > 255 ? 255 : g);
    b = b < 0 ? 0 : (b > 255 ? 255 : b);
    a = a < 0 ? 0 : (a > 255 ? 255 : a);
    
    // Return interpolated pixel
    TPixel result;
    result.r = (unsigned char)r;
    result.g = (unsigned char)g;
    result.b = (unsigned char)b;
    result.a = (unsigned char)a;
    return result;
}

void scaleBlit(Tigr* dest, Tigr* src, float centerX, float centerY, float scale) {
    for (int y = 0; y < dest->h; y++) {
        for (int x = 0; x < dest->w; x++) {
            float srcX = centerX + (x - dest->w / 2.0f) * scale;
            float srcY = centerY + (y - dest->h / 2.0f) * scale;
            
            if (srcX >= 0 && srcX < src->w && srcY >= 0 && srcY < src->h) {
                // Usar muestreo adaptativo según el nivel de zoom
                dest->pix[y * dest->w + x] = bicubicSample(src, srcX, srcY);
            }
        }
    }
}

void tigrRenderMap(Tigr *screen, Tigr *map, int mx, int my, int buttons, float wheel) {
    static int   dragging, lastX, lastY;
    static float zoom, minZoom, maxZoom;
    static float centerX, centerY;
    
    // init if needed
    static int once = 1; if (!screen && !map) { once = 1; return; }
    if (once) {
        once = 0;
        
        dragging = 0;
        lastX = 0;
        lastY = 0;
        
        minZoom = 1.0f;
        maxZoom = tigrMax(map->w, map->h);
        
        zoom = minZoom;
        centerX = map->w / 2.0f;
        centerY = map->h / 2.0f;
        
        printf("MAP INIT: map dimensions: %dx%d center(%f,%f)\n", map->w, map->h, centerX, centerY);
    }
    
    // Mouse panning
    if (buttons & 1) {
        if (!dragging) {
            dragging = 1;
            lastX = mx;
            lastY = my;
        } else {
            float scale = (float)tigrMax(map->w, map->h) / (WINDOW_WIDTH * zoom);
            float dx = (mx - lastX) * scale;
            float dy = (my - lastY) * scale;
            
            centerX -= dx;
            centerY -= dy;
            
            lastX = mx;
            lastY = my;
        }
    } else {
        dragging = 0;
    }
    
    // Mouse Zooming
    if (wheel != 0) {
        float oldScale = (float)tigrMax(map->w, map->h) / (WINDOW_WIDTH * zoom);
        float mouseMapX = centerX + (mx - WINDOW_WIDTH / 2.0f) * oldScale;
        float mouseMapY = centerY + (my - WINDOW_HEIGHT / 2.0f) * oldScale;
        
        float zoomFactor = (wheel > 0) ? 1.05f : (1.0f / 1.05f);
        zoom = tigrClamp(zoom * zoomFactor, minZoom, maxZoom);
        
        float newScale = (float)tigrMax(map->w, map->h) / (WINDOW_WIDTH * zoom);
        centerX = mouseMapX - (mx - WINDOW_WIDTH / 2.0f) * newScale;
        centerY = mouseMapY - (my - WINDOW_HEIGHT / 2.0f) * newScale;
    }
    
    // Calc view limits
    float scale = (float)tigrMax(map->w, map->h) / (WINDOW_WIDTH * zoom);
    float viewWidth = WINDOW_WIDTH * scale;
    float viewHeight = WINDOW_HEIGHT * scale;
    
    // Modify how limits are calculated to avoid discrepancies
    float minX = viewWidth / 2.0f;
    float maxX = map->w - viewWidth / 2.0f;
    float minY = viewHeight / 2.0f;
    float maxY = map->h - viewHeight / 2.0f;
    
    // Ensure minX <= maxX y minY <= maxY, so centerY does not jitter
    if (minX > maxX) minX = maxX = map->w / 2.0f;
    if (minY > maxY) minY = maxY = map->h / 2.0f;
    
    // Apply limits
    centerX = tigrClamp(centerX, minX, maxX);
    centerY = tigrClamp(centerY, minY, maxY);
    
    // Render using exact coords
    scaleBlit(screen, map, centerX, centerY, scale);
    
    // Cross-hair
    enum { _10 = 3 };
    TPixel red = tigrRGB(255, 255, 240);
    tigrLine(screen, mx - _10, my, mx + _10 + 1, my, red);
    tigrLine(screen, mx, my - _10, mx, my + _10 + 1, red);
}

void tigrRenderInitMap(void) {
    tigrRenderMap(NULL, NULL, 0, 0, 0, 0);
}

/*
int main() {
    Tigr* screen = tigrWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Map Demo", 0);
    if (!screen) {
        printf("Failed to create window\n");
        return 1;
    }

    // give non 1:1 aspect ratio maps a try
    enum { MAP_W = 2048, MAP_H = 2048 };
    Tigr* map = tigrBitmap(MAP_W*2, MAP_H);
    if (!map) {
        printf("Failed to create map buffer\n");
        return 1;
    }

    tigrClear(map, tigrRGB(200, 200, 200));
    for (int y = 0; y < map->h; y += 64) {
        for (int x = 0; x < map->w; x += 64) {
            TPixel col = tigrRGB(rand() % 100 + 100, rand() % 100 + 100, rand() % 100 + 100);
            tigrFillRect(map, x, y, 62, 62, col);
            tigrRect(map, x, y, 64, 64, tigrRGB(0, 0, 0));
        }
    }

    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        tigrClear(screen, tigrRGB(0, 0, 0));

        int mx, my, buttons;
        tigrMouse(screen, &mx, &my, &buttons);
        float wheel = tigrMouseWheel(screen);

        tigrRenderMap(screen, map, mx, my, buttons, wheel);
        tigrUpdate(screen);
    }

    tigrFree(map);
    tigrFree(screen);
    return 0;
}
*/
