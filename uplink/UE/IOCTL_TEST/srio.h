#include <stdint.h>
#define USERIOCCMD	SIOCDEVPRIVATE + 1

#define PSEND_RUN 	0x01
#define PSEND_STOP 	0x02
#define PSEND_INTERVAL	0x03
#define SEND_TO_SRIO	0x4E
#define READ_MEM		0x4F
#define RA_COMPLETE		0x48
#define RIODT_STOP		0x4D
#define LOST_PKT_COUNT	0x4C
#define ADD_INTERVAL	0x66
#define MINUS_INTERVAL	0x67
#define PRINT_COUNT_STATISTICS 0x68
#define STOP_TIMER 0x888
#define SEND_INFO 0x889
#define HELLO_WORLD 0x900

typedef uint32_t 	u32;

struct kifreq
{
	u32 cmd;
	size_t	size;
	void* data_ptr;
};

void periodical_send_stop(struct kifreq* ifr_ptr);	
void send_to_srio(struct kifreq* ifr_ptr);	
void ra_complete(struct kifreq* ifr_ptr);
void read_mem(struct kifreq* ifr_ptr);
void lost_pkt(struct kifreq* ifr_ptr);
void stop_riodt(struct kifreq* ifr_ptr);	
void periodical_send_run(struct kifreq* ifr_ptr);
void periodical_send_interval(struct kifreq* ifr_ptr, float interval);
void add_interval(struct kifreq* ifr_ptr);
void minus_interval(struct kifreq* ifr_ptr);
void pkt_from_mac(struct kifreq* ifr_ptr);
void usage(void);
void stop_timer(struct kifreq* ifr_ptr);
void send_info(struct kifreq* ifr_ptr);
void print_hello(struct kifreq* ifr_ptr);
