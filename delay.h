#pragma once
#ifndef _DELAY_H
#define _DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "misc.h"

void delay_init(void);
void delay_ms(uint16_t nms);	//��ʱms������ʱ
void delay_us(uint32_t nus);	//��ʱus
void delay_ms_long(uint16_t nms);	//��ʱms������ʱ����ε��ö���ʱ

#ifdef __cplusplus
}
#endif

#endif