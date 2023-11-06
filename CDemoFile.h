#pragma once

#include "SDK/Public/Tier1/utlbuffer.h"

#define MAX_OSPATH 260
#define PATH_MAX 260
#define FDEMO_NORMAL		0
#define FDEMO_USE_ORIGIN2	(1<<0)
#define FDEMO_USE_ANGLES2	(1<<1)
#define FDEMO_NOINTERP		(1<<2)	// don't interpolate between this an last view

#define DEMO_HEADER_ID		"HL2DEMO"
#define DEMO_PROTOCOL		3
// The current network protocol version.  Changing this makes clients and servers incompatible
#define PROTOCOL_VERSION    24

#define DEMO_BACKWARDCOMPATABILITY

// For backward compatibility of demo files (NET_MAX_PAYLOAD_BITS went away)
#define PROTOCOL_VERSION_23		23

// For backward compatibility of demo files (sound index bits used to = 13 )
#define PROTOCOL_VERSION_22		22

// For backward compatibility of demo files (before the special DSP was shipped to public)
#define PROTOCOL_VERSION_21		21

// For backward compatibility of demo files (old-style dynamic model loading)
#define PROTOCOL_VERSION_20		20

// For backward compatibility of demo files (post Halloween sound flag extra bit)
#define PROTOCOL_VERSION_19		19

// For backward compatibility of demo files (pre Halloween sound flag extra bit)
#define PROTOCOL_VERSION_18		18

// For backward compatibility of demo files (MD5 in map version)
#define PROTOCOL_VERSION_17		17

// For backward compatibility of demo files (create string tables compression flag)
#define PROTOCOL_VERSION_14		14

// For backward compatibility of demo files
#define PROTOCOL_VERSION_12		12

// The PROTOCOL_VERSION when replay shipped to public
#define PROTOCOL_VERSION_REPLAY		16

struct demoheader_t
{
	char	demofilestamp[8];				// Should be HL2DEMO
	int		demoprotocol;					// Should be DEMO_PROTOCOL
	int		networkprotocol;				// Should be PROTOCOL_VERSION
	char	servername[MAX_OSPATH];		// Name of server
	char	clientname[MAX_OSPATH];		// Name of client who recorded the game
	char	mapname[MAX_OSPATH];			// Name of map
	char	gamedirectory[MAX_OSPATH];	// Name of game directory (com_gamedir)
	float	playback_time;					// Time of track
	int     playback_ticks;					// # of ticks in track
	int     playback_frames;				// # of frames in track
	int		signonlength;					// length of sigondata in bytes
};

// ---------------------------------------------------------------------------- -
// Forward declarations
//-----------------------------------------------------------------------------
class IDemoBuffer;

//-----------------------------------------------------------------------------
// Demo file 
//-----------------------------------------------------------------------------
class CDemoFile
{
public:
	CDemoFile();
	~CDemoFile();

	bool	Open(const char* name, bool bReadOnly, bool bMemoryBuffer = false, int nBufferSize = 0, bool bAllowHeaderWrite = true);
	bool	IsOpen();
	void	Close();

	void	SeekTo(int position, bool bRead);
	unsigned int GetCurPos(bool bRead);
	int		GetSize();

	void	WriteRawData(const char* buffer, int length);
	int		ReadRawData(char* buffer, int length);

	void	WriteSequenceInfo(int nSeqNrIn, int nSeqNrOutAck);
	void	ReadSequenceInfo(int& nSeqNrIn, int& nSeqNrOutAck);

	//void	WriteCmdInfo(democmdinfo_t& info);
	//void	ReadCmdInfo(democmdinfo_t& info);

	void	WriteCmdHeader(unsigned char cmd, int tick);
	void	ReadCmdHeader(unsigned char& cmd, int& tick);

	void	WriteConsoleCommand(const char* cmd, int tick);
	const char* ReadConsoleCommand(void);

	//void	WriteNetworkDataTables(bf_write* buf, int tick);
	//int		ReadNetworkDataTables(bf_read* buf);
	//
	//void	WriteStringTables(bf_write* buf, int tick);
	//int		ReadStringTables(bf_read* buf);

	void	WriteUserCmd(int cmdnumber, const char* buffer, unsigned char bytes, int tick);
	int		ReadUserCmd(char* buffer, int& size);

	void	WriteDemoHeader();
	demoheader_t* ReadDemoHeader();

	//void	WriteFileBytes(FileHandle_t fh, int length);

	// Returns the PROTOCOL_VERSION used when .dem was recorded
	int		GetProtocolVersion();
public:
	char			m_szFileName[MAX_PATH];	//name of current demo file
	demoheader_t    m_DemoHeader;  //general demo info
	CUtlBuffer* m_pBuffer;
	bool			m_bAllowHeaderWrite;
	bool			m_bIsStreamBuffer;
};

inline void ByteSwap_demoheader_t(demoheader_t& swap)
{
	swap.demoprotocol = LittleDWord(swap.demoprotocol);
	swap.networkprotocol = LittleDWord(swap.networkprotocol);
	LittleFloat(&swap.playback_time, &swap.playback_time);
	swap.playback_ticks = LittleDWord(swap.playback_ticks);
	swap.playback_frames = LittleDWord(swap.playback_frames);
	swap.signonlength = LittleDWord(swap.signonlength);
}