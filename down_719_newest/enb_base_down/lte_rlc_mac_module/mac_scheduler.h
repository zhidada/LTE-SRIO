#include "../fsm/fsmdec.h"

#include <linux/if_ether.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include "queue_manager.h"
#include "enb_macscheduler.h"
#include "macpkfmt.h"
#ifndef _MAC_SCHEDULER_H
#define _MAC_SCHEDULER_H

/*
*****************  �궨�� define  *****************
*/

//#define IOCCMD_MACTOPHY_UL_DCI 0x12
//#define IOCCMD_MACTOPHY_DL_DCI 0x13
//#define IOCCMD_MACTOPHY_RAR_DCI 0x14

/***** �����¼� *****/
#define TC_RNTI_CHECK_TIMER 15
#define TC_RNTI_CHECK (fsm_ev_type() == FSM_EV_TYPE_SELF && fsm_ev_code() == TC_RNTI_CHECK_TIMER )

/****DCI ****/
#define DCI0 0
#define DCI1 1
#define DCI1A 2
#define DCI1B 3
#define DCI1C 4
#define DCI1D 5
#define DCI2 6
#define DCI2A 7
#define DCI3 8
#define DCI3A 9


/****���е���****/
#define DCIFORMAT1A 1
#define LVRB 0
#define DVRB 1
#define DL_RAR_DATA 1	//1��PRB


#define UL_CQI 9
#define DL_CQI 9
//#define DL_SCHEDULE_NUM 5	//���е����û�����
#define DL_TA 2
/**** ���е��� ****/

#define DCIFORMAT0 0


/**** RNTI���� ****/
#define NUM_RNTI 200      //rnti��200��ȡֵ


#define CRNTI_TYPE 2
#define TC_RNTI 1
#define UNALLOCATED_RNTI 0
/**** ������� ****/
#define SIZE_MSG3 56  	 //the data size of msg3(random access),bit
#define RAR_RIV_RNTI 63
/**** �������� ****/
#define CHANNEL_BANDWIDTH 20 
#define CPMODE_OFDM 7	//cp mode :6/7,every PRB has the number of OFDM.
//ռ������
#define CFI_OFDM 1
#define SPECIAL_OFDM 4
//#define RS_OFDM 12;
//ռ�ĸ���
#define PBCH_OFDM 264//PRB47-52 PBCH:288-24
#define PSCH_OFDM 72//PRB47-52 PSCH:72
#define SSCH_OFDM 72//PRB47-52 SSCH:72

#define SCHEDULE_MODE 0 //decide the way of schedule:(0)Persistent, (1)Semi_Persistent ,(2)dynamic




/*
*****************  ���ݽṹ   *****************
*/

/**  data structure of RNTI manager **/
struct rnti_array
{
	u16 m_rnti;
	u16  Type_rnti;// 0:��δ������ 1:TC-RNTI  2:C-RNTI 
 
};

/**  data structure of rar_access **/
typedef struct ULgrant_rar
{
    u32	m_hoppingflag	:1;            	//1bit  ��Ƶ��־
	u32	rb_assignment	:10;         	//10bit ��Դ����RIV
	u32	m_mcs			:4;          	//4bit  ���Ʊ����ʽ0-15
  	u32	m_tpc			:3;           	//3bits ��Թ���
	u32	m_ulDelay		:1;           	//1bit  UL�ӳ�
    u32	m_cqiRequest	:1;				//1bit  cqi����
	u32	emptybits		:12;			
}__attribute__((packed))ULgrant;//structure  ULgrant

typedef struct Data_of_MacRarPdu
{
	u16	m_tcrnti;
	u16	m_ta;
	u16	m_bi;
	u16	m_rapid;
	ULgrant s_ulgrant;//���ulgrant����
}__attribute__((packed))DATA_RAR_PDU;//structure of RAR elements to  Multiplexing


//����Դ��������2  
// 7.1.6.3

struct result_sched_algorithm
{
	u16 m_rnti;
	//u32 m_bitmap;
	u16 rb_start;
	u16 lcrbs;
	u32 m_data_size;
	u16 sfn; 	            //ϵͳ֡�ţ�0��1023
	u16 subframeN;			//��֡�ţ�0����9
	u32 tbsize;
};


typedef struct Ul_DCI
{
    u32	m_format  	:1;                 //1bit ����format0��1A
    u32	m_hopping 	:1;                 //1bit  ��Ƶ��־
    u32	m_riv      	:13;                //13bit:when band==20MHZ type2
    u32	m_mcs		:5;                 //5bit ���Ʊ����ʽ��RV��Ϣ
    u32	m_ndi		:1;                 //1bit �����ݱ�־
    u32	m_tpc		:2;                 //2bit
    u32 Cyclic_shift:3;                 //3bit ���ݽ����Ƶ��ѭ����λ
    u32 m_dai		:2;                 //2bit ���з�����Ŀ�ı�ʶ when������ʱ����ȣ�#1��6
    u32 m_cqiRequest:1;                 //1bit cqi�����ʶ
    u32 padding		:2;                 //2bit ������ ʹ�䳤����format 1A��ͬ(Э��Ҫ��)
    u32 emptybits	:1;					//��ϵͳ������32bit������  
}__attribute__((packed))Data_Ul_DCI;// structure of UL DCI format0

