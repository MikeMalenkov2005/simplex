#ifndef _DRV_X86PS2_H
#define _DRV_X86PS2_H

#define PS2_DEVICE_KEYBOARD 0
#define PS2_DEVICE_MOUSE    1 /* TODO: NOT IMPLEMENTED */

#define PS2_RECEIVE 1 /* (dev)                              */
#define PS2_COMMAND 2 /* (dev, cmd_size, ack_size, cmd...)  */
#define PS2_DETECT  3 /* (dev) TODO: NOT IMPLEMENTED        */

#define PS2_REVEIVE_S "\001"
#define PS2_COMMAND_S "\002"
#define PS2_DETECT_S  "\003"

#endif

