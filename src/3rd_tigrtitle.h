char* tigrTitle(Tigr *win, const char *title) {
    if( title ) {
#if DEV
    	puts(title);
#endif

#ifdef __APPLE__
        // @todo
#elif defined _WIN32
        static wchar_t bak[128];
        wchar_t *widen(const char *);
        _snwprintf(bak, 128-1, L"%s", widen(title));

        if(win->handle) {
            int err = SetWindowTextW((HWND)(win->handle), bak) == 0;
            RedrawWindow((HWND)(win->handle), NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
            //SendMessageW( (HWND)win->handle, WM_SETTEXT, 0, (LPARAM)bak );
        }
#else
        XTextProperty prop;
        int result = Xutf8TextListToTextProperty(dpy, (char**)&title, 1, XUTF8StringStyle, &prop);
        if (result == Success) {
            Atom wmName = XInternAtom(dpy, "_NET_WM_NAME", 0);
            XSetTextProperty(tigrInternal(win)->dpy, tigrInternal(win)->win, &prop, wmName);
            XFree(prop.value);
        }
#endif
    }
    static char copy[128] = {0};
    if( title ) snprintf(copy, 128-1, "%s", title);
    return copy;
}