typedef struct Dl_DCI
{
	u32	m_format	:1;             	//1bit ����format0��format1A�ı�־
	u32 map_vrb		:1;					//1bit LVRB����DVRBӳ�䷽ʽ������
  u32	m_riv      	:13;                //13bit:when band==20MHZ type2
	u32	m_mcs		:5;               	//5bit  ���Ʊ����ʽ
	u32	num_harq	:4; 				//4bit:HARQ���̺���
	u32	m_ndi		:1;       		    //1bit �����ݱ�־
	u32	m_rv		:2;            		//2bit HARQ RV��Ϣ
	u32	m_tpc		:2;                	//2bit ���ʿ�����Ϣ
	u32	m_dai		:2;               	//2bit ���з�����Ŀ�ı�ʶ
	u32	emptybits	:1;					//��ϵͳ������32bit������
}__attribute__((packed))Data_Dl_DCI;// structure of DL DCI format 1A

/*������1C
typedef struct RAR_DCI
{
    u16	m_gap  		:1;                 //1bit ����DVRB Gap1����Gap2
    u16	RIV       	:9;                //9bit:when band==20MHZ
    u16	I_TBS		:5;                 //5bit tbsize��Ź�32�ֿ��ܣ���������Сָʾ�����
    u16 emptybits	:1;				//��ϵͳ������32bit������  
}__attribute__((packed))Data_RAR_DCI;// DCI format 1C

*/

typedef struct ENBMAC_TO_PHY_RARDCI
{
	u16	m_rnti;
	u32	m_tbsize; //PDSCHTBSIZE
	u8 cfi;
	u8 dci_format;
	Data_Dl_DCI rar_dci;
}__attribute__((packed))ENBMAC_TO_PHY_Rardci;//LHLmodified 20141013

typedef struct ENBMAC_TO_PHY_DLSCHEDULE
{
	u16	m_rnti;
	u32	m_tbsize;//PDSCHTBSIZE
	u8 cfi;
	u8 dci_format;//DCI1A
	Data_Dl_DCI s_dl_dci;
}__attribute__((packed))ENBMAC_TO_PHY_DLscheduleDCI;//LHLmodified 20141008

typedef struct ENBMAC_TO_PHY_ULSCHEDULE
{
	u16	m_rnti;
	u32	m_tbsize;
	u8 dci_format;//DCI0
	Data_Ul_DCI s_ul_dci;
}__attribute__((packed))ENBMAC_TO_PHY_ULscheduleDCI;//LHLmodified 20141008


