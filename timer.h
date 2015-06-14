#define MICROSEC_PER_SECONDS 1000000
extern int timer_count;

void timer_increment(int uS);
void timer_reset();
int timer_is_timeup(int uS);


