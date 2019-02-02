#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <stdint.h>

void mailbox_turn_led_on();
uint32_t mailbox_get_arm_memory_size();
uint32_t mailbox_get_vc_memory_size();
uint32_t mailbox_get_firmware_revision();
uint32_t mailbox_get_board_model();
uint32_t mailbox_get_board_revision();
void mailbox_print_all_clocks();
uint32_t mailbox_get_system_temp();

#endif
