/*
 * utils.h
 *
 *  Created on: Apr 19, 2020
 *      Author: Zviad
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <inttypes.h>
#include "defines.h"

/*
 * Gets length of unsigned number up to 10 digits
 * @params x: unsigned number
 *
 * @returns: length
 *  */
int unumlen(uint32_t x);

void* find(void* collection, void* item, uint32_t item_size, uint32_t count);

bool_t equals(void* item1, void* item2, uint32_t item_size);

bool_t is_integer(char* str);

void strtrim(const char* src, char* dest, uint32_t dest_max);

bool_t strnullempty(const char* restrict str);

uint8_t singlebitpos(uint16_t n);

#endif /* INC_UTILS_H_ */
