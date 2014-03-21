/**
 * @file:   usr_proc.c
 * @brief:  Two user processes: proc1 and proc2
 * @author: TEAM BLACKJACK
 * @date:   2014/01/17
 * NOTE: Each process is in an infinite loop. Processes never terminate.
 */

#include "rtx.h"
#include "uart_polling.h"
#include "usr_proc.h"

#ifdef DEBUG_0
#include "printf.h"
#endif /* DEBUG_0 */

typedef struct msg {
	int mtype;
	char mtext[1];
} msg;

/* initialization table item */
PROC_INIT g_test_procs[NUM_TEST_PROCS];
int test_results[NUM_TEST_PROCS];

void print_crt(char* message) {
	msg* send_to_crt;
	send_to_crt = (msg*)request_memory_block();
	send_to_crt->mtype = 2;
	//send_to_crt->mtext = message; use strncpy if needed
	
	send_message(13, send_to_crt);
}

void print_debug(char* message) {
	#ifdef DEBUG_0
		uart1_put_string(message);
	#endif
}

void print_char_debug(char message) {
	#ifdef DEBUG_0
		uart1_put_char(message);
	#endif
}

void set_test_procs() {
	int i;
	for( i = 0; i < NUM_TEST_PROCS; i++ ) {
		g_test_procs[i].m_pid=(U32)(i+1);
		g_test_procs[i].m_priority=LOWEST;
		g_test_procs[i].m_stack_size=0x100;
		test_results[i] = 1;
		
		// Testing with different priorities on processes
		if (i == 3) {  // Memory blocks test
			g_test_procs[i].m_priority=3;
  	}
		if (i == 4) { // Send message process
			g_test_procs[i].m_priority=3;
		}
		if (i == 5) { // Recieve message process	
			g_test_procs[i].m_priority=4;
		}
		if (i == 6) { // A	
			g_test_procs[i].m_priority=4;
		}
		if (i == 7) { // B	
			g_test_procs[i].m_priority=4;
		}
		if (i == 8) { // C	
			g_test_procs[i].m_priority=4;
		}
	}

	g_test_procs[0].mpf_start_pc = &proc1;
	g_test_procs[1].mpf_start_pc = &proc2;
	g_test_procs[2].mpf_start_pc = &priority_test;
	g_test_procs[3].mpf_start_pc = &memory_block_test;
	g_test_procs[4].mpf_start_pc = &message_send_test;
	g_test_procs[5].mpf_start_pc = &message_receive_test;
	g_test_procs[6].mpf_start_pc = &A;
	g_test_procs[7].mpf_start_pc = &B;
	g_test_procs[8].mpf_start_pc = &C;
	
	print_debug("\n\r");
  print_debug("G021_test: START\r\n");
}

/**
 * @brief: a process that prints five uppercase letters
 *         and then yields the cpu.
 */
void proc1(void)
{
	int i = 0;
	int ret_val = 10;
	while (1) {
		if ( i != 0 && i%5 == 0 ) {
			print_debug("\n\r");
	
			ret_val = release_processor();
			
			print_debug("Process 1 completed!\r\n");

			if (ret_val == -1) {
				print_debug("G021_test: test 1 FAIL\r\n");
				test_results[0] = 0;
			} else {
				print_debug("G021_test: test 1 OK\r\n");
			}
		}
		
		print_char_debug('A' + i%26);
		i++;
	}
}

/**
 * @brief: a process that prints five numbers
 *         and then yields the cpu.
 */
void proc2(void)
{
	int i = 0;
	int ret_val = 20;
	
	while (1) {
		if ( i != 0 && i%5 == 0 ) {
			print_debug("\n\r");
			ret_val = release_processor();
			
			print_debug("Process 2 completed!\r\n");
			
			if (ret_val == -1) {
				print_debug("G021_test: test 2 FAIL\r\n");
				test_results[1] = 0;
			} else {
				print_debug("G021_test: test 2 OK\r\n");
			}
		}
		print_char_debug('0' + i%10);
		i++;
	}
}

/**
 * @brief: Sets priority for process and returns get value
 */
void priority_test(void)
{	
	int ret_val = 50;
	int i = 0;
	//set_process_priority(5,1);
	
	while(1) {	
		for (i = 1;i < 3;i++) {
			set_process_priority(3,i);
			print_debug("PRIORITY SET!\r\n");
		}
		
		print_debug("Process 3 completed!\r\n");
		
		set_process_priority(3,3);
		ret_val = release_processor();
		//printf("Timer result: %d \r\n",g_timer_count);
		if (ret_val == -1) {
			print_debug("G021_test: test 3 FAIL\r\n");
			test_results[2] = 0;
		} else {
			print_debug("G021_test: test 3 OK\r\n");
		}
	}
}

/**
 * @brief: Requests memory blocks and returns
 */
