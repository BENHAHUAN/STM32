#include "includes.h"

/* Exported types ------------------------------------------------------------*/
/**
* @brief  Type of received communication frame
*/
typedef enum
{
    TYPE_INVALID_MIN = (uint8_t)0,
    TYPE_GET_VER,			 /*!< User System get version of this node. */
    TYPE_TX_RF_DATA, 		 /*!< User System send data that need to TX by RF. */
    TYPE_SET_RF_SETTINGS, 	 /*!< User System set settings of RF. */
    TYPE_GET_RF_SETTINGS, 	 /*!< User System get settings of RF. */
    TYPE_GET_NET_SETTINGS,	  /*!< User System get network settings of this node. */
    TYPE_QUIT_NET, 			  /*!< User System set this node quit from current network. */
    TYPE_SET_TX_PWR, 		  /*!< User System set TX power of RF. */
    TYPE_GET_TX_PWR,		  /*!< User System get TX power of RF. */
    TYPE_WAKE_ACK, 			  /*!< User System send wake ack to node. */
    TYPE_GET_PKT_RSSI, 		  /*!< User System get the RSSI of the latest packet received. */
    TYPE_INVALID_MAX,

    TYPE_WAKE_DATA = ((uint8_t)0xC0), /*!< Node send wake data to User System. */
} COMM_FRAME_TYPE_TypeDef;


/* Exported variables ------------------------------------------------------- */
typedef struct
{
    uint8_t    byHead;
    COMM_FRAME_TYPE_TypeDef    eType;
    uint8_t    byDataSize;
} COMM_FRAME_HEAD;
typedef struct
{
    uint8_t    byCS;
    uint8_t    byTail;
} COMM_FRAME_TAIL;

/* UPLINK=Only support uplink; UPLINK_WAKE=Support uplink and wake */
#define UPLINK    1
#define UPLINK_WAKE    2
#define CUR_SYST    UPLINK_WAKE

/* Private macros ------------------------------------------------------------*/
#define MAX_LEN_COMM_TRM_DATA    255u
#if (UPLINK == CUR_SYST)
#define MAX_LEN_UART_FRAME_DATA    \
    ( MAX_LEN_COMM_TRM_DATA - sizeof(COMM_FRAME_HEAD) -    \
      sizeof(COMM_FRAME_TAIL) - sizeof(NetAddr_t) ) /* 2 bytes for NodeAddr. */
#else
#define MAX_LEN_UART_FRAME_DATA    \
    ( MAX_LEN_COMM_TRM_DATA - sizeof(COMM_FRAME_HEAD) -    \
      sizeof(COMM_FRAME_TAIL) - sizeof(NetAddr_t) - sizeof(int8_t)) /* 2 bytes for NodeAddr, 1 byte for RSSI. */
#endif
/* Device unique ID */
typedef uint32_t    DEV_ID;

typedef uint16_t    NetAddr_t;

/**
* @brief  Configure for wireless network.
*/
typedef struct
{
    uint8_t    byUplinkPayload; 	/* Bytes of the uplink payload of node. */
    int8_t     chRepeatNum; 		/* Repeat number of node uplink. */
    uint8_t    byWakeDataSize; 		/* Size of wake data. */
    uint8_t    byWakeAckSize; 		/* Size of wake ack. */
    int16_t    nNodeNum; 			/* Number of node in this subnet. */
    uint16_t   wSlotLen; 			/* Slot length, the unit is millisecond. */
    int32_t    lUplinkPeriod;		/* milliseconds, the interval between 2 uplinked. */
    int32_t    lWakeInterval; 		/* milliseconds, the interval between 2 waked. */
} WNetConf_t;

/**
* @brief  E:Node request join into network.
* @brief  C:�ڵ������������(8B).
*/
typedef struct
{
	uint8_t		byFlag;		//0x01
	uint8_t		byNetVer;	//0=�ն˵�һ�γ�����255=���ػָ��������ã�1~254=�������ն����������Ÿ�����1B
	DEV_ID		tDevID;		//ȡ��MCU�����к�(4B)
	uint16_t    wCRC16;		//��ǰ�棨�������⣩�������ݽ��м��㡣(2B)
}RF_FRAME_REQ_JOIN;

/**
* @brief  E:Sink response the Node join into network.
* @brief  C:sink��Ӧ������Ϣ(14B)
*/
typedef struct
{
    uint8_t    byFlag;		//0x81
    uint8_t    byNetVer;	//0=�ն˵�һ�γ�����255=���ػָ��������ã�1~254=�������ն����������Ÿ�����1B
    NetAddr_t  tAddr;		//�����ַ2B
    DEV_ID     tDevID;		//ȡ��MCU�����к�4B
    int32_t    lCurRTC; 	/* milliseconds, Current RTC of Sink. 4B*/
    uint16_t   wCRC16;    	//2B
} RF_FRAME_RESP_JOIN;

