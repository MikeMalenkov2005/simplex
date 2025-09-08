#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#define SYS_EXIT  0 /* Exit Task (code *UNUSED FOR NOW*)  */

#define SYS_SEND  1 /* Send Message (payload[K_MESSAGE_SIZE], target) */
#define SYS_POLL  2 /* Poll Message (buffer[K_MESSAGE_SIZE]) -> tid   */
#define SYS_WAIT  3 /* Wait Message (buffer[K_MESSAGE_SIZE]) -> tid   */
#define SYS_PEEK  4 /* Peek Message (buffer[K_MESSAGE_SIZE]) -> tid   */

#define SYS_GET_TASK_ID   5 /* Get Task ID                            */
#define SYS_GET_GROUP_ID  6 /* Get Task Group ID                      */
#define SYS_GET_PARENT_ID 7 /* Get Parent Task ID                     */
#define SYS_GET_LEADER_ID 8 /* Get Leader Task ID                     */
#define SYS_CREATE_THREAD 9 /* Create Thread Task (entry, stack size) */

#define SYS_MAP   20 /* Map Memory/Device (Address(for device), Size, Flags)  */
#define SYS_FREE  21 /* Free Memory/Device (VirtualAddress, Size)             */
#define SYS_REMAP 22 /* ReMap Memory/Device (VirtualAddress, Size, Flags)     */
#define SYS_SHARE 23 /* Share Memory/Device (VirtualAddress, Size, TaskID)    */

#define SYS_IRQ_WAIT  30 /* Wait For IRQ (irq)  */
#define SYS_IRQ_EXIT  31 /* Exit IRQ Loop       */

#endif

