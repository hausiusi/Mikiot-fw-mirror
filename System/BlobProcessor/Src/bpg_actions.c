/*
 * bpg_actions.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "debug.h"
#include "error.h"
#include "FreeRTOS.h"
#include "macrodefs.h"
#include "mw_timebase.h"
#include "task.h"

bfp_array_t bp_actions;

static int _delay(blob_t* blob) {
	debug_info("BLOB-FUNC: actions._delay\n");
	blob->counter += blob->data.args_length;
	if (blob->data.args_length != sizeof(uint32_t)) {
		error_report(7, BlobProcessorError);
		return -1;
	}
	uint32_t delay_amount_ms = *((uint32_t*) blob->data.args);
	debug_info("_delay timeout: %lums\n", delay_amount_ms);
	uint32_t timeout_end = mw_timebase_ticks_get() + delay_amount_ms;
	while (mw_timebase_ticks_get() < timeout_end) {
		vTaskDelay(0);
	}

	/* return block */
	return 0;
}

static int _jump(blob_t* blob) {
	debug_info("BLOB-FUNC: actions._jump\n");
	blob->counter += blob->data.args_length;
	if (blob->data.args_length != sizeof(uint16_t)) {
		error_report(11, BlobProcessorError);
		return -1;
	}
	uint16_t jump_to = *((uint16_t*) blob->data.args);
	debug_info("Jump to: %i\n", jump_to);
	blob->counter = jump_to;
	return 0;
}

/* @formatter:off */
static const blob_fp_t _actions[] = {
	_delay,
	_jump,
};
/* @formatter:on */

void bpt_actions_init() {
	bp_actions.functions = (blob_fp_t*) _actions;
	bp_actions.len = array_count(_actions);
}
