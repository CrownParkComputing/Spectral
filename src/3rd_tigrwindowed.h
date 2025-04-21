int tigrWindowed(Tigr *app) {
    return tigrInternal(app)->flags & TIGR_FULLSCREEN ? 0 : 1;
}

void tigrGetDesktop(int *width, int *height) {
    *width = 0;
    *height = 0;

#ifdef _WIN32
    // Windows
    *width = GetSystemMetrics(SM_CXSCREEN);
    *height = GetSystemMetrics(SM_CYSCREEN);
#elif defined(__linux__)
    // Linux with X11
    Display *display = XOpenDisplay(NULL);
    if (display) {
        Screen *screen = DefaultScreenOfDisplay(display);
        *width = screen->width;
        *height = screen->height;
        XCloseDisplay(display);
    }
#elif defined(__APPLE__)
    // macOS
    CGDirectDisplayID mainDisplay = CGMainDisplayID();
    *width = (int)CGDisplayPixelsWide(mainDisplay);
    *height = (int)CGDisplayPixelsHigh(mainDisplay);
#endif
}