/*
*****************  ��������   *****************
*/
static unsigned int BufferSizeLevelBsrTable[] ={
  0, 10, 12, 14, 17, 19, 22, 26, 31, 36, 42, 49, 57, 67, 78, 91,
  107, 125, 146, 171, 200, 234, 274, 321, 376, 440, 515, 603,
  706, 826, 967, 1132, 1326, 1552, 1817, 2127, 2490, 2915, 3413,
  3995, 4677, 5476, 6411, 7505, 8787, 10287, 12043, 14099, 16507,
  19325, 22624, 26487, 31009, 36304, 42502, 49759, 58255,
  68201, 79846, 93749, 109439, 128125, 150000, 150000
};
static unsigned int McsforCqi[16] = {
0, 0, 0, 2, 4, 6, 8, 11, 13, 15, 18, 20, 22, 24, 26, 28};
static unsigned int Ul_ModulationSchemeForMcs[29] = {
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  6, 6, 6, 6, 6, 6, 6, 6
};
static unsigned int Dl_ModulationSchemeForMcs[32] = {
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  4, 4, 4, 4, 4, 4, 4,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  2,  // reserved
  4,  // reserved
  6,  // reserved
};
static unsigned int Dl_McsToItbsize[29] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 18,
  19, 20, 21, 22, 23, 24, 25, 26
};
static unsigned int Ul_McsToItbsize[29] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  19, 20, 21, 22, 23, 24, 25, 26
};
//Format 1C
/*
static unsigned int TBSizeTableofFormat1c[32]={
  40, 56, 72, 120, 136, 144, 176, 208, 224, 256, 280, 296, 328, 336, 392, 488, 552,
  600, 632, 696, 776, 840, 904, 1000, 1064, 1128, 1224, 1288, 1384, 1480, 1608, 1736
};
*/	
//normal
static unsigned int TransportBlockSizeTable[110][27] = {

  /* NPRB 001*/ { 16, 24, 32, 40, 56, 72, 88, 104, 120, 136, 144, 176, 208, 224, 256, 280, 328, 336, 376, 408, 440, 488, 520, 552, 584, 616, 712},
  /* NPRB 002*/ { 32, 56, 72, 104, 120, 144, 176, 224, 256, 296, 328, 376, 440, 488, 552, 600, 632, 696, 776, 840, 904, 1000, 1064, 1128, 1192, 1256, 1480},
  /* NPRB 003*/ { 56, 88, 144, 176, 208, 224, 256, 328, 392, 456, 504, 584, 680, 744, 840, 904, 968, 1064, 1160, 1288, 1384, 1480, 1608, 1736, 1800, 1864, 2216},
  /* NPRB 004*/ { 88, 144, 176, 208, 256, 328, 392, 472, 536, 616, 680, 776, 904, 1000, 1128, 1224, 1288, 1416, 1544, 1736, 1864, 1992, 2152, 2280, 2408, 2536, 2984},
  /* NPRB 005*/ { 120, 176, 208, 256, 328, 424, 504, 584, 680, 776, 872, 1000, 1128, 1256, 1416, 1544, 1608, 1800, 1992, 2152, 2344, 2472, 2664, 2856, 2984, 3112, 3752},
  /* NPRB 006*/ { 152, 208, 256, 328, 408, 504, 600, 712, 808, 936, 1032, 1192, 1352, 1544, 1736, 1800, 1928, 2152, 2344, 2600, 2792, 2984, 3240, 3496, 3624, 3752, 4392},
  /* NPRB 007*/ { 176, 224, 296, 392, 488, 600, 712, 840, 968, 1096, 1224, 1384, 1608, 1800, 1992, 2152, 2280, 2536, 2792, 2984, 3240, 3496, 3752, 4008, 4264, 4392, 5160},
  /* NPRB 008*/ { 208, 256, 328, 440, 552, 680, 808, 968, 1096, 1256, 1384, 1608, 1800, 2024, 2280, 2472, 2600, 2856, 3112, 3496, 3752, 4008, 4264, 4584, 4968, 5160, 5992},
  /* NPRB 009*/ { 224, 328, 376, 504, 632, 776, 936, 1096, 1256, 1416, 1544, 1800, 2024, 2280, 2600, 2728, 2984, 3240, 3624, 3880, 4136, 4584, 4776, 5160, 5544, 5736, 6712},
  /* NPRB 010*/ { 256, 344, 424, 568, 696, 872, 1032, 1224, 1384, 1544, 1736, 2024, 2280, 2536, 2856, 3112, 3240, 3624, 4008, 4264, 4584, 4968, 5352, 5736, 5992, 6200, 7480},
  /* NPRB 011*/ { 288, 376, 472, 616, 776, 968, 1128, 1320, 1544, 1736, 1928, 2216, 2472, 2856, 3112, 3368, 3624, 4008, 4392, 4776, 5160, 5544, 5992, 6200, 6712, 6968, 8248},
  /* NPRB 012*/ { 328, 424, 520, 680, 840, 1032, 1224, 1480, 1672, 1864, 2088, 2408, 2728, 3112, 3496, 3624, 3880, 4392, 4776, 5160, 5544, 5992, 6456, 6968, 7224, 7480, 8760},
  /* NPRB 013*/ { 344, 456, 568, 744, 904, 1128, 1352, 1608, 1800, 2024, 2280, 2600, 2984, 3368, 3752, 4008, 4264, 4776, 5160, 5544, 5992, 6456, 6968, 7480, 7992, 8248, 9528},
  /* NPRB 014*/ { 376, 488, 616, 808, 1000, 1224, 1480, 1672, 1928, 2216, 2472, 2792, 3240, 3624, 4008, 4264, 4584, 5160, 5544, 5992, 6456, 6968, 7480, 7992, 8504, 8760, 10296},
  /* NPRB 015*/ { 392, 520, 648, 872, 1064, 1320, 1544, 1800, 2088, 2344, 2664, 2984, 3368, 3880, 4264, 4584, 4968, 5352, 5992, 6456, 6968, 7480, 7992, 8504, 9144, 9528, 11064},
  /* NPRB 016*/ { 424, 568, 696, 904, 1128, 1384, 1672, 1928, 2216, 2536, 2792, 3240, 3624, 4136, 4584, 4968, 5160, 5736, 6200, 6968, 7480, 7992, 8504, 9144, 9912, 10296, 11832},
  /* NPRB 017*/ { 456, 600, 744, 968, 1192, 1480, 1736, 2088, 2344, 2664, 2984, 3496, 3880, 4392, 4968, 5160, 5544, 6200, 6712, 7224, 7992, 8504, 9144, 9912, 10296, 10680, 12576},
  /* NPRB 018*/ { 488, 632, 776, 1032, 1288, 1544, 1864, 2216, 2536, 2856, 3112, 3624, 4136, 4584, 5160, 5544, 5992, 6456, 7224, 7736, 8248, 9144, 9528, 10296, 11064, 11448, 13536},
  /* NPRB 019*/ { 504, 680, 840, 1096, 1352, 1672, 1992, 2344, 2664, 2984, 3368, 3880, 4392, 4968, 5544, 5736, 6200, 6712, 7480, 8248, 8760, 9528, 10296, 11064, 11448, 12216, 14112},
  /* NPRB 020*/ { 536, 712, 872, 1160, 1416, 1736, 2088, 2472, 2792, 3112, 3496, 4008, 4584, 5160, 5736, 6200, 6456, 7224, 7992, 8504, 9144, 9912, 10680, 11448, 12216, 12576, 14688},
  /* NPRB 021*/ { 568, 744, 936, 1224, 1480, 1864, 2216, 2536, 2984, 3368, 3752, 4264, 4776, 5352, 5992, 6456, 6712, 7480, 8248, 9144, 9912, 10680, 11448, 12216, 12960, 13536, 15264},
  /* NPRB 022*/ { 600, 776, 968, 1256, 1544, 1928, 2280, 2664, 3112, 3496, 3880, 4392, 4968, 5736, 6200, 6712, 7224, 7992, 8760, 9528, 10296, 11064, 11832, 12576, 13536, 14112, 16416},
  /* NPRB 023*/ { 616, 808, 1000, 1320, 1608, 2024, 2408, 2792, 3240, 3624, 4008, 4584, 5352, 5992, 6456, 6968, 7480, 8248, 9144, 9912, 10680, 11448, 12576, 12960, 14112, 14688, 16992},
  /* NPRB 024*/ { 648, 872, 1064, 1384, 1736, 2088, 2472, 2984, 3368, 3752, 4264, 4776, 5544, 6200, 6968, 7224, 7736, 8760, 9528, 10296, 11064, 12216, 12960, 13536, 14688, 15264, 17568},
  /* NPRB 025*/ { 680, 904, 1096, 1416, 1800, 2216, 2600, 3112, 3496, 4008, 4392, 4968, 5736, 6456, 7224, 7736, 7992, 9144, 9912, 10680, 11448, 12576, 13536, 14112, 15264, 15840, 18336},
  /* NPRB 026*/ { 712, 936, 1160, 1480, 1864, 2280, 2728, 3240, 3624, 4136, 4584, 5352, 5992, 6712, 7480, 7992, 8504, 9528, 10296, 11064, 12216, 12960, 14112, 14688, 15840, 16416, 19080},
  /* NPRB 027*/ { 744, 968, 1192, 1544, 1928, 2344, 2792, 3368, 3752, 4264, 4776, 5544, 6200, 6968, 7736, 8248, 8760, 9912, 10680, 11448, 12576, 13536, 14688, 15264, 16416, 16992, 19848},
  /* NPRB 028*/ { 776, 1000, 1256, 1608, 1992, 2472, 2984, 3368, 3880, 4392, 4968, 5736, 6456, 7224, 7992, 8504, 9144, 10296, 11064, 12216, 12960, 14112, 15264, 15840, 16992, 17568, 20616},
  /* NPRB 029*/ { 776, 1032, 1288, 1672, 2088, 2536, 2984, 3496, 4008, 4584, 5160, 5992, 6712, 7480, 8248, 8760, 9528, 10296, 11448, 12576, 13536, 14688, 15840, 16416, 17568, 18336, 21384},
  /* NPRB 030*/ { 808, 1064, 1320, 1736, 2152, 2664, 3112, 3624, 4264, 4776, 5352, 5992, 6712, 7736, 8504, 9144, 9912, 10680, 11832, 12960, 14112, 15264, 16416, 16992, 18336, 19080, 22152},
  /* NPRB 031*/ { 840, 1128, 1384, 1800, 2216, 2728, 3240, 3752, 4392, 4968, 5544, 6200, 6968, 7992, 8760, 9528, 9912, 11064, 12216, 13536, 14688, 15840, 16992, 17568, 19080, 19848, 22920},
  /* NPRB 032*/ { 872, 1160, 1416, 1864, 2280, 2792, 3368, 3880, 4584, 5160, 5736, 6456, 7224, 8248, 9144, 9912, 10296, 11448, 12576, 13536, 14688, 15840, 16992, 18336, 19848, 20616, 23688},
  /* NPRB 033*/ { 904, 1192, 1480, 1928, 2344, 2856, 3496, 4008, 4584, 5160, 5736, 6712, 7480, 8504, 9528, 10296, 10680, 11832, 12960, 14112, 15264, 16416, 17568, 19080, 19848, 20616, 24496},
  /* NPRB 034*/ { 936, 1224, 1544, 1992, 2408, 2984, 3496, 4136, 4776, 5352, 5992, 6968, 7736, 8760, 9912, 10296, 11064, 12216, 13536, 14688, 15840, 16992, 18336, 19848, 20616, 21384, 25456},
  /* NPRB 035*/ { 968, 1256, 1544, 2024, 2472, 3112, 3624, 4264, 4968, 5544, 6200, 6968, 7992, 9144, 9912, 10680, 11448, 12576, 14112, 15264, 16416, 17568, 19080, 19848, 21384, 22152, 25456},
  /* NPRB 036*/ { 1000, 1288, 1608, 2088, 2600, 3112, 3752, 4392, 4968, 5736, 6200, 7224, 8248, 9144, 10296, 11064, 11832, 12960, 14112, 15264, 16992, 18336, 19080, 20616, 22152, 22920, 26416},
  /* NPRB 037*/ { 1032, 1352, 1672, 2152, 2664, 3240, 3880, 4584, 5160, 5736, 6456, 7480, 8504, 9528, 10680, 11448, 12216, 13536, 14688, 15840, 16992, 18336, 19848, 21384, 22920, 23688, 27376},
  /* NPRB 038*/ { 1032, 1384, 1672, 2216, 2728, 3368, 4008, 4584, 5352, 5992, 6712, 7736, 8760, 9912, 11064, 11832, 12216, 13536, 15264, 16416, 17568, 19080, 20616, 22152, 22920, 24496, 28336},
  /* NPRB 039*/ { 1064, 1416, 1736, 2280, 2792, 3496, 4136, 4776, 5544, 6200, 6712, 7736, 8760, 9912, 11064, 11832, 12576, 14112, 15264, 16992, 18336, 19848, 21384, 22152, 23688, 24496, 29296},
  /* NPRB 040*/ { 1096, 1416, 1800, 2344, 2856, 3496, 4136, 4968, 5544, 6200, 6968, 7992, 9144, 10296, 11448, 12216, 12960, 14688, 15840, 16992, 18336, 19848, 21384, 22920, 24496, 25456, 29296},
  /* NPRB 041*/ { 1128, 1480, 1800, 2408, 2984, 3624, 4264, 4968, 5736, 6456, 7224, 8248, 9528, 10680, 11832, 12576, 13536, 14688, 16416, 17568, 19080, 20616, 22152, 23688, 25456, 26416, 30576},
  /* NPRB 042*/ { 1160, 1544, 1864, 2472, 2984, 3752, 4392, 5160, 5992, 6712, 7480, 8504, 9528, 10680, 12216, 12960, 13536, 15264, 16416, 18336, 19848, 21384, 22920, 24496, 25456, 26416, 30576},
  /* NPRB 043*/ { 1192, 1544, 1928, 2536, 3112, 3752, 4584, 5352, 5992, 6712, 7480, 8760, 9912, 11064, 12216, 12960, 14112, 15264, 16992, 18336, 19848, 21384, 22920, 24496, 26416, 27376, 31704},
  /* NPRB 044*/ { 1224, 1608, 1992, 2536, 3112, 3880, 4584, 5352, 6200, 6968, 7736, 8760, 9912, 11448, 12576, 13536, 14112, 15840, 17568, 19080, 20616, 22152, 23688, 25456, 26416, 28336, 32856},
  /* NPRB 045*/ { 1256, 1608, 2024, 2600, 3240, 4008, 4776, 5544, 6200, 6968, 7992, 9144, 10296, 11448, 12960, 13536, 14688, 16416, 17568, 19080, 20616, 22920, 24496, 25456, 27376, 28336, 32856},
  /* NPRB 046*/ { 1256, 1672, 2088, 2664, 3240, 4008, 4776, 5736, 6456, 7224, 7992, 9144, 10680, 11832, 12960, 14112, 14688, 16416, 18336, 19848, 21384, 22920, 24496, 26416, 28336, 29296, 34008},
  /* NPRB 047*/ { 1288, 1736, 2088, 2728, 3368, 4136, 4968, 5736, 6456, 7480, 8248, 9528, 10680, 12216, 13536, 14688, 15264, 16992, 18336, 20616, 22152, 23688, 25456, 27376, 28336, 29296, 35160},
  /* NPRB 048*/ { 1320, 1736, 2152, 2792, 3496, 4264, 4968, 5992, 6712, 7480, 8504, 9528, 11064, 12216, 13536, 14688, 15840, 17568, 19080, 20616, 22152, 24496, 25456, 27376, 29296, 30576, 35160},
  /* NPRB 049*/ { 1352, 1800, 2216, 2856, 3496, 4392, 5160, 5992, 6968, 7736, 8504, 9912, 11064, 12576, 14112, 15264, 15840, 17568, 19080, 21384, 22920, 24496, 26416, 28336, 29296, 31704, 36696},
  /* NPRB 050*/ { 1384, 1800, 2216, 2856, 3624, 4392, 5160, 6200, 6968, 7992, 8760, 9912, 11448, 12960, 14112, 15264, 16416, 18336, 19848, 21384, 22920, 25456, 27376, 28336, 30576, 31704, 36696},
  /* NPRB 051*/ { 1416, 1864, 2280, 2984, 3624, 4584, 5352, 6200, 7224, 7992, 9144, 10296, 11832, 12960, 14688, 15840, 16416, 18336, 19848, 22152, 23688, 25456, 27376, 29296, 31704, 32856, 37888},
  /* NPRB 052*/ { 1416, 1864, 2344, 2984, 3752, 4584, 5352, 6456, 7224, 8248, 9144, 10680, 11832, 13536, 14688, 15840, 16992, 19080, 20616, 22152, 24496, 26416, 28336, 29296, 31704, 32856, 37888},
  /* NPRB 053*/ { 1480, 1928, 2344, 3112, 3752, 4776, 5544, 6456, 7480, 8248, 9144, 10680, 12216, 13536, 15264, 16416, 16992, 19080, 21384, 22920, 24496, 26416, 28336, 30576, 32856, 34008, 39232},
  /* NPRB 054*/ { 1480, 1992, 2408, 3112, 3880, 4776, 5736, 6712, 7480, 8504, 9528, 11064, 12216, 14112, 15264, 16416, 17568, 19848, 21384, 22920, 25456, 27376, 29296, 30576, 32856, 34008, 40576},
  /* NPRB 055*/ { 1544, 1992, 2472, 3240, 4008, 4776, 5736, 6712, 7736, 8760, 9528, 11064, 12576, 14112, 15840, 16992, 17568, 19848, 22152, 23688, 25456, 27376, 29296, 31704, 34008, 35160, 40576},
  /* NPRB 056*/ { 1544, 2024, 2536, 3240, 4008, 4968, 5992, 6712, 7736, 8760, 9912, 11448, 12576, 14688, 15840, 16992, 18336, 20616, 22152, 24496, 26416, 28336, 30576, 31704, 34008, 35160, 40576},
  /* NPRB 057*/ { 1608, 2088, 2536, 3368, 4136, 4968, 5992, 6968, 7992, 9144, 9912, 11448, 12960, 14688, 16416, 17568, 18336, 20616, 22920, 24496, 26416, 28336, 30576, 32856, 35160, 36696, 42368},
  /* NPRB 058*/ { 1608, 2088, 2600, 3368, 4136, 5160, 5992, 6968, 7992, 9144, 10296, 11832, 12960, 14688, 16416, 17568, 19080, 20616, 22920, 25456, 27376, 29296, 31704, 32856, 35160, 36696, 42368},
  /* NPRB 059*/ { 1608, 2152, 2664, 3496, 4264, 5160, 6200, 7224, 8248, 9144, 10296, 11832, 13536, 15264, 16992, 18336, 19080, 21384, 23688, 25456, 27376, 29296, 31704, 34008, 36696, 37888, 43816},
  /* NPRB 060*/ { 1672, 2152, 2664, 3496, 4264, 5352, 6200, 7224, 8504, 9528, 10680, 12216, 13536, 15264, 16992, 18336, 19848, 21384, 23688, 25456, 28336, 30576, 32856, 34008, 36696, 37888, 43816},
  /* NPRB 061*/ { 1672, 2216, 2728, 3624, 4392, 5352, 6456, 7480, 8504, 9528, 10680, 12216, 14112, 15840, 17568, 18336, 19848, 22152, 24496, 26416, 28336, 30576, 32856, 35160, 36696, 39232, 45352},
  /* NPRB 062*/ { 1736, 2280, 2792, 3624, 4392, 5544, 6456, 7480, 8760, 9912, 11064, 12576, 14112, 15840, 17568, 19080, 19848, 22152, 24496, 26416, 29296, 31704, 34008, 35160, 37888, 39232, 45352},
  /* NPRB 063*/ { 1736, 2280, 2856, 3624, 4584, 5544, 6456, 7736, 8760, 9912, 11064, 12576, 14112, 16416, 18336, 19080, 20616, 22920, 24496, 27376, 29296, 31704, 34008, 36696, 37888, 40576, 46888},
  /* NPRB 064*/ { 1800, 2344, 2856, 3752, 4584, 5736, 6712, 7736, 9144, 10296, 11448, 12960, 14688, 16416, 18336, 19848, 20616, 22920, 25456, 27376, 29296, 31704, 34008, 36696, 39232, 40576, 46888},
  /* NPRB 065*/ { 1800, 2344, 2856, 3752, 4584, 5736, 6712, 7992, 9144, 10296, 11448, 12960, 14688, 16992, 18336, 19848, 21384, 23688, 25456, 28336, 30576, 32856, 35160, 37888, 39232, 40576, 48936},
  /* NPRB 066*/ { 1800, 2408, 2984, 3880, 4776, 5736, 6968, 7992, 9144, 10296, 11448, 13536, 15264, 16992, 19080, 20616, 21384, 23688, 26416, 28336, 30576, 32856, 35160, 37888, 40576, 42368, 48936},
  /* NPRB 067*/ { 1864, 2472, 2984, 3880, 4776, 5992, 6968, 8248, 9528, 10680, 11832, 13536, 15264, 16992, 19080, 20616, 22152, 24496, 26416, 29296, 31704, 34008, 36696, 37888, 40576, 42368, 48936},
  /* NPRB 068*/ { 1864, 2472, 3112, 4008, 4968, 5992, 6968, 8248, 9528, 10680, 11832, 13536, 15264, 17568, 19848, 20616, 22152, 24496, 27376, 29296, 31704, 34008, 36696, 39232, 42368, 43816, 51024},
  /* NPRB 069*/ { 1928, 2536, 3112, 4008, 4968, 5992, 7224, 8504, 9528, 11064, 12216, 14112, 15840, 17568, 19848, 21384, 22152, 24496, 27376, 29296, 31704, 35160, 36696, 39232, 42368, 43816, 51024},
  /* NPRB 070*/ { 1928, 2536, 3112, 4136, 4968, 6200, 7224, 8504, 9912, 11064, 12216, 14112, 15840, 18336, 19848, 21384, 22920, 25456, 27376, 30576, 32856, 35160, 37888, 40576, 42368, 43816, 52752},
  /* NPRB 071*/ { 1992, 2600, 3240, 4136, 5160, 6200, 7480, 8760, 9912, 11064, 12576, 14112, 16416, 18336, 20616, 22152, 22920, 25456, 28336, 30576, 32856, 35160, 37888, 40576, 43816, 45352, 52752},
  /* NPRB 072*/ { 1992, 2600, 3240, 4264, 5160, 6200, 7480, 8760, 9912, 11448, 12576, 14688, 16416, 18336, 20616, 22152, 23688, 26416, 28336, 30576, 34008, 36696, 39232, 40576, 43816, 45352, 52752},
  /* NPRB 073*/ { 2024, 2664, 3240, 4264, 5160, 6456, 7736, 8760, 10296, 11448, 12960, 14688, 16416, 19080, 20616, 22152, 23688, 26416, 29296, 31704, 34008, 36696, 39232, 42368, 45352, 46888, 55056},
  /* NPRB 074*/ { 2088, 2728, 3368, 4392, 5352, 6456, 7736, 9144, 10296, 11832, 12960, 14688, 16992, 19080, 21384, 22920, 24496, 26416, 29296, 31704, 34008, 36696, 40576, 42368, 45352, 46888, 55056},
  /* NPRB 075*/ { 2088, 2728, 3368, 4392, 5352, 6712, 7736, 9144, 10680, 11832, 12960, 15264, 16992, 19080, 21384, 22920, 24496, 27376, 29296, 32856, 35160, 37888, 40576, 43816, 45352, 46888, 55056},
  /* NPRB 076*/ { 2088, 2792, 3368, 4392, 5544, 6712, 7992, 9144, 10680, 11832, 13536, 15264, 17568, 19848, 22152, 23688, 24496, 27376, 30576, 32856, 35160, 37888, 40576, 43816, 46888, 48936, 55056},
  /* NPRB 077*/ { 2152, 2792, 3496, 4584, 5544, 6712, 7992, 9528, 10680, 12216, 13536, 15840, 17568, 19848, 22152, 23688, 25456, 27376, 30576, 32856, 35160, 39232, 42368, 43816, 46888, 48936, 57336},
  /* NPRB 078*/ { 2152, 2856, 3496, 4584, 5544, 6968, 8248, 9528, 11064, 12216, 13536, 15840, 17568, 19848, 22152, 23688, 25456, 28336, 30576, 34008, 36696, 39232, 42368, 45352, 46888, 48936, 57336},
  /* NPRB 079*/ { 2216, 2856, 3496, 4584, 5736, 6968, 8248, 9528, 11064, 12576, 14112, 15840, 18336, 20616, 22920, 24496, 25456, 28336, 31704, 34008, 36696, 39232, 42368, 45352, 48936, 51024, 57336},
  /* NPRB 080*/ { 2216, 2856, 3624, 4776, 5736, 6968, 8248, 9912, 11064, 12576, 14112, 16416, 18336, 20616, 22920, 24496, 26416, 29296, 31704, 34008, 36696, 40576, 43816, 45352, 48936, 51024, 59256},
  /* NPRB 081*/ { 2280, 2984, 3624, 4776, 5736, 7224, 8504, 9912, 11448, 12960, 14112, 16416, 18336, 20616, 22920, 24496, 26416, 29296, 31704, 35160, 37888, 40576, 43816, 46888, 48936, 51024, 59256},
  /* NPRB 082*/ { 2280, 2984, 3624, 4776, 5992, 7224, 8504, 9912, 11448, 12960, 14688, 16416, 19080, 21384, 23688, 25456, 26416, 29296, 32856, 35160, 37888, 40576, 43816, 46888, 51024, 52752, 59256},
  /* NPRB 083*/ { 2280, 2984, 3752, 4776, 5992, 7224, 8760, 10296, 11448, 12960, 14688, 16992, 19080, 21384, 23688, 25456, 27376, 30576, 32856, 35160, 39232, 42368, 45352, 46888, 51024, 52752, 61664},
  /* NPRB 084*/ { 2344, 3112, 3752, 4968, 5992, 7480, 8760, 10296, 11832, 13536, 14688, 16992, 19080, 21384, 24496, 25456, 27376, 30576, 32856, 36696, 39232, 42368, 45352, 48936, 51024, 52752, 61664},
  /* NPRB 085*/ { 2344, 3112, 3880, 4968, 5992, 7480, 8760, 10296, 11832, 13536, 14688, 16992, 19080, 22152, 24496, 26416, 27376, 30576, 34008, 36696, 39232, 42368, 45352, 48936, 52752, 55056, 61664},
  /* NPRB 086*/ { 2408, 3112, 3880, 4968, 6200, 7480, 9144, 10680, 12216, 13536, 15264, 17568, 19848, 22152, 24496, 26416, 28336, 30576, 34008, 36696, 40576, 43816, 46888, 48936, 52752, 55056, 63776},
  /* NPRB 087*/ { 2408, 3240, 3880, 5160, 6200, 7736, 9144, 10680, 12216, 13536, 15264, 17568, 19848, 22152, 25456, 26416, 28336, 31704, 34008, 37888, 40576, 43816, 46888, 51024, 52752, 55056, 63776},
  /* NPRB 088*/ { 2472, 3240, 4008, 5160, 6200, 7736, 9144, 10680, 12216, 14112, 15264, 17568, 19848, 22920, 25456, 27376, 28336, 31704, 35160, 37888, 40576, 43816, 46888, 51024, 52752, 55056, 63776},
  /* NPRB 089*/ { 2472, 3240, 4008, 5160, 6456, 7736, 9144, 11064, 12576, 14112, 15840, 18336, 20616, 22920, 25456, 27376, 29296, 31704, 35160, 37888, 42368, 45352, 48936, 51024, 55056, 57336, 66592},
  /* NPRB 090*/ { 2536, 3240, 4008, 5352, 6456, 7992, 9528, 11064, 12576, 14112, 15840, 18336, 20616, 22920, 25456, 27376, 29296, 32856, 35160, 39232, 42368, 45352, 48936, 51024, 55056, 57336, 66592},
  /* NPRB 091*/ { 2536, 3368, 4136, 5352, 6456, 7992, 9528, 11064, 12576, 14112, 15840, 18336, 20616, 23688, 26416, 28336, 29296, 32856, 36696, 39232, 42368, 45352, 48936, 52752, 55056, 57336, 66592},
  /* NPRB 092*/ { 2536, 3368, 4136, 5352, 6456, 7992, 9528, 11448, 12960, 14688, 16416, 18336, 21384, 23688, 26416, 28336, 30576, 32856, 36696, 39232, 42368, 46888, 48936, 52752, 57336, 59256, 68808},
  /* NPRB 093*/ { 2600, 3368, 4136, 5352, 6712, 8248, 9528, 11448, 12960, 14688, 16416, 19080, 21384, 23688, 26416, 28336, 30576, 34008, 36696, 40576, 43816, 46888, 51024, 52752, 57336, 59256, 68808},
  /* NPRB 094*/ { 2600, 3496, 4264, 5544, 6712, 8248, 9912, 11448, 12960, 14688, 16416, 19080, 21384, 24496, 27376, 29296, 30576, 34008, 37888, 40576, 43816, 46888, 51024, 55056, 57336, 59256, 68808},
  /* NPRB 095*/ { 2664, 3496, 4264, 5544, 6712, 8248, 9912, 11448, 13536, 15264, 16992, 19080, 21384, 24496, 27376, 29296, 30576, 34008, 37888, 40576, 43816, 46888, 51024, 55056, 57336, 61664, 71112},
  /* NPRB 096*/ { 2664, 3496, 4264, 5544, 6968, 8504, 9912, 11832, 13536, 15264, 16992, 19080, 22152, 24496, 27376, 29296, 31704, 35160, 37888, 40576, 45352, 48936, 51024, 55056, 59256, 61664, 71112},
  /* NPRB 097*/ { 2728, 3496, 4392, 5736, 6968, 8504, 10296, 11832, 13536, 15264, 16992, 19848, 22152, 25456, 28336, 29296, 31704, 35160, 37888, 42368, 45352, 48936, 52752, 55056, 59256, 61664, 71112},
  /* NPRB 098*/ { 2728, 3624, 4392, 5736, 6968, 8760, 10296, 11832, 13536, 15264, 16992, 19848, 22152, 25456, 28336, 30576, 31704, 35160, 39232, 42368, 45352, 48936, 52752, 57336, 59256, 61664, 73712},
  /* NPRB 099*/ { 2728, 3624, 4392, 5736, 6968, 8760, 10296, 12216, 14112, 15840, 17568, 19848, 22920, 25456, 28336, 30576, 31704, 35160, 39232, 42368, 46888, 48936, 52752, 57336, 61664, 63776, 73712},
  /* NPRB 100*/ { 2792, 3624, 4584, 5736, 7224, 8760, 10296, 12216, 14112, 15840, 17568, 19848, 22920, 25456, 28336, 30576, 32856, 36696, 39232, 43816, 46888, 51024, 55056, 57336, 61664, 63776, 75376},
  /* NPRB 101*/ { 2792, 3752, 4584, 5992, 7224, 8760, 10680, 12216, 14112, 15840, 17568, 20616, 22920, 26416, 29296, 30576, 32856, 36696, 40576, 43816, 46888, 51024, 55056, 57336, 61664, 63776, 75376},
  /* NPRB 102*/ { 2856, 3752, 4584, 5992, 7224, 9144, 10680, 12576, 14112, 16416, 18336, 20616, 23688, 26416, 29296, 31704, 32856, 36696, 40576, 43816, 46888, 51024, 55056, 59256, 61664, 63776, 75376},
  /* NPRB 103*/ { 2856, 3752, 4584, 5992, 7480, 9144, 10680, 12576, 14688, 16416, 18336, 20616, 23688, 26416, 29296, 31704, 34008, 36696, 40576, 43816, 48936, 51024, 55056, 59256, 63776, 66592, 75376},
  /* NPRB 104*/ { 2856, 3752, 4584, 5992, 7480, 9144, 10680, 12576, 14688, 16416, 18336, 21384, 23688, 26416, 29296, 31704, 34008, 37888, 40576, 45352, 48936, 52752, 57336, 59256, 63776, 66592, 75376},
  /* NPRB 105*/ { 2984, 3880, 4776, 6200, 7480, 9144, 11064, 12960, 14688, 16416, 18336, 21384, 23688, 27376, 30576, 31704, 34008, 37888, 42368, 45352, 48936, 52752, 57336, 59256, 63776, 66592, 75376},
  /* NPRB 106*/ { 2984, 3880, 4776, 6200, 7480, 9528, 11064, 12960, 14688, 16992, 18336, 21384, 24496, 27376, 30576, 32856, 34008, 37888, 42368, 45352, 48936, 52752, 57336, 61664, 63776, 66592, 75376},
  /* NPRB 107*/ { 2984, 3880, 4776, 6200, 7736, 9528, 11064, 12960, 15264, 16992, 19080, 21384, 24496, 27376, 30576, 32856, 35160, 39232, 42368, 46888, 48936, 52752, 57336, 61664, 66592, 68808, 75376},
  /* NPRB 108*/ { 2984, 4008, 4776, 6200, 7736, 9528, 11448, 12960, 15264, 16992, 19080, 22152, 24496, 27376, 30576, 32856, 35160, 39232, 42368, 46888, 51024, 55056, 59256, 61664, 66592, 68808, 75376},
  /* NPRB 109*/ { 2984, 4008, 4968, 6456, 7736, 9528, 11448, 13536, 15264, 16992, 19080, 22152, 24496, 28336, 31704, 34008, 35160, 39232, 43816, 46888, 51024, 55056, 59256, 61664, 66592, 68808, 75376},
  /* NPRB 110*/ { 3112, 4008, 4968, 6456, 7992, 9528, 11448, 13536, 15264, 17568, 19080, 22152, 25456, 28336, 31704, 34008, 35160, 39232, 43816, 46888, 51024, 55056, 59256, 63776, 66592, 71112, 75376}
};




