#include "fsm_tlc_actions.h"
#include <unistd.h> // for NULL
#include <stdio.h> // for puts

void tlc_off_on_entry(void *user_data)
{
    (void)user_data;
    puts("----");
    puts("TLC off");
}

void tlc_off_on_exit(void *user_data)
{
    (void)user_data;
}

void tlc_red_on_entry(void *user_data)
{
    (void)user_data;
    puts("----");
    puts("Red on");
}

void tlc_red_on_exit(void *user_data)
{
    (void)user_data;
    puts("Red off");
    sleep(1);
}

void tlc_yellow_on_entry(void *user_data)
{
    (void)user_data;
    puts("----");
    puts("Yellow on");
}

void tlc_yellow_on_exit(void *user_data)
{
    (void)user_data;
    puts("Yellow off");
    sleep(1);
}

void tlc_green_on_entry(void *user_data)
{
    (void)user_data;
    puts("----");
    puts("Green on");
}

void tlc_green_on_exit(void *user_data)
{
    (void)user_data;
    puts("Green off");
    sleep(1);
}

void tlc_off_on_tick(void *user_data)
{
    (void)user_data;
}

void tlc_red_on_tick(void *user_data)
{
    (void)user_data;
}

void tlc_yellow_on_tick(void *user_data)
{
    (void)user_data;
}

void tlc_green_on_tick(void *user_data)
{
    (void)user_data;
}

