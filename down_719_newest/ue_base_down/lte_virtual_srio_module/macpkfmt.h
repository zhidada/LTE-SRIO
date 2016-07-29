
//#include "../lte_system.h"

#ifndef _MACPKFMT_H
#define _MACPKFMT_H



/*UE interface information MAC-RLC */
typedef enum PBCH{PCCH = 1, BCCH =2, ELSE = 0}PBCH;

typedef struct RLCtoMAC_IciMsg{
      u16 rnti; // UE sign
	  u16 len; // the length of the subheaders corresponding to the SDUs from RLC
}__attribute__((packed))RLCtoMAC_IciMsg;

typedef struct MACtoRLC_IciMsg{  //modified by HQ 20140504 
      u16 rnti; // UE sign
	 PBCH pbch; //PBCh = 1 indicate PCCH ; PBCh = 2 indicate BCCH; 0:else
	  u8 lcid; //logical channel ID
}__attribute__((packed))MACtoRLC_IciMsg;
                   
/*typedef struct MACtoRLC_IciMsg{
	  u8 type;   // type =  2 
      u16 rnti; // UE sign
	  u8 lcid; //logical channel ID
}__attribute__((packed))MACtoRLC_IciMsg;

typedef struct RLC_MAC_IciMsgPB{  //����  ����ICI��UE��Ӧ������MAC����RLC��
	  u8 type;      // type = 1
      u16 rnti;    // UE sign
	  PBCH pbCh;  //PBCh = 1 indicate PCCH ; PBCh = 2 indicate BCCH; 0:else
}__attribute__((packed))RLC_MAC_IciMsgPB;

*/
/*eNB�� RLC-MAC interface
typedef struct RLCtoMAC_IciMsg{
	  PBCH pbCh;  //PBCh = 1 indicate PCCH ; PBCh = 2 indicate BCCH; 0:else      
      u16 rnti; // UE sign
	  u16 len; // the length of the subheaders corresponding to the SDUs from RLC
}__attribute__((packed))RLCtoMAC_IciMsg;

typedef struct MACtoRLC_IciMsg{
      u16 rnti; // UE sign
	  u8 lcid; //logical channel ID
}__attribute__((packed))MACtoRLC_IciMsg;
*/

/*interface information from PHY-adapter */
/*
typedef struct MACtoPHYadapter_IciMsg
{
   	 u16  frameNo ;       //system frame number
  	 u16  subframeNo ;    // subframe number for now
  	 u16 rnti ;         // UE ID
      u8 tcid;         // the type of Transport channel
     u16 MessageType ; // the type of packet

}__attribute__((packed))MACtoPHYadapter_IciMsg;

typedef struct PHYadaptertoMAC_IciMsg
{
      u8 tcid;         // the type of Transport channel
     u16 MessageType ; // the type of packet
     u16 rnti ; 
}__attribute__((packed))PHYadaptertoMAC_IciMsg;
#define PCH 0
#define BCH 1
#define DLSCH 2
#define MCH 3
*/

/*MAC���Ƶ�Ԫ��ʽ MAC_CE*/
/*shortBSR  longBSR C_RNTI UECR TACMD PW DRX */
/*************HeXi identify******************/
#define CLIENT_SIZE 5
//#define rapreambleID -1		//����һ��Ӧ���е�ȫ�ֱ��
#define LCG_SIZE 4
#define MACCE_ARRAY_size 8
//#define RLC_ICI_ARRAY_size 8
#define CCCH_LCID 0
#define CONTENTION_RESOLUTION_LCID 28
#define TIMING_ADVANCE_LCID 29
#define DRX_LCID 30
#define PADDING_LCID 31
#define PHR_LCID 26
#define C_RNTI_LCID 27
#define SHORT_BSR 29
#define LONG_BSR 30


typedef struct lteMacSubhead7Bit_s{
	unsigned char m_lcid_e_r_r;	//LCID 5bits,E 1bit,R 1bit
	unsigned char m_f_l;	//F 1bit,L 7bits
}__attribute__((packed))MAC_SDU_subhead_7bit;	//7bit��ͷ

typedef struct lteMacSubhead15Bit_s{
	unsigned char m_lcid_e_r_r;	//LCID 5bits,E 1bit,R 1bit
	unsigned char m_f_l;	//F 1bit,L 7bits
	unsigned char m_l;		//L 8bits
}__attribute__((packed))MAC_SDU_subhead_15bit;	//15bit��ͷ

typedef struct lteMacLastSubhead_s{
	unsigned char m_lcid_e_r_r;	//LCID 5bits,E 1bit,R 1bit
}__attribute__((packed))MAC_SDU_subhead_last;

typedef struct lteMacShortBsrControlSdu_s{
	unsigned char m_lcgid_buffersize;	//LCG ID 2bits,Buffer size 6bits
}__attribute__((packed))MAC_CE_shortBSR;	//��BSR

typedef struct lteMacLongBsrControlSdu_s{
	unsigned char m_buffersize1;	//Buffer Size #0~3 6bits
	unsigned short  m_buffersize2;
}__attribute__((packed))MAC_CE_longBSR;	//��BSR

