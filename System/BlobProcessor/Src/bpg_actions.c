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
#include "mw_timebase.h"
#include "task.h"

blob_fp_t* bp_actions;

int _delay(blob_t* blob) {
	debug_p("BLOB-FUNC: actions._delay\n");
	if (blob->data.args_length != sizeof(uint32_t)) {
		error_report(7, BlobProcessorError);
		return -1;
	}
	uint32_t delay_amount_ms = *((uint32_t*) blob->data.args);
	debug_p("_delay timeout: %lums\n", delay_amount_ms);
	uint32_t timeout_end = mw_timebase_ticks_get() + delay_amount_ms;
	while (mw_timebase_ticks_get() < timeout_end) {
		vTaskDelay(0);
	}

	/* return block */
	return 0;
}

/* @formatter:off */
static const blob_fp_t _actions[] = {
	_delay,
};
/* @formatter:on */

void bpt_actions_init() {
	bp_actions = (blob_fp_t*) _actions;
}
