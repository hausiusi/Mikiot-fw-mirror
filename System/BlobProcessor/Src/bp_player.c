/*
 * bp_player.c
 *
 *  Created on: Jan 29, 2020
 *      Author: Zviad
 */

#include <inttypes.h>
#include <string.h>
#include "defines.h"
#include "debug.h"
#include "bp_helper.h"
#include "bp_groups.h"
#include "error.h"

bool_t _verify(uint8_t* blob_bytes) {
	//TODO: will be implemented with ISSUE_39
	return true;
}

void bp_player_play(uint8_t* blob_bytes) {
	if (!_verify(blob_bytes)) {
		error_report(4, VerificationError);
		return;
	}
	blob_t blob = { 0 };
	blob.length = *((uint32_t*) blob_bytes);
	blob.crc = *(uint16_t*) (blob_bytes + (blob.length - 2));
	/* Skip blob length-bytes (4) and never process checksum-bytes (2). */
	blob.counter += offsetof(blob_t, data); // Start from blob->data.
	while ((blob.counter) < (blob.length - 2)) {
		memcpy((uint8_t*) &blob.data, blob_bytes + blob.counter,
				sizeof(blob_data_t));
		/* Initialize pointer of blob->data arguments every time. */
		blob.counter += offsetof(blob_data_t, args); // Skip bad function nicely
		blob.data.args = blob_bytes + blob.counter;
		blob_fp_t bfp = bp_groups_get_actual_function(
				(bp_groups_enum_t) blob.data.group_id, blob.data.member_id);
		if (bfp) {
			bpf_exit_code = bfp(&blob);
			if (bpf_exit_code != 0) {
				debug_p("BLOB-PLAYER: function execution failed\n");
			}
		} else {
			error_report(10, BlobProcessorError);
			return;
		}
	}
}
