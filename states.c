#ifndef _WIN32
#ifndef __MINGW32__
#include <syslog.h>
#endif
#endif
#ifdef _WIN32
#define LOG_INFO 0
void dlog(int level, char const* msg) {
	/* NOTHING TO DO */
}
#endif
#include "statemac.h"
#include "gpio_signals.h"
#include "states.h"
#include "timer.h"

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
    dlog(LOG_INFO, "pedsignal enter state sVB0\n");
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
    dlog(LOG_INFO, "pedsignal enter state sVB1\n");
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
    dlog(LOG_INFO, "pedsignal enter state sVY\n");
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
    dlog(LOG_INFO, "pedsignal enter state sPB0\n");
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
    dlog(LOG_INFO, "pedsignal enter state sPY0\n");
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
    dlog(LOG_INFO, "pedsignal enter state sPY1\n");
}
void sPY1_run(struct transtate_t* transtate)
{
    if(timer_is_timeup(T4 * MICROSEC_PER_SECONDS)) {
        statemac_transition_to(&state_machine, &sVB1);
    }
    signal_vehicle_red();
    if(((timer_count / (MICROSEC_PER_SECONDS / (BLINK_FREQ * 2))) % 2) == 0) {
        signal_pedestrian_blue();
    } else {
        signal_pedestrian_off();
    }
    message_wait();
}
void sPY1_leave(struct transtate_t* transtate)
{
    dlog(LOG_INFO, "pedsignal leave state sPY1\n");
    timer_reset();
}

