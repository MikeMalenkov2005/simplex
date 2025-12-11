#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#define SYS_EXIT  0 /* Exit Task (status *UNUSED FOR NOW*)  */

#define SYS_SEND  1 /* Send Message (payload[K_MESSAGE_SIZE], target)   */
#define SYS_POLL  2 /* Poll Message (buffer[K_MESSAGE_SIZE]) -> tid     */
#define SYS_WAIT  3 /* Wait Message (buffer[K_MESSAGE_SIZE]) -> tid     */
#define SYS_FIRE  4 /* Fire Message (payload[K_MESSAGE_SIZE], target)   */

#define SYS_GET_TASK_ID   5   /* Get Task ID                            */
#define SYS_GET_GROUP_ID  6   /* Get Task Group ID                      */
#define SYS_GET_PARENT_ID 7   /* Get Parent Task ID                     */
#define SYS_GET_LEADER_ID 8   /* Get Leader Task ID                     */

#define SYS_CREATE_THREAD 9   /* Create Thread Task (entry, stack size) */
#define SYS_SWITCH_TASK   10  /* Switch to the next queued Task         */

#define SYS_SIGNAL_TASK   11  /* Interrupt a Task (tid, value to stack) */
#define SYS_SET_HANDLER   12  /* Set a Signal Handler (func)            */

#define SYS_GET_TIME      13  /* Get the current time as time_t         */
#define SYS_SET_TIME      14  /* Set the current time (time_t)          */

#define SYS_TLS_NEW       15  /* Create a new TLS entry () -> slot      */
#define SYS_TLS_GET       16  /* Get a TLS entry value (slot) -> value  */
#define SYS_TLS_SET       17  /* Set a TLS entry value (slot, value)    */

#define SYS_GROUP_EXIT    18  /* Exit the Task Group (status)           */

#define SYS_GET_TICKS     19  /* Get the number of ticks since boot     */

#define SYS_MAP   20 /* Map Memory/Device (Address(for device), Size, Flags)  */
#define SYS_FREE  21 /* Free Memory/Device (VirtualAddress, Size)             */
#define SYS_REMAP 22 /* ReMap Memory/Device (VirtualAddress, Size, Flags)     */
#define SYS_SHARE 23 /* Share Memory/Device (VirtualAddress, Size, TaskID)    */

#define SYS_GET_TIME_ZONE 24  /* Get the local time zone () -> seconds        */
#define SYS_SET_TIME_ZONE 25  /* Set the local time zone (seconds) -> prev    */

#define SYS_IRQ_WAIT  30 /* Wait For IRQ (irq)  */
#define SYS_IRQ_EXIT  31 /* Exit IRQ Loop       */

/* Signal Handlers MUST preserve ALL GP-Registers and CPU flags */

/* * * * * * * * * * * * * * * *
 * Signal Handler Stack Frame  *
 * +- - - - - - - - -+         *
 * |    sig value    |         *
 * +-----------------+         *
 * | return address  |         *
 * +-----------------+ <- TOP  *
 * * * * * * * * * * * * * * * */

#endif

