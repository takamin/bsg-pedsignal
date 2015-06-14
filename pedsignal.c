/*
 * GPIO ports definition
 *
 * Signal for cars
 *
 * GPIO_2 - Blue
 * GPIO_3 - Yellow
 * GPIO_4 - Red
 *
 * Signal for pedestrian
 *
 * GPIO_14 - Walk
 * GPIO_15 - Stop
 *
 * Message LED for pedestrian
 *
 * GPIO_17 - Push the button to abroad
 * GPIO_18 - Wait till the pedestrian signal turn to 'Walk'
 */
#ifndef __MINGW32__
#include <syslog.h>
#endif

#include <signal.h>
#ifndef __MINGW32__
#include <sys/wait.h>
#endif
#include "gpio.h"
#include "gpio_port.h"
#include "statemac.h"

GPIO_PORT* GPIO_LIGHT_CAR_BLUE = 0;
GPIO_PORT* GPIO_LIGHT_CAR_YELLOW = 0;
GPIO_PORT* GPIO_LIGHT_CAR_RED = 0;
GPIO_PORT* GPIO_LIGHT_PED_WALK = 0;
GPIO_PORT* GPIO_LIGHT_PED_STOP = 0;
GPIO_PORT* GPIO_DISP_PED_PUSH = 0;
GPIO_PORT* GPIO_DISP_PED_WAIT = 0;
GPIO_PORT* GPIO_PED_BUTTON = 0;

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

#define BLINK_FREQ 2
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

                /* VEHICLE  PEDESTRIAN      */
#define T1 15   /* BLUE     RED             */
#define T2 5    /* YELLOW   RED             */
#define T3 20   /* RED      BLUE            */
#define T4 10   /* RED      BLINKING BLUE   */

#define MICROSEC_PER_SECONDS 1000000
int timer_count = 0;
void timer_increment(int uS)
{
    timer_count += uS;
}
void timer_reset()
{
    timer_count = 0;
}
int timer_is_timeup(int uS)
{
    if(timer_count > uS) {
        return 1;
    }
    return 0;
}
void sVB0_enter(struct transtate_t* transtate);
void sVB0_run(struct transtate_t* transtate);
void sVB0_leave(struct transtate_t* transtate);

void sVB1_enter(struct transtate_t* transtate);
void sVB1_run(struct transtate_t* transtate);
void sVB1_leave(struct transtate_t* transtate);

void sVY_enter(struct transtate_t* transtate);
void sVY_run(struct transtate_t* transtate);
void sVY_leave(struct transtate_t* transtate);

void sPB0_enter(struct transtate_t* transtate);
void sPB0_run(struct transtate_t* transtate);
void sPB0_leave(struct transtate_t* transtate);

void sPY0_enter(struct transtate_t* transtate);
void sPY0_run(struct transtate_t* transtate);
void sPY0_leave(struct transtate_t* transtate);

void sPY1_enter(struct transtate_t* transtate);
void sPY1_run(struct transtate_t* transtate);
void sPY1_leave(struct transtate_t* transtate);

TRANSTATE sVB0 = { 0, sVB0_enter, sVB0_run, sVB0_leave, };
TRANSTATE sVB1 = { 0, sVB1_enter, sVB1_run, sVB1_leave, };
TRANSTATE sVY  = { 0, sVY_enter, sVY_run, sVY_leave, };
TRANSTATE sPB0 = { 0, sPB0_enter, sPB0_run, sPB0_leave, };
TRANSTATE sPY0 = { 0, sPY0_enter, sPY0_run, sPY0_leave, };
TRANSTATE sPY1 = { 0, sPY1_enter, sPY1_run, sPY1_leave, };

STATEMAC state_machine;

void sVB0_enter(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "ENTER sVB0\n");
    timer_reset();
}
void sVB0_run(struct transtate_t* transtate)
{
    if(is_button_pushed()) {
        statemac_transition_to(&state_machine, &sVB1);
    }
    signal_vehicle_blue();
    signal_pedestrian_red();
    message_push();
}
void sVB0_leave(struct transtate_t* transtate)
{
}

void sVB1_enter(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "ENTER sVB1\n");
}
void sVB1_run(struct transtate_t* transtate)
{
    if(timer_is_timeup(T1 * MICROSEC_PER_SECONDS)) {
        statemac_transition_to(&state_machine, &sVY);
    }
    signal_vehicle_blue();
    signal_pedestrian_red();
    message_wait();
}
void sVB1_leave(struct transtate_t* transtate)
{
}

