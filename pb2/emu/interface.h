#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emu.h"

#define ARRLEN(s) (sizeof(s) / sizeof(*s))

char* rl_gets(void);
int cmd_q(char*);
int cmd_help(char*);
int cmd_c(char*);
int cmd_si(char*);
int cmd_info(char*);
int cmd_x(char*);
int cmd_cto(char*);

typedef struct {
    char* name;
    char* description;
    int (*handler)(char*);  // function pointer
} cmd_t;


__attribute_used__ static cmd_t cmd_table[]  = {
    {"q", "exit the interactive interface", &cmd_q},
    {"help", "print useful information about supported commands", &cmd_help},
    {"c", "Execute instructions until the end", &cmd_c},
    {"si", "Execute single instruction (if N is not passed) / execute N instructions", &cmd_si},
    {"info", "Print values of all registers (r) / Print current PC (pc)", &cmd_info},
    {"x", "Print N words starting from BASE_ADDR", &cmd_x},
    {"cto", "Execute instructions until PC == ADDR", &cmd_cto},
};
#define NUM_CMD ARRLEN(cmd_table)

#endif