/**
 * @file:   usr_proc.h
 * @brief:  Two user processes header file
 * @author: TEAM BLACKJACK
 * @date:   2014/01/17
 */
 
#ifndef USR_PROC_H_
#define USR_PROC_H

extern int g_timer_count;

void print(char* msg);
void set_test_procs(void);
void print_test_results(void);
void proc1(void);
void proc2(void);
void priority_test(void);
void memory_block_test(void);
void blocked_test(void);
void message_send_test(void);
void message_receive_test(void);

#endif /* USR_PROC_H_ */
