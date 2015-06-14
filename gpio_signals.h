/* TIMER DURATIONS */

                /* VEHICLE  PEDESTRIAN      */
#define T1 15   /* BLUE     RED             */
#define T2 5    /* YELLOW   RED             */
#define T3 20   /* RED      BLUE            */
#define T4 10   /* RED      BLINKING BLUE   */

#define BLINK_FREQ 2

void signal_setup(void);
void signal_shutdown(void);

void signal_vehicle_blue(void);
void signal_vehicle_yellow(void);
void signal_vehicle_red(void);
void signal_pedestrian_blue(void);
void signal_pedestrian_off(void);
void signal_pedestrian_red(void);

void message_push(void);
void message_wait(void);
void message_off(void);

int is_button_pushed(void);


