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
* @brief  C:节点申请加入网络(8B).
*/
typedef struct
{
	uint8_t		byFlag;		//0x01
	uint8_t		byNetVer;	//0=终端第一次出厂，255=网关恢复出厂设置，1~254=网关与终端网络参数序号更迭。1B
	DEV_ID		tDevID;		//取自MCU的序列号(4B)
	uint16_t    wCRC16;		//对前面（除自身外）所有数据进行计算。(2B)
}RF_FRAME_REQ_JOIN;

/**
* @brief  E:Sink response the Node join into network.
* @brief  C:sink回应入网信息(14B)
*/
typedef struct
{
    uint8_t    byFlag;		//0x81
    uint8_t    byNetVer;	//0=终端第一次出厂，255=网关恢复出厂设置，1~254=网关与终端网络参数序号更迭。1B
    NetAddr_t  tAddr;		//网络地址2B
    DEV_ID     tDevID;		//取自MCU的序列号4B
    int32_t    lCurRTC; 	/* milliseconds, Current RTC of Sink. 4B*/
    uint16_t   wCRC16;    	//2B
} RF_FRAME_RESP_JOIN;

/**
* @brief  E:Sink response the Node join into network that have different version.
* @brief  C:Sink和Node的网络参数版本号不相同，回应数据帧格式如下（26B）
*/
typedef struct
{
    uint8_t      byFlag;	//0x82	
    uint8_t      byNetVer;	//0=终端第一次出厂，255=网关恢复出厂设置，1~254=网关与终端网络参数序号更迭。1B
    NetAddr_t    tAddr;		//网络地址2B
    DEV_ID       tDevID;	//取自MCU的序列号4B
    int32_t      lCurRTC; 	/* milliseconds, Current RTC of Sink. */
    WNetConf_t   stWNetConf;
    uint16_t     wCRC16;    
} RF_FRAME_RESP_JOIN_DIFF_VER;

/**
* @brief  E:Node uplink data to Sink.
* @brief  C:节点上报数据
*/
typedef struct
{
    uint8_t    byFlag;			//0x02
#if (UPLINK_WAKE == CUR_SYST)
    uint8_t    byNetVer;    	//NetVer是Node所接收到的网络参数版本号1B
#endif
    NetAddr_t    tAddr;			//2B
    uint8_t    a_byDataBuf[MAX_LEN_UART_FRAME_DATA];	//DataPayload是数据帧的总长减去3字节（Tag+NodeAddr），它的最大值为247。
} RF_FRAME_UPLINK_DATA;

/**
* @brief  E:Sink ACK to node that uplink data.
* @brief  C:Sink回应上报数据
*/
typedef struct
{
    uint8_t    byFlag;			//0x82
#if (UPLINK_WAKE == CUR_SYST)	//
    uint8_t    byNetVer;    	
#endif
    NetAddr_t    tAddr;			//2B
    int32_t    lCurRTC; 		/* milliseconds, Current RTC of Sink. */
    uint16_t    wCRC16;    		//CRC16对前面（除自身外）所有数据进行计算。
} RF_FRAME_UPLINK_ACK;

/**
* @brief  Sink ACK to node that uplink data, have different network version.
* @brief  Sink和Node的网络参数版本号不相同，回应数据帧格式（26B）
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
* @brief  C:唤醒数据帧
*/
typedef struct
{
    NetAddr_t    tAddr; 	 /* 0x0000=保留, 0xFFFF=广播地址, 0x0001~0xFFFE=单播地址. */
    uint8_t      byDataTime; /* 1bit=32ms, 等待时长 */
} RF_FRAME_WAKE_ADDR;

/**
* @brief  E:Sink send broadcast data frame to Node.
* @brief  C:Sink广播数据帧（MAX=248B）
*/
typedef struct
{
    uint8_t    byFlag;
    uint8_t    a_byDataBuf[MAX_LEN_UART_FRAME_DATA];
} RF_FRAME_BC_DATA;

/**
* @brief  Sink send broadcast time frame to Node.
* @brief  Sink广播校时（7B）
*/
typedef struct
{
    uint8_t    byFlag;
    int32_t    lCurRTC; /* milliseconds, Current RTC of Sink. */
    uint16_t    wCRC16;    
} RF_FRAME_BC_TIME;

/**
* @brief  Sink send broadcast network settings frame to Node.
* @brief  . Sink广播更新网络参数（20B）
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
* @brief  Sink发送单播数据帧（MAX=248）
*/
typedef RF_FRAME_BC_DATA    RF_FRAME_UC_DATA;

/**
* @brief  Node response wake data ack frame to Sink.
* @brief  Node回应单播数据帧（MAX=250）
*/
typedef struct
{
    uint8_t 	 byFlag;
    NetAddr_t    tAddr;			//该数据帧中添加NodeAddr是为了实现的方便，否则Sink系统需要阻塞等待唤醒通信全过程。
    uint8_t      a_byDataBuf[MAX_LEN_UART_FRAME_DATA];
} RF_FRAME_UC_DATA_ACK;