typedef struct lteMacC_rntiControlSdu_s{
	unsigned short m_crnti;	//C_RNTI 16bits
}__attribute__((packed))MAC_CE_Crnti;	//c_rnti���Ƶ�Ԫ

typedef struct lteMacUeContentionResolutionIdentityControlSdu_s{
	unsigned int m_resolusion_identity_1;	//UE Contention Resolusion Identity 48bits
	unsigned short m_resolusion_identity_2;
}__attribute__((packed))MAC_CE_content_resolution_identity;	//���������֤���Ƶ�Ԫ

typedef struct lteMacTimingAdvanceCommandControlSdu_s{
	unsigned char m_r_r_command;	//R 1bit,Timing Advance Command 6bits
}__attribute__((packed))MAC_CE_time_advance;	//��ʱ��ǰ������Ƶ�Ԫ

typedef struct lteMacPowerHeadroomControlSdu_s{
	unsigned char m_r_r_ph;	//R 1bit,PH 6bits
}__attribute__((packed))MAC_CE_power_headroom;	//�����������Ƶ�Ԫ

typedef struct lteMacPaddingSubhead_s{
	unsigned char m_lcid_e_r_r;	//LCID 5bits,E 1bit,R 1bit
}__attribute__((packed))MAC_SH_padding;	//padding��ͷ��

typedef struct lteMacRarSdu_s{
	unsigned char m_r_ta;
	unsigned char m_ta_ulgrant;
	unsigned short m_ulgrant;
	unsigned short m_tcrnti;
}__attribute__((packed))MAC_RAR_sdu;	//RAR SDU

typedef struct lteMacRarSubheadWithBi_s
{
	unsigned char m_e_t_r_r_bi;
}__attribute__((packed))MAC_RAR_subhead_withbi;	//RAR ��ͷ��

typedef struct lteMacRarSubhead_s{
	unsigned char m_e_t_rapid;
}__attribute__((packed))MAC_RAR_subhead;





struct Data_of_MacRarPdu{	
	unsigned short    m_ta;
	unsigned char     m_bi;
	unsigned char     m_rapid;
	unsigned int    m_ul_grant;
	unsigned short    m_tcrnti;
	struct list_head m_listnode;
};

/**********HeXi 20140423*******************/
typedef struct MacCeListElements_s  // MAC����eNB��BSR�ṹ��ȫ�֣���ÿ�η�����Դ�Լ�RLC��������Ҫ����( ��Ϊ����Ҫ���е�һЩ��־)
{
  uint16_t  m_rnti;	
	int  m_lcgnum;//��־�ж��ٸ�LCG������Ҫ����
	int m_TotalLcgData;
  int  m_bufferStatus[4];
}__attribute__((packed))MacCeListElement_s;	//they should have been provided by Liu Hanli
/*
typedef struct RRCConnectionRequest_s{	
	
	int type;     //pe=1:s_TMSI,  type=2:randomValue  //RLCʵ��ʱ���Լ����ӵ��ֶ�
	union InitialUE_Identity{
		//struct S_TMSI s_TMSI;	//there is no such a struct in the .h profile but there should be one!!!!!
		long randomValue;         //BIT STRING (SIZE (40))
	}ue_Identity;
	
	enum EstablishmentCause{
		emergency=0, 
		highPriorityAccess=1, 
		mt_Access=2, 
		mo_Signalling=3, 
		mo_Data=4		
	} establishmentCause;	
}__attribute__((packed))RRCConnectionRequest;
*/
typedef struct temp_contention_resolution_s{
	unsigned int m_part_1;
	unsigned short m_part_2;
}__attribute__((packed))temp_contention_resolution;	//���������Ԫ���� ������msg4�Ա�
/*****************HeXi 20140423 end **************/


/*************HeXi identify  end ******************/

/******* compelx and decomplex __HeXi 20140423*******/
bool decomplexRarPdu(FSM_PKT *skb);
bool lteMacCEC_rntiSend(MAC_CE_Crnti *m_crnti);
bool lteMacCEPhSend(MAC_CE_power_headroom *m_power_headroom);
int lteMacCcchSend(FSM_PKT *skb,int head_len);
 int lteMacCEBSRSend();
void setMACtoRLC_IciMsg(MACtoRLC_IciMsg *ici_msg,PBCH pbch,int lcid);
//void setMACtoPHY_IciMsg(MACtoPHYadapter_IciMsg *ici_msg,int tcid);
//static  int lteMacGetLcgId(int bsr_flag); //�����˵�δ����
//static  int lteMacGetBufSize(int bsr_flag);
//static void lteMacGetBufSizes(int bsr_flag,int buf_sizes[]);
void decomplexUeDataPdu(FSM_PKT *skb);
void complexUeMacPdu(FSM_PKT *skb);
int getRapreambleID();
void clean_CE_tags();

void MACCETA_handle(unsigned char ta_index);
void Pdcch_c_rnti(unsigned short c_rnti);


#endif


