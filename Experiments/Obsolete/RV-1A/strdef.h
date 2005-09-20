typedef struct {
	float	j1;
	float	j2;
	float	j3;
	float	j4;
	float	j5;
	float	j6;
	float	j7;
	float	j8;
} JOINT;

typedef struct {
	float	x;
	float	y;
	float	z;
	float	a;
	float	b;
	float	c;
	float	l1;
	float	l2;
} WORLD;

typedef struct{
	WORLD	w;
	unsigned int sflg1;
	unsigned int sflg2;
} POSE;

typedef struct{
	long	p1;
	long	p2;
	long	p3;
	long	p4;
	long	p5;
	long	p6;
	long	p7;
	long	p8;
} PULSE;

typedef struct enet_rtcmd_str {
	unsigned short Command;
#define MXT_CMD_NULL	0
#define MXT_CMD_MOVE	1
#define MXT_CMD_END		255

	unsigned short	SendType;
	unsigned short	RecvType;

#define MXT_TYP_NULL	0
#define MXT_TYP_POSE	1
#define MXT_TYP_JOINT	2
#define MXT_TYP_PULSE	3

#define MXT_TYP_FPOSE	4
#define MXT_TYP_FJOINT	5
#define MXT_TYP_FPULSE	6

#define MXT_TYP_DPOSE	7
#define MXT_TYP_DJOINT	8
#define MXT_TYP_DPULSE	9
#define MXT_TYP_DTOOL	10
#define MXT_TYP_ELCUR	11

	union rtdata{
		POSE	pos;
		JOINT	jnt;
		PULSE	pls;
	} dat;

	unsigned short SendIOType;
	unsigned short RecvIOType;

#define MXT_IO_NULL		0
#define MXT_IO_OUT		1
#define MXT_IO_IN		2

	unsigned short BitTop;
	unsigned short BitMask;
	unsigned short IoData;

	unsigned short TCount;
	unsigned long  CCount;

} MXTCMD;
