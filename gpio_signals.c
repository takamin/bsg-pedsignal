#ifndef _WIN32
#include "gpio.h"
#include "gpio_port.h"
#else
#include "../Signal/signallib.h"
#endif

/* GPIO port objects */

#ifndef _WIN32
GPIO_PORT* GPIO_LIGHT_CAR_BLUE = 0;
GPIO_PORT* GPIO_LIGHT_CAR_YELLOW = 0;
GPIO_PORT* GPIO_LIGHT_CAR_RED = 0;
GPIO_PORT* GPIO_LIGHT_PED_WALK = 0;
GPIO_PORT* GPIO_LIGHT_PED_STOP = 0;
GPIO_PORT* GPIO_DISP_PED_PUSH = 0;
GPIO_PORT* GPIO_DISP_PED_WAIT = 0;
GPIO_PORT* GPIO_PED_BUTTON = 0;
#endif

void signal_setup(void)
{
#ifndef _WIN32
	gpio_init();
	GPIO_LIGHT_CAR_BLUE = gpio_port_output(2);
	GPIO_LIGHT_CAR_YELLOW = gpio_port_output(3);
	GPIO_LIGHT_CAR_RED = gpio_port_output(4);
	GPIO_LIGHT_PED_WALK = gpio_port_output(17);
	GPIO_LIGHT_PED_STOP = gpio_port_output(18);
	GPIO_DISP_PED_PUSH = gpio_port_output(27);
	GPIO_DISP_PED_WAIT = gpio_port_output(22);
	GPIO_PED_BUTTON = gpio_port_input_pullup(24);
#else
#endif
}
void signal_shutdown(void)
{
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
	gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
	gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
	gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
	gpio_port_write(GPIO_DISP_PED_WAIT, 0);
#else
	SetRoadwaySignalColor(RSCBlack);
	SetCrosswalkSignalColor(CSCBlack);
	SetPedestrianLamp(PLOff);
#endif

}
/* signal control routine */

void signal_vehicle_blue(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 1);
    gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
    gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
#else
	SetRoadwaySignalColor(RSCBlue);
#endif
}
void signal_vehicle_yellow(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
    gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 1);
    gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
#else
	SetRoadwaySignalColor(RSCYellow);
#endif
}
void signal_vehicle_red(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
    gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
    gpio_port_write(GPIO_LIGHT_CAR_RED, 1);
#else
	SetRoadwaySignalColor(RSCRed);
#endif
}
void signal_pedestrian_blue(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_PED_WALK, 1);
    gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
#else
	SetCrosswalkSignalColor(CSCBlue);
#endif
}

void signal_pedestrian_off(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
    gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
#else
	SetCrosswalkSignalColor(CSCBlack);
#endif
}
void signal_pedestrian_red(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
    gpio_port_write(GPIO_LIGHT_PED_STOP, 1);
#else
	SetCrosswalkSignalColor(CSCRed);
#endif
}
void message_push(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_DISP_PED_PUSH, 1);
    gpio_port_write(GPIO_DISP_PED_WAIT, 0);
#else
	SetPedestrianLamp(PLPushButton);
#endif
}
void message_wait(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
    gpio_port_write(GPIO_DISP_PED_WAIT, 1);
#else
	SetPedestrianLamp(PLWait);
#endif
}
void message_off(void) {
#ifndef _WIN32
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
    gpio_port_write(GPIO_DISP_PED_WAIT, 0);
#else
	SetPedestrianLamp(PLOff);
#endif
}
#ifdef _WIN32
int button_pushed = 0;
#endif
int is_button_pushed(void) {
#ifndef _WIN32
	int button_state = gpio_port_read(GPIO_PED_BUTTON);
    int pushed = (button_state == 0 ? 1 : 0);
    return pushed;
#else
	if (button_pushed) {
		button_pushed = 0;
		return 1;
	}
	return 0;
#endif
}
