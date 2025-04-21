#include <stdbool.h>
void enable_os_keys( bool on );

#ifdef _WIN32

void enable_accessibility_keys( bool bAllowKeys ) {
    static STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};
    static TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
    static FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};
    static int once = 0;
    for(;!once;once=1) {
    // Save the current sticky/toggle/filter key settings so they can be restored them later
    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
    }

    if( bAllowKeys ) {
        // Restore StickyKeys/etc to original state and enable Windows key
        STICKYKEYS sk = g_StartupStickyKeys;
        TOGGLEKEYS tk = g_StartupToggleKeys;
        FILTERKEYS fk = g_StartupFilterKeys;
        
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
    }
    else {
        // Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
        // then leave the settings alone as its probably being usefully used
 
        STICKYKEYS skOff = g_StartupStickyKeys;
        if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 ) {
            // Disable the hotkey and the confirmation
            skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
            skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
            SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
        }
 
        TOGGLEKEYS tkOff = g_StartupToggleKeys;
        if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 ) {
            // Disable the hotkey and the confirmation
            tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
            tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
            SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
        }
 
        FILTERKEYS fkOff = g_StartupFilterKeys;
        if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 ) {
            // Disable the hotkey and the confirmation
            fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
            fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY; 
            SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
        }
    }
}

static HHOOK keyhook = NULL;
static
LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam ) {
    // Disable Win key. Note that this will not block the Xbox Game Bar hotkeys (Win+G, Win+Alt+R, etc.)
    if( nCode == HC_ACTION ) {
        if( wParam == WM_KEYDOWN || wParam == WM_KEYUP ) {
            KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*)(lParam);
            BOOL app_active = 1; // (GetActiveWindow() == hWnd);
            if( app_active ) {
                // check whether ALT/CTRL/SHIFT keys are pressed
                BOOL ALT_held = !!(p->flags & LLKHF_ALTDOWN); // !!(GetAsyncKeyState(VK_MENU) & 0x8000); //;
                BOOL CTRL_held = !!(GetAsyncKeyState(VK_CONTROL) & 0x8000);
                BOOL SHIFT_held = !!(GetAsyncKeyState(VK_LSHIFT) & 0x8000);
                int down = p->vkCode;

                // Disable CTRL+ESC (which opens the Start Menu)
                if( CTRL_held && VK_ESCAPE == down )
                    return 1;

                // Disable WIN (which opens the Start Menu and, when combined with other keys, performs various system actions)
                if( VK_LWIN == down || VK_RWIN == down )
                    return 1;

                // @fixme: following entries do not work

                // Disable ALT+ESC (which cycles through open windows in the order they were opened)
                if( ALT_held && VK_ESCAPE == down )
                    return 1;

                // Disable ALT+TAB (which opens the Task Switcher and allows cycling through open applications)
                if( ALT_held && VK_TAB == down )
                    return 1;

                // Disable CTRL+SHIFT (which switches the keyboard input language/layout)
                if( CTRL_held && VK_SHIFT == down )
                    return 1;
            }
        }
    }
    return CallNextHookEx( keyhook, nCode, wParam, lParam );
}

void enable_win_key( bool on ) {
    if(!on && !keyhook) keyhook = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0 );
    if( on &&  keyhook) UnhookWindowsHookEx( keyhook ), keyhook = NULL;
}

#endif

void enable_os_keys( bool on ) {
#ifdef _WIN32
//    enable_win_key(on);
    enable_accessibility_keys(on);
#endif
}
