#ifndef __BUMP_H
#define __BUMP_H

#include <stdint.h>

extern uint8_t BUMP_DATA;
extern uint8_t BUMP_VALID;
extern float BUMP_ERR;

void Bump_Init(void);

uint8_t Bump_Read(void);
void Get_Bump_Update(void);
#endif
