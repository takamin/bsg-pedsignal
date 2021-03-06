#include "statemac.h"
int statemac_init(STATEMAC* statemac, TRANSTATE* transtates)
{
    statemac->current_state = transtates;
    statemac->next_state = 0;
    return 0;
}
int statemac_run(STATEMAC* statemac)
{
    if(statemac->current_state != statemac->next_state) {
        if(statemac->next_state == 0) {
            statemac->next_state = statemac->current_state ;
        }
        statemac->current_state = statemac->next_state;
        statemac->current_state->enter(statemac->current_state);
    }
    statemac->current_state->run(statemac->current_state);
    if(statemac->current_state != statemac->next_state) {
        statemac->current_state->leave(statemac->current_state);
    }
    return 0;
}
int statemac_transition_to(STATEMAC* statemac, TRANSTATE* next_state)
{
    statemac->next_state = next_state;
    return 0;
}
