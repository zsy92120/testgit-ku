#ifndef _ADT860API_H_
#define _ADT860API_H_


#if (DEV_SYSTEM == _DEV_WIN32_)
    #pragma pack(push,1)    // structure with 1 byte alignment
#endif

#define	HALFFIFO     		512

#define	START_CONVERSION 	0
#define	DA_DATA_LSB      	1
#define	CHANNEL_RANGE    	2
#define	DA_OUTPORT		   	3
#define	DA_DATA0_MSB		  4
#define	DA_DATA1_MSB		  5
#define	DA_DATA2_MSB		  6
#define	DA_DATA3_MSB		  7
#define	CLR_INT					  8
#define	INT_CTRL					9
#define	TIMER_FIFO_CTRL		10
#define	AD_CFG     				11



#define	AD_DATA_LSB     	0
#define	AD_DATA_MSB      	1
#define	CHANNEL_RANGER    2
#define	DA_INPORT     		3
#define	DA_UPDATE				  4
#define	ST_REG	          8
#define	INT_CTRLBK				9
#define	ST_FIFO     			10
#define	AD_FIFOBK     		11



#define	TIMER_0 	       	12  //page 0 RST_PAGE_CFG
#define	TIMER_1      			13
#define	TIMER_2	    			14
#define	TIMER_CTRL    		15

#define	CAL_DAVAl					12  //page 1 RST_PAGE_CFG
#define	CAL_DACH      		13
#define	CAL_CTRL	    		14
#define	EEP_CTRL    			15

#define	IO8255_A					12  //page 2 RST_PAGE_CFG
#define	IO8255_B      		13
#define	IO8255_C	    		14
#define	IO8255_CTRL    		15



typedef unsigned char	uchar;
typedef unsigned short	uint;
typedef unsigned short	u16;
typedef unsigned char	u8;
#define DAAUTOCAL_FIXED_OR_PROGRAMMABLE_DETECTION_TOLERANCE  100
#define AD_CAL_TOLERANCE 2


typedef	struct
{
	int		mode;       	// the mode
	int		valid;      	// if it's valid (1) or not (0)
	int		polarity;   	// polarity
	int		range;      	// range
	int		gain;       	// gain
	double	minvolt;  		// min voltage
	double	maxvolt;  		// max voltage
	int		midmux;     	// which mux channel is close to the middle
	int		fsmux;      	// which mux channel is close to FS
	int		dacmod[2];  	// which way to move the dac
} ADModeData;


typedef	struct
{
	unsigned char	modesel;   	//1--mode select, 0---port c bit operation
	unsigned char	modegrp0;   //0:mode0,1:mode1,x1:mode2;Group 0(porta , portch)mode select (bit[6:5])
	unsigned char	modegrp1;   //0:mode0,1:mode1;Group 1(portb , portcl)mode select (bit[2])
			
	unsigned char	dira;		//1:input,0:output
	unsigned char	dirb;
	unsigned char	dircl;	
	unsigned char	dirch;	
	unsigned char	paval;
	unsigned char	pbval;
	unsigned char	pclval;		//
	unsigned char	pchval;		//portch val moved to bit0		
}io8255;
typedef	struct
{
	unsigned short BaseAddr;
	unsigned short irqnum;
}resource;//baseAddress ,IRQ
extern "C" {
        __declspec(dllexport)  HANDLE	_stdcall	ADT860_DevLoad(int CardNo);
		__declspec(dllexport)  BOOL		_stdcall	ADT860_DevClose(HANDLE hDriver);
			__declspec(dllexport)  resource		_stdcall	ADT860_GetAddressAndIrq(HANDLE hDriver);
		__declspec(dllexport)  int		_stdcall	ADT860_Timer1FRQ(HANDLE hDriver, enum FRQCLK timer1frq);
		__declspec(dllexport)  int		_stdcall	ADT860_ClockMode(HANDLE hDriver, uchar clock, uchar mode);
		__declspec(dllexport)  int		_stdcall	ADT860_ClockDivisor(HANDLE hDriver, uchar clock, uint divisor);
		__declspec(dllexport)  unsigned int	_stdcall ADT860_ClockReadBack(HANDLE hDriver, uchar clock);
		__declspec(dllexport)  int		_stdcall	ADT860_RstADT(HANDLE hDriver);
		__declspec(dllexport)  int		_stdcall	ADT860_ClearIRQ(HANDLE hDriver);
		__declspec(dllexport)  int		_stdcall	ADT860_SelIRQSource(HANDLE hDriver, enum ADTIMER adtimer0);
		__declspec(dllexport)  int		_stdcall	ADT860_ADTrigSet(HANDLE hDriver, uchar trigflag);
		__declspec(dllexport)  int		_stdcall	ADT860_FIFOEN(HANDLE hDriver,uchar ENON);
		__declspec(dllexport)  int		_stdcall	ADT860_ADFIFOReset(HANDLE hDriver);
		__declspec(dllexport)  int		_stdcall	ADT860_StartConversion(HANDLE hDriver);
		__declspec(dllexport)  int		_stdcall	ADT860_ReadData(HANDLE hDriver, unsigned short *addata);
		__declspec(dllexport)  int		_stdcall	ADT860_SetChannel(HANDLE hDriver, uchar high, uchar low);
			__declspec(dllexport)  int		_stdcall	ADT860_SetEvent(HANDLE hDriver, HANDLE hEvent);
		__declspec(dllexport)  int		_stdcall	ADT860_ADConfig(HANDLE hDriver, uchar range, uchar polarity, uchar gain, uchar sedi);
		__declspec(dllexport)  int		_stdcall	ADT860_DAConvert(HANDLE hDriver, uchar ch, unsigned short daval);
		__declspec(dllexport)  int		_stdcall	ADT860_DACchdata(HANDLE hDriver, uchar chanl, uint daval);
		__declspec(dllexport)  int		_stdcall	ADT860_UpdateDAC(HANDLE hDriver);
		__declspec(dllexport)  int		_stdcall	ADT860_SetADBootMode(HANDLE hDriver, uchar admode);
		__declspec(dllexport)  int		_stdcall	ADT860_SetDABootMode(HANDLE hDriver, uchar damode);
		__declspec(dllexport)  int		_stdcall	ADT860_Iomd0Oper(HANDLE hDriver, io8255 * iostruct);
		__declspec(dllexport)  double	_stdcall	ADT860_ADCodeToVoltage(unsigned short code, int mode);
		__declspec(dllexport)  unsigned int	_stdcall ADT860_ADVoltageToADCode(double volt, int mode);		
		__declspec(dllexport)  int		_stdcall	ADT860_ReadIOPort(HANDLE hDriver, uchar portoff, uchar *portval);
		__declspec(dllexport)  int		_stdcall	ADT860_WriteIOPort(HANDLE hDriver, uchar portoff, uchar portval);

		__declspec(dllexport)  int		_stdcall	ADT860_ConfigIOPorts(HANDLE hDriver,unsigned char Port0Inout, unsigned char Port1Inout,unsigned char Port2up4Inout,unsigned char Port2low4Inout);
		__declspec(dllexport)  unsigned char	_stdcall	ADT860_ReadDigitIO(HANDLE hDriver, unsigned char InputPort); 
		__declspec(dllexport)  int		_stdcall	ADT860_WriteDigitIO(HANDLE hDriver,unsigned char OutputPort, unsigned char portval); 

}

#endif