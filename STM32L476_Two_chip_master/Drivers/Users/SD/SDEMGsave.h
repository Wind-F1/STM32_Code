#ifndef __SDEMGSAVE_H__
#define __SDEMGSAVE_H__
#include "ADC.h"

void EMGsave_SD(struct EMGACCs_t *PEMGACC);

typedef enum {NoFile = 0, FileOpen = 1}Filestate;

#endif

