// Header: DCMI Control
// File Name: dcmi_control.h
// Author: Mikhail Kaa
// Date: 13.05.2023

#ifndef DCMI_CTRL
#define DCMI_CTRL

void dcmi_start(void);
void dcmi_stop(void);
void dcmi_control(uint8_t);

int ucmd_dcmi(int argc, char ** argv);

#endif /* DCMI_CTRL */