void sVY_enter(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "ENTER sVY\n");
    timer_reset();
}
void sVY_run(struct transtate_t* transtate)
{
    if(timer_is_timeup(T2 * MICROSEC_PER_SECONDS)) {
        statemac_transition_to(&state_machine, &sPB0);
    }
    signal_vehicle_yellow();
    signal_pedestrian_red();
    message_wait();
}
void sVY_leave(struct transtate_t* transtate)
{
}

void sPB0_enter(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "ENTER sPB0\n");
    timer_reset();
}
void sPB0_run(struct transtate_t* transtate)
{
    if(timer_is_timeup(T3 * MICROSEC_PER_SECONDS)) {
        statemac_transition_to(&state_machine, &sPY0);
    }
    signal_vehicle_red();
    signal_pedestrian_blue();
    message_off();
}
void sPB0_leave(struct transtate_t* transtate)
{
}

void sPY0_enter(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "ENTER sPY0\n");
    timer_reset();
}
void sPY0_run(struct transtate_t* transtate)
{
    if(is_button_pushed()) {
        statemac_transition_to(&state_machine, &sPY1);
    }
    if(timer_is_timeup(T4 * MICROSEC_PER_SECONDS)) {
        statemac_transition_to(&state_machine, &sVB0);
    }
    signal_vehicle_red();
    if(((timer_count / (MICROSEC_PER_SECONDS / (BLINK_FREQ * 2))) % 2) == 0) {
        signal_pedestrian_blue();
    } else {
        signal_pedestrian_off();
    }
    message_push();
}
void sPY0_leave(struct transtate_t* transtate)
{
}

void sPY1_enter(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "ENTER sPY1\n");
}
void sPY1_run(struct transtate_t* transtate)
{
    if(timer_is_timeup(T4 * MICROSEC_PER_SECONDS)) {
        statemac_transition_to(&state_machine, &sVB1);
    }
    signal_vehicle_red();
    if(((timer_count / (MICROSEC_PER_SECONDS / (BLINK_FREQ * 2))) % 2) == 0) {
        signal_pedestrian_red();
    } else {
        signal_pedestrian_off();
    }
    message_wait();
}
void sPY1_leave(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "LEAVE sPY1\n");
    timer_reset();
}

static volatile int sigterm = 0;
static volatile int sigint = 0;
static volatile int sighup = 0;

static void handle_sigint(int sig)
{
    dlog(LOG_INFO, "SIGINT received.\n");
    sigint = 1;
}
static void handle_sigterm(int sig)
{
    dlog(LOG_INFO, "SIGTERM received.\n");
    sigterm = 1;
}
static void handle_sighup(int sig)
{
    dlog(LOG_INFO, "SIGHUP received.\n");
    sighup = 1;
}

int daemon_main(int argc, char* argv[], char* envp[])
{
    dlog(LOG_INFO, "pedsignal started.\n");
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        dlog(LOG_ERR, "fail to setup SIGINT.\n");
        return 2;
    } else
    if (signal(SIGTERM, handle_sigterm) == SIG_ERR) {
        dlog(LOG_ERR, "fail to setup SIGTERM.\n");
        return 2;
#ifndef __MINGW32__
    } else if (signal(SIGHUP, handle_sighup) == SIG_ERR) {
        dlog(LOG_ERR, "fail to ignore SIGHUP.\n");
        return 2;
#endif
    }
	gpio_init();

	GPIO_LIGHT_CAR_BLUE = gpio_port_output(2);
	GPIO_LIGHT_CAR_YELLOW = gpio_port_output(3);
	GPIO_LIGHT_CAR_RED = gpio_port_output(4);
	GPIO_LIGHT_PED_WALK = gpio_port_output(17);
	GPIO_LIGHT_PED_STOP = gpio_port_output(18);
	GPIO_DISP_PED_PUSH = gpio_port_output(27);
	GPIO_DISP_PED_WAIT = gpio_port_output(22);
	GPIO_PED_BUTTON = gpio_port_input_pullup(24);
	
    statemac_init(&state_machine, &sVB0);

	while(!sigterm && !sigint) {
        statemac_run(&state_machine);
		usleep(10000);
        timer_increment(10000);
	}
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
	gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
	gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
	gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
	gpio_port_write(GPIO_DISP_PED_WAIT, 0);

    dlog(LOG_INFO, "pedsignal stopped.\n");
	return 0;
}

