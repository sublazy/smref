#ifndef FSM_TLC_ACTIONS_H
#define FSM_TLC_ACTIONS_H

void tlc_off_on_entry(void *user_data);
void tlc_off_on_exit(void *user_data);
void tlc_off_on_tick(void *user_data);
void tlc_red_on_entry(void *user_data);
void tlc_red_on_exit(void *user_data);
void tlc_red_on_tick(void *user_data);
void tlc_yellow_on_entry(void *user_data);
void tlc_yellow_on_exit(void *user_data);
void tlc_yellow_on_tick(void *user_data);
void tlc_green_on_entry(void *user_data);
void tlc_green_on_exit(void *user_data);
void tlc_green_on_tick(void *user_data);

#endif // FSM_TLC_ACTIONS_H