/**
* @brief  E:Sink response the Node join into network that have different version.
* @brief  C:Sink��Node����������汾�Ų���ͬ����Ӧ����֡��ʽ���£�26B��
*/
typedef struct
{
    uint8_t      byFlag;	//0x82	
    uint8_t      byNetVer;	//0=�ն˵�һ�γ�����255=���ػָ��������ã�1~254=�������ն����������Ÿ�����1B
    NetAddr_t    tAddr;		//�����ַ2B
    DEV_ID       tDevID;	//ȡ��MCU�����к�4B
    int32_t      lCurRTC; 	/* milliseconds, Current RTC of Sink. */
    WNetConf_t   stWNetConf;
    uint16_t     wCRC16;    
} RF_FRAME_RESP_JOIN_DIFF_VER;

/**
* @brief  E:Node uplink data to Sink.
* @brief  C:�ڵ��ϱ�����
*/
typedef struct
{
    uint8_t    byFlag;			//0x02
#if (UPLINK_WAKE == CUR_SYST)
    uint8_t    byNetVer;    	//NetVer��Node�����յ�����������汾��1B
#endif
    NetAddr_t    tAddr;			//2B
    uint8_t    a_byDataBuf[MAX_LEN_UART_FRAME_DATA];	//DataPayload������֡���ܳ���ȥ3�ֽڣ�Tag+NodeAddr�����������ֵΪ247��
} RF_FRAME_UPLINK_DATA;

/**
* @brief  E:Sink ACK to node that uplink data.
* @brief  C:Sink��Ӧ�ϱ�����
*/
typedef struct
{
    uint8_t    byFlag;			//0x82
#if (UPLINK_WAKE == CUR_SYST)	//
    uint8_t    byNetVer;    	
#endif
    NetAddr_t    tAddr;			//2B
    int32_t    lCurRTC; 		/* milliseconds, Current RTC of Sink. */
    uint16_t    wCRC16;    		//CRC16��ǰ�棨�������⣩�������ݽ��м��㡣
} RF_FRAME_UPLINK_ACK;

/**
* @brief  Sink ACK to node that uplink data, have different network version.
* @brief  Sink��Node����������汾�Ų���ͬ����Ӧ����֡��ʽ��26B��
*/
typedef struct
{
    uint8_t    byFlag;			//0x82
    uint8_t    byNetVer;		
    NetAddr_t    tAddr;
    int32_t    lCurRTC; 		/* milliseconds, Current RTC of Sink. */
    WNetConf_t    stWNetConf;
    uint16_t    wCRC16;    
} RF_FRAME_UPLINK_ACK_DIFF_VER;

/**
* @brief  E:wake address frame.
* @brief  C:��������֡
*/
typedef struct
{
    NetAddr_t    tAddr; 	 /* 0x0000=����, 0xFFFF=�㲥��ַ, 0x0001~0xFFFE=������ַ. */
    uint8_t      byDataTime; /* 1bit=32ms, �ȴ�ʱ�� */
} RF_FRAME_WAKE_ADDR;

/**
* @brief  E:Sink send broadcast data frame to Node.
* @brief  C:Sink�㲥����֡��MAX=248B��
*/
typedef struct
{
    uint8_t    byFlag;
    uint8_t    a_byDataBuf[MAX_LEN_UART_FRAME_DATA];
} RF_FRAME_BC_DATA;

/**
* @brief  Sink send broadcast time frame to Node.
* @brief  Sink�㲥Уʱ��7B��
*/
typedef struct
{
    uint8_t    byFlag;
    int32_t    lCurRTC; /* milliseconds, Current RTC of Sink. */
    uint16_t    wCRC16;    
} RF_FRAME_BC_TIME;

/**
* @brief  Sink send broadcast network settings frame to Node.
* @brief  . Sink�㲥�������������20B��
*/
typedef struct
{
    uint8_t    byFlag;
    uint8_t    byNetVer;    
    WNetConf_t    stWNetConf;
    uint16_t    wCRC16;    
} RF_FRAME_BC_NET_SETTINGS;

/**
* @brief  Sink send unicast data frame to Node.
* @brief  Sink���͵�������֡��MAX=248��
*/
typedef RF_FRAME_BC_DATA    RF_FRAME_UC_DATA;

/**
* @brief  Node response wake data ack frame to Sink.
* @brief  Node��Ӧ��������֡��MAX=250��
*/
typedef struct
{
    uint8_t 	 byFlag;
    NetAddr_t    tAddr;			//������֡�����NodeAddr��Ϊ��ʵ�ֵķ��㣬����Sinkϵͳ��Ҫ�����ȴ�����ͨ��ȫ���̡�
    uint8_t      a_byDataBuf[MAX_LEN_UART_FRAME_DATA];
} RF_FRAME_UC_DATA_ACK;












