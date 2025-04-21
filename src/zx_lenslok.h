// [ref] https://www.youtube.com/watch?v=GN_vPGQ4BNM (how to)

void lenslok(Tigr *dest, Tigr *src) {
    // Based on LensKey - a Lenslok Emulator
    // (c) 2002-2008 Simon Owen <simon@simonowen.com>
    // https://simonowen.com/spectrum/lenskey/

    enum { STRIPS = 16       };   // 16 vertical strips in the lens viewer
    enum { STRIP_HEIGHT = 32 };   // Sample strips taken are stored as 32-pixels high

    // 16 strip values: 6 for each character, with a strip of padding around each character
    // The values indicate the position a strip is taken from, as a percentage of the selection width
    // Negative values are to the left of the selection, and positive to the right
    // Zero is a special case and will be drawn as background colour to leave a gap
    struct lenslok_t {
        const char *title;
        const double panDecode[16];
    } lenslok[] = {
        {"ACE (Air Combat Emulator)",   { 0, -0.81, -0.31,  0.13, -0.62, -0.41,  0.22, 0,  0, -0.22,  0.39, 0.58, -0.12, 0.29, 0.70, 0 }},
        {"Art Studio",                  { 0, -0.41, -0.30, -0.68, -0.52, -0.11, -0.20, 0,  0,  0.32,  0.60, 0.11,  0.22, 0.49, 0.71, 0 }},
        {"Elite",                       { 0, -0.41, -0.57, -0.77,  0.10, -0.28, -0.19, 0,  0,  0.43, -0.10, 0.22,  0.32, 0.77, 0.58, 0 }},
        {"Graphic Adventure Creator",   { 0, -0.77, -0.28, -0.04, -0.19, -0.59, -0.39, 0,  0,  0.20,  0.51, 0.10,  0.10, 0.66, 0.28, 0 }},
        {"Jewels of Darkness",          { 0, -0.40, -0.57, -0.71,  0.14, -0.27, -0.21, 0,  0,  0.42, -0.12, 0.22,  0.27, 0.67, 0.53, 0 }},
        {"Mooncresta",                  { 0, -0.79, -0.31, -0.07, -0.22, -0.61, -0.44, 0,  0,  0.18,  0.50, 0.07,  0.67, 0.39, 0.27, 0 }},
        {"Price of Magik",              { 0, -0.27, -0.39, -0.71, -0.06, -0.17, -0.48, 0,  0,  0.51,  0.64, 0.07,  0.40, 0.17, 0.79, 0 }},
        {"Tomahawk",                    { 0, -0.82, -0.31, -0.58, -0.20, -0.42,  0.10, 0,  0, -0.10,  0.32, 0.65,  0.20, 0.44, 0.80, 0 }},
        {"TT Racer",                    { 0, -0.20, -0.41, -0.69, -0.53,  0.06, -0.29, 0,  0, -0.09,  0.64, 0.20,  0.46, 0.33, 0.81, 0 }},
    };

    // Fixed rect dimensions: width is fixed, height can vary.
    static int _120 = 58; int _60 = _120/2, XSCALE = _120/STRIPS;
#if DEV
    // calibration
    if( key_pressed(TK_RIGHT) )
        printf("%d\n", _120 = tigrMax((_120 + 1)%480, STRIPS));
#endif

    // Positioning
    int srcX = mouse().x, srcY = mouse().y;
    int dstX = srcX, dstY = srcY;

    // Iterate all lenslok devices, arranged in 3x3 grid
    for( int game = 0; game < countof(lenslok); ++game ) {
        const double* panDecode = lenslok[game].panDecode;

        // Draw a marquee rect
        int ui_rect(window *ui_layer, int x, int y, int x2, int y2);
        ui_rect(dest, dstX-1, dstY-1, dstX-1 +_120+1, dstY-1 +_60+1);
        tigrFillRect(dest, dstX-1,dstY-1, 1+_120+1,1+_60+1, tigrRGB(0,0,0));

        // Decode the image by taking vertical strips at the appropriate positions
        for( int x = 0 ; x < STRIPS ; x++ ) {
            // Work out the pixel offset, using an offset off the end for zero (spacing gap)
            int nOffset = _120 * panDecode[x];

            // Copy a slice to our memory bitmap
            if( panDecode[x] )
                tigrBlit(dest,src, dstX+x*XSCALE,dstY+0, srcX+nOffset,srcY, XSCALE,_60);
        }

        // Arrange next item in 3x3 grid
        dstX += (game % 3)  < 2 ? _120 : _120 * -2;
        dstY += (game % 3) == 2 ? _60 : 0;
    }
}