void memory_block_test(void)
{	
	int ret_val = 60;
	int i = 0;
	int* tmp_int;
	char* tmp_string;
	
	while(1) {
		print_debug("Iterating through proc 4 with\r\n");
		tmp_int = request_memory_block();
		tmp_string = request_memory_block();
		i++;
		*tmp_int = i;
		if (i > 1337) {
			i = 0;
		}
		tmp_string[0] = 'H';
		tmp_string[1] = 'E';
		tmp_string[2] = 'L';
		tmp_string[3] = 'L';
		tmp_string[4] = 'O';
		tmp_string[5] = '\0';
		
		//printf("INT!   Address: 0x%x, Value: %d\r\n",tmp_int,*tmp_int);
		print_debug("CHAR[]!: ");
		print_debug(tmp_string);
		print_debug("\r\n");
		//printf("CHAR[]! Address: 0x%x, Value: %s\r\n",&tmp_string,tmp_string);
		
		release_memory_block(tmp_int);
		release_memory_block(tmp_string);
		
		print_debug("Proc 4 completed!\r\n");
		
		ret_val = release_processor();
		
		if (ret_val == -1) {
			print_debug("G021_test: test 4 FAIL\r\n");
			test_results[3] = 0;
		} else {
			print_debug("G021_test: test 4 OK\r\n");
		}
	}
}

/* use memory_block_test and comment out releases to test blocked on memory
void blocked_test(void)
{	
	int ret_val = 70;
	int i = 0;
	int* tmp_int;
	
	while(1) {
		tmp_int = request_memory_block();
		
		i++;
		*tmp_int = i;
		if (i > 1337) {
			i = 0;
		}
		
		print_debug("Proc 5 completed!\r\n");
		
		//release_memory_block(tmp_int);
		ret_val = release_processor();
		
		if (ret_val == -1) {
			print_debug("G021_test: test 5 FAIL\r\n");
			test_results[4] = 0;
		} else {
			print_debug("G021_test: test 5 OK\r\n");
		}
		
		if (i == 5) {
			//print_test_results();
		}
	}
}*/
	
void message_send_test(void) {
	int ret_val = 80;
	char text[25] = "Vegetables are my enemy.";
	
	
	while(1) {
		msg* testMessage;
		testMessage = (msg*)request_memory_block();
		testMessage->mtype = 2;
		my_strcpy(testMessage->mtext, text);
		//testMessage->mtext = text;
		
		/*msg* testMessage2 = (msg*)request_memory_block();
 		testMessage2->mtype = 0;
		testMessage2->mtext = "MSG is my favourite vitamin.";*/
		
		print_debug("Preparing to send message...");
		send_message(13, testMessage);
		//delayed_send(6, testMessage, 150);
		print_debug("Message sent!\r\n");
		
		//send_message(6, testMessage2);
		//delayed_send(6, testMessage2, 100);
		
		print_debug("Process 6 completed!\r\n");
		
		ret_val = release_processor();
	}
}
	
void message_receive_test(void) {
	int ret_val = 90;
	
	while(1) {
	  int i = 0;
		int sender_id;
	
		msg* rcvMessage;

		rcvMessage = (msg*)(receive_message(&sender_id));
		//printf("SenderID: %d Message:",sender_id);
		while (rcvMessage->mtext[i] != '\0') {
			print_char_debug(rcvMessage->mtext[i++]);
		}
		
		release_memory_block(rcvMessage);
		
		print_debug("Process 7 completed!\r\n");
		
		ret_val = release_processor();
	}
}

void A(void) {
	int num;
	void* p = request_memory_block();
	/*register with Command Decoder as handler of %Z commands
	while (1) {
		p = receive_message(NULL);
		if (the message(p) contains the %Z command then) {
			release_memory_block(p);
			break;
	  } else {
			release_memory_block(p);
		}
	}
	num = 0;
	while (1) {
		p = request_memory_block();
		((Message *)p)->mtype = count_report;
		((Message *)p)->mtext = num;
		send_message(8,p);
		num = num + 1;
		release_processor();
	}*/
	// note that Process A does not de-allocate
	// any received envelopes in the second loop
}

void B(void) {
	/*while (1) {
		void* msg = receive_message(NULL);
		send_message(7,msg);
		release_processor();
	}*/
}

void C (void) {
	while (1) {/*
		perform any needed initialization and create a local message queue
		while (1) {
			if (local message queue is empty) {
				p <- receive a message
			}
			else {
				p <- dequeue the first message from the local message queue
			}
			
			if (msg_type of p == count_report) {
				if (msg_data[0] of p is evenly divisible by 20) {
					send "Process C" to CRT display using msg envelope p
					q <- request_memory_block()
					request a delayed_send for 10 sec delay with msg_type=wakeup10 using q
					while (1) {
						p <- receive a message //block and let other processes execute
						if (message_type of p == wakeup10) then
						exit this loop
					} else {
						put message (p) on the local message queue for later processing
					}
				}
			}
		}
		
		release_memory_block(p);*/
		release_processor();
	}
}
