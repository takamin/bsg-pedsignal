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

