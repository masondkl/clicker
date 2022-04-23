#include "input.h"
#include <stdio.h>
#include <Windows.h>
#include <synchapi.h>
#include <time.h>
#include <math.h>

const double PI = 3.1415926535897931;
const double PERLIN_SCALE = 5.0;
int toggled = 0, clicking = 0, state = 0, cps = 10;
long last = 0, delay = 0;

double random(long min, long max) {
    double 
        time = ((double)clock() / 5000.0),
        perlin = (sin(time/2.0) + sin(PI*time)) / 2.0;
    min += (perlin * PERLIN_SCALE); max += (perlin * PERLIN_SCALE);
    min *= 8.0 / (double)cps; max *= 8.0 / (double)cps;
    double random = (rand() % ((max+1) - min)) + min;
    return (random);
}
void on_left_down() { 
    clicking = 1;
    state = 0;
    delay = random(30, 60);
}
void on_left_up() {
    clicking = 0;
}
void on_toggle() {
    toggled = !toggled; 
    printf("%s\n", toggled ? "Enabled" : "Disabled"); 
}
void on_increment() {
    cps++;
    if (cps > 20) cps = 20;
    printf("CPS: %d\n", cps);
}
void on_decrement() {
    cps--;
    if (cps < 1) cps = 0;
    printf("CPS: %d\n", cps);
}
void main() {
    create_hooks(on_left_down, on_left_up, on_toggle, on_increment, on_decrement);
    while (1) {
        if (!toggled || !clicking) continue;
        long now = clock();
        srand(now);
        if (state && now - last > delay) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, NULL, NULL);
            delay = random(30, 60);
            last = now;
            state = !state;
        } else if (!state && now - last > delay) {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, NULL, NULL);
            delay = random(40, 70);
            last = now;
            state = !state;
        }
    }
    Sleep(INFINITE);
}