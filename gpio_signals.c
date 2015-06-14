#include "gpio.h"
#include "gpio_port.h"

/* GPIO port objects */

GPIO_PORT* GPIO_LIGHT_CAR_BLUE = 0;
GPIO_PORT* GPIO_LIGHT_CAR_YELLOW = 0;
GPIO_PORT* GPIO_LIGHT_CAR_RED = 0;
GPIO_PORT* GPIO_LIGHT_PED_WALK = 0;
GPIO_PORT* GPIO_LIGHT_PED_STOP = 0;
GPIO_PORT* GPIO_DISP_PED_PUSH = 0;
GPIO_PORT* GPIO_DISP_PED_WAIT = 0;
GPIO_PORT* GPIO_PED_BUTTON = 0;

void signal_setup(void)
{
	gpio_init();
	GPIO_LIGHT_CAR_BLUE = gpio_port_output(2);
	GPIO_LIGHT_CAR_YELLOW = gpio_port_output(3);
	GPIO_LIGHT_CAR_RED = gpio_port_output(4);
	GPIO_LIGHT_PED_WALK = gpio_port_output(17);
	GPIO_LIGHT_PED_STOP = gpio_port_output(18);
	GPIO_DISP_PED_PUSH = gpio_port_output(27);
	GPIO_DISP_PED_WAIT = gpio_port_output(22);
	GPIO_PED_BUTTON = gpio_port_input_pullup(24);
}
void signal_shutdown(void)
{
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
	gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
	gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
	gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
	gpio_port_write(GPIO_DISP_PED_WAIT, 0);

}
/* signal control routine */

void signal_vehicle_blue(void) {
    gpio_port_write(GPIO_LIGHT_CAR_BLUE, 1);
    gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
    gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
}
void signal_vehicle_yellow(void) {
    gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
    gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 1);
    gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
}
void signal_vehicle_red(void) {
    gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
    gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
    gpio_port_write(GPIO_LIGHT_CAR_RED, 1);
}
void signal_pedestrian_blue(void) {
    gpio_port_write(GPIO_LIGHT_PED_WALK, 1);
    gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
}

void signal_pedestrian_off(void) {
    gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
    gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
}
void signal_pedestrian_red(void) {
    gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
    gpio_port_write(GPIO_LIGHT_PED_STOP, 1);
}
void message_push(void) {
    gpio_port_write(GPIO_DISP_PED_PUSH, 1);
    gpio_port_write(GPIO_DISP_PED_WAIT, 0);
}
void message_wait(void) {
    gpio_port_write(GPIO_DISP_PED_PUSH, 0);
    gpio_port_write(GPIO_DISP_PED_WAIT, 1);
}
void message_off(void) {
    gpio_port_write(GPIO_DISP_PED_PUSH, 0);
    gpio_port_write(GPIO_DISP_PED_WAIT, 0);
}
int is_button_pushed(void) {
    int button_state = gpio_port_read(GPIO_PED_BUTTON);
    int pushed = (button_state == 0 ? 1 : 0);
    return pushed;
}
