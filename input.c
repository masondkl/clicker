#ifndef INPUT_C
#define INPUT_C

#include "input.h"
#include <Windows.h>

typedef struct input_handle {
    void (*on_left_down)();
    void (*on_left_up)();
    void (*on_toggle)();
    void (*on_increment)();
    void (*on_decrement)();
    HHOOK mouse_callback, key_callback;
} INPUTHANDLE;
INPUTHANDLE *handle;
LRESULT CALLBACK mouse_listener(int code, WPARAM wp, LPARAM lp) {
    PMSLLHOOKSTRUCT hook = (PMSLLHOOKSTRUCT) lp;
    if (hook != NULL 
        && wp != WM_MOUSEMOVE 
        && hook->flags == 0
    ) { switch (wp) {
        case WM_LBUTTONDOWN: handle->on_left_down(); break;
        case WM_LBUTTONUP: handle->on_left_up(); break;
    } }
    return CallNextHookEx(handle->mouse_callback, code, wp, lp);
}
LRESULT CALLBACK key_listener(int code, WPARAM wp, LPARAM lp) {
    PKBDLLHOOKSTRUCT hook = (PKBDLLHOOKSTRUCT) lp;
    if (hook != NULL 
        && WM_KEYUP == wp
    ) { switch (hook->vkCode) {
        case VK_OEM_3: handle->on_toggle(); break;
        case VK_LEFT: handle->on_decrement(); break;
        case VK_RIGHT: handle->on_increment(); break;
    } }
    return CallNextHookEx(handle->key_callback, code, wp, lp);
}
DWORD WINAPI hook_thread(LPVOID lp) {
    handle->mouse_callback = SetWindowsHookEx(
        WH_MOUSE_LL, &mouse_listener, 
    NULL, 0);
    handle->key_callback = SetWindowsHookEx(
        WH_KEYBOARD_LL, &key_listener, 
    NULL, 0);
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) 
        { TranslateMessage(&message); DispatchMessage(&message); }
    UnhookWindowsHookEx(handle->mouse_callback); UnhookWindowsHookEx(handle->key_callback);
    free(handle);
    return 0;
}
void create_hooks(
    void (*on_left_down)(), 
    void (*on_left_up)(), 
    void (*on_toggle)(),
    void (*on_increment)(),
    void (*on_decrement)()
) {
    handle = malloc(sizeof(INPUTHANDLE));
    handle->on_left_down = on_left_down;
    handle->on_left_up = on_left_up;
    handle->on_toggle = on_toggle;
    handle->on_increment = on_increment;
    handle->on_decrement = on_decrement;
    CreateThread(NULL, 0, 
        (LPTHREAD_START_ROUTINE)&hook_thread, 
    NULL, 0, 0);
}

#endif