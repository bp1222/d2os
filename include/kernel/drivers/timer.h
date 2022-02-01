#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct {
    void (*evt)();    
} kernel_timer_device_t;

void timer_init(kernel_timer_device_t *dev);

#endif