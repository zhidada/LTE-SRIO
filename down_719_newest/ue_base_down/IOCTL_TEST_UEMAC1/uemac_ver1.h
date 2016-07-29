#include <stdbool.h>
#define USERIOCCMD	SIOCDEVPRIVATE + 1
/*
#define PSEND_RUN 	0x01
#define PSEND_STOP 	0x02
#define PSEND_INTERVAL	0x03
*/
#define _RRCtoMAC_reset           	   0x01
#define _RLCtoMAC_datasend_Req         0x02
#define _PHYtoMAC_RA_Req               0x03  //携带参数，RA-RNTI,RAPID(可能有多个),TA,
#define _PHYtoMAC_SR                   0x04   //物理层接收到某个UE的SR,携带参数C-RNTI
#define _IOCCMD_TEST_WITHDATA          0x05  //测试用
#define _IOCTL_USERtoRLC_send_Req      0x12//用户空间给RLC的命令
#define _IOCCMD_RRCtoMAC_LC_config     0x11
/*

/*************分割线********************/
#define  _IOCTLtoUEMAC_RAR_Failed  0x70//配置RAR失败次数
#define  _IOCTLtoUEMAC_Contention_Failed 0x71//配置竞争解决失败次数

#define _IOCTLtoUEMAC_PKT_NUM	0x72

/*
struct kifreq
{
	unsigned char cmd;
	unsigned short	size;
	void* data_ptr;
};
*/

struct kifreq
{
	unsigned int cmd;
	size_t size;
	void* data_ptr;
};
struct test_s
{	
	char name[5];
	int num;	
};

struct Ul_SpecificParameters{
	int priority;        //INTEGER (1..16)
	enum PrioritisedBitRate{
		kBps0=0, kBps8=8, kBps16=16, kBps32=32, kBps64=64, kBps128=128,
		kBps256=256, infinity=-1//..........spare1~8未表示
	}prioritisedBitRate;
	enum BucketSizeDuration{
		bucketSizeDuration_ms50=50, bucketSizeDuration_ms100=100, bucketSizeDuration_ms150=150, bucketSizeDuration_ms300=300, 
		bucketSizeDuration_ms500=500, bucketSizeDuration_ms1000=1000//..........spare1~2未表示
	}bucketSizeDuration;
	int logicalChannelGroup;	//INTEGER (0..3) 
};

struct LogicalChannelConfig//lhl 更改，RRC层关于逻辑信道的结构
{
	bool haveUl_SpecificParameters;
	struct Ul_SpecificParameters ul_SpecificParameters;
}__attribute__((packed));


typedef struct MAC_LogicalChannelConfig_IoctrlMsg{
	unsigned short crnti;
	int logicalChannelIdentity;
	struct LogicalChannelConfig logicalChannelConfig;
}__attribute__((packed))MAC_LogicalChannelConfig_IoctrlMsg;

/*
typedef struct RA_req_info
{
	u16 ra_rnti;
	u8 rapid;//ra_preamble_index 暂时只实现为只含一个rapid
}__attribute__((packed))RA_req_info;
*/
#define RA_UE_NUMBER_MAX 10
/*
struct S_RAinfo //当有UE接入eNB时，物理层会检测到RNTI和RAPID上报 按这样的格式
{
    u16    ra_rnti;             //
    u16    pgtype;           //0,1,2
    u16    sfn;              //系统帧号，0—1023
    u16    subframeN;        //子帧号，0——9
    u16    crc; 
    u16    harqindex;
    u16    sr;
    u16    cqi;
    u16    pmi;
    u16    ta;
    u8    rapid[RA_UE_NUMBER_MAX];
}__attribute__((packed));
*/
struct S_RAinfo //当有UE接入eNB时，物理层会检测到RNTI和RAPID上报 按这样的格式
{
    unsigned short   ra_rnti;             //
    unsigned short    pgtype;           //0,1,2
    unsigned short    sfn;              //系统帧号，0—1023
    unsigned short    subframeN;        //子帧号，0——9
    unsigned short    crc; 
    unsigned short   harqindex;
    unsigned short    sr;
    unsigned short    cqi;
    unsigned short    pmi;
    unsigned short   ta;
    char    rapid[RA_UE_NUMBER_MAX];
}__attribute__((packed));

//void periodical_send_stop(struct kifreq* ifr_ptr);	
//void periodical_send_run(struct kifreq* ifr_ptr);
//void periodical_send_interval(struct kifreq* ifr_ptr, float interval);
void send_reset(struct kifreq* ifr_ptr);
void usage(void);
/*******************分割线*******************************/
void rar_failed(struct kifreq * ifr_ptr,char *failed_times);
void contention_failed(struct kifreq * ifr_ptr,char *failed_times);
void get_pkt_num(struct kifreq* ifr_ptr);
void get_mac_pkt_num(struct kifreq* ifr_ptr);