/*
*****************  ��������   *****************
*/
/**** About control RNTI  *****/

void init_rnti_array_val(void);
void delete_overtime_tc_rnti(void);
int Do_judge_rnti(u16 receive_rnti);
void convert_TcRntiToCRnti(u16 receive_rnti);
void recall_rnti(u16 receive_rnti);
void release_UE_rnti(u16 receive_rnti);


void init_Bitmap(void);

void Reset_Prb_dlBitmap(void);
void Reset_Prb_ulBitmap(void);

/**** About random access  *****/
void Do_Data_of_MacRarPdu_ulgrant(DATA_RAR_PDU * p_rar_pdu, struct S_RAinfo *p_ra_UEinfo,int index);//
int Do_RAR_DCI1A(struct S_RAinfo *ra_UEinfo);
int Do_RAR_DCI0(struct S_RAinfo *ra_UEinfo);

/**** About UL-schedule  *****/
//int Order_Ul_Sched(struct S_UEinfo *p_ul_UEinfo);
int Order_Ul_Sched(u16 user_rnti,ENBMAC_TO_PHY_ULscheduleDCI *p_ul_ici_dci,int ue_number ,u16 ue_index);
void Add_sueinfo_Node(PHYtoMAC_Info *temp, int m_bsr, int type_bsr);

/**** About DL-schedule  *****/
int Do_sched_algorithm(u16 rnti, int num_ue, int index);
int Do_cal_datasize(int index);
void Order_Dl_Sched(RLC_Request *mac_buffer, int num_ue, int index);
void *getUeNum(void);	//add in 20151229



#endif
