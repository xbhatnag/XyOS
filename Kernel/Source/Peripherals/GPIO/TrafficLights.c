#include "TrafficLights.h"
#include "GPIO.h"

void traffic_lights_init() {
    gpio_mode(9, GPIO_MODE_OUTPUT);
    gpio_mode(10, GPIO_MODE_OUTPUT);
    gpio_mode(11, GPIO_MODE_OUTPUT);
}

void traffic_light_red_on() {
    gpio_set(9, GPIO_LEVEL_HIGH);
}

void traffic_light_yellow_on() {
    gpio_set(10, GPIO_LEVEL_HIGH);
}

void traffic_light_green_on() {
    gpio_set(11, GPIO_LEVEL_HIGH);
}
