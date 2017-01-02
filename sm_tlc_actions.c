#include "sm_tlc_actions.h"
#include <unistd.h> // for NULL
#include <stdio.h> // for puts

void tlc_off_on_entry(void)
{
}

void tlc_off_on_exit(void)
{
}

void tlc_off_run(void)
{
}

void tlc_red_on_entry(void)
{
    puts("Red on.");
}

void tlc_red_on_exit(void)
{
    puts("Red off.");
}

void tlc_red_run(void)
{
}

void tlc_yellow_on_entry(void)
{
    puts("Yellow on.");
}

void tlc_yellow_on_exit(void)
{
    puts("Yellow off.");
}

void tlc_yellow_run(void)
{
}

void tlc_green_on_entry(void)
{
    puts("Green on.");
}

void tlc_green_on_exit(void)
{
    puts("Green off.");
}

void tlc_green_run(void)
{
}


