/*
 * The 'Push-to-Walk' signal emulator
 */
#ifndef __MINGW32__
#include <syslog.h>
#endif

#include <signal.h>
#ifndef __MINGW32__
#include <sys/wait.h>
#endif
#include "gpio_signals.h"
#include "statemac.h"
#include "states.h"

static volatile int sigterm = 0;
static volatile int sigint = 0;
static volatile int sighup = 0;

static void handle_sigint(int sig)
{
    dlog(LOG_INFO, "pedsignal SIGINT received.\n");
    sigint = 1;
}
static void handle_sigterm(int sig)
{
    dlog(LOG_INFO, "pedsignal SIGTERM received.\n");
    sigterm = 1;
}
static void handle_sighup(int sig)
{
    dlog(LOG_INFO, "pedsignal SIGHUP received.\n");
    sighup = 1;
}

int daemon_main(int argc, char* argv[], char* envp[])
{
    dlog(LOG_INFO, "pedsignal started.\n");
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        dlog(LOG_ERR, "pedsignal fail to setup SIGINT.\n");
        return 2;
    } else
    if (signal(SIGTERM, handle_sigterm) == SIG_ERR) {
        dlog(LOG_ERR, "pedsignal fail to setup SIGTERM.\n");
        return 2;
#ifndef __MINGW32__
    } else if (signal(SIGHUP, handle_sighup) == SIG_ERR) {
        dlog(LOG_ERR, "pedsignal fail to ignore SIGHUP.\n");
        return 2;
#endif
    }

    signal_setup();
    statemac_init(&state_machine, &sVB0);
	while(!sigterm && !sigint) {
        statemac_run(&state_machine);
		usleep(10000);
        timer_increment(10000);
	}
    signal_shutdown();
    dlog(LOG_INFO, "pedsignal stopped.\n");
	return 0;
}

