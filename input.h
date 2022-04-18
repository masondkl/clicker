#ifndef INPUT_H
#define INPUT_H

void create_hooks(
    void (*on_left_down)(), 
    void (*on_left_up)(), 
    void (*on_toggle)(),
    void (*on_increment)(),
    void (*on_decrement)()
);

#endif