#include "CDemoFile.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "SDK/Public/Tier1/CUtlMemory.h"
#include <vector>

CDemoFile::CDemoFile() :
	m_pBuffer(NULL),
	m_bAllowHeaderWrite(true),
	m_bIsStreamBuffer(false)
{
}

CDemoFile::~CDemoFile()
{
	if (IsOpen())
	{
		Close();
	}
}

bool CDemoFile::IsOpen()
{
	return m_pBuffer && m_pBuffer->IsValid();
}

void CDemoFile::Close()
{
	// CUtlBuffer base class does NOT have a virtual destructor!
	if (m_bIsStreamBuffer)
	{
		// Destructor will call Close() as needed
		// //We wont call this, so close it.
		//delete static_cast<CUtlStreamBuffer*>(m_pBuffer);
	}
	else
	{
		delete m_pBuffer;
	}
	m_pBuffer = NULL;
}

bool CDemoFile::Open(const char* name, bool bReadOnly, bool bMemoryBuffer, int nBufferSize/*=0*/, bool bAllowHeaderWrite/*=true*/)
{
	// I have no fucking clue how tf2 opens the actual demo from the disk, its unreadable, so im going to just do it myself lmfao
	//CUtlBuffer( const void* pBuffer, int size, int nFlags = 0 );
	// it reads the name given as binary.
	std::string basePath = "F:\\Users\\Mitch\\Documents\\Coding Stuff\\My Projects or Things I Contribute to\\DemoReader\\x64\\Debug\\";
	auto finalPath = basePath + name;
	auto fullPathName = std::filesystem::path(finalPath);
	std::ifstream input(fullPathName, std::ios::in | std::ios::binary);
	if (input.is_open() == false)
	{
		// waht the fok
	}

	// get length of file:
	input.seekg(0, std::ios::end);
	long long length = (long long)input.tellg() + 1LL;
	auto lengthint = (int)length + 1;

	std::unique_ptr<char[]> buffer = std::make_unique< char[] >(length);

	memset(buffer.get(), 0LL, length);

	input.seekg(0, std::ios::beg);
	input.read(buffer.get(), length - 1LL);
	input.close();

	CUtlBuffer* buf = new CUtlBuffer(buffer.get(), length, 0);

	m_pBuffer = buf;

	if (m_pBuffer && m_pBuffer->IsValid())
	{
		//ConMsg("CDemoFile::Open: file already open.\n");
		return false;
	}

	m_szFileName[0] = 0;  // clear name
	memset(&m_DemoHeader, 0, sizeof(m_DemoHeader)); // and demo header

	// This is used by replay, which manually writes a header.
	m_bAllowHeaderWrite = bAllowHeaderWrite;

	if (bMemoryBuffer)
	{
		//Assert(!bReadOnly);	// Only read from files
		//Assert(nBufferSize > 0);
		m_pBuffer = new CUtlBuffer(nBufferSize, nBufferSize, 0);
		m_bIsStreamBuffer = false;
	}
	else
	{
		////wont be called, so commented out
		//m_pBuffer = new CUtlStreamBuffer(name, NULL, bReadOnly ? CUtlBuffer::READ_ONLY : 0, false);
		//m_bIsStreamBuffer = true;
	}

	// Demo files are always little endian
	m_pBuffer->SetBigEndian(false);

	if (!m_pBuffer || !m_pBuffer->IsValid())
	{
		//ConMsg("CDemoFile::Open: couldn't open file %s for %s.\n",
		//	   name, bReadOnly ? "reading" : "writing");
		Close();
		return false;
	}

	if (name)
	{
		// void special_V_strncpy(char* pDest, char const* pSrc, int maxLen)
		auto maxLen = sizeof(m_szFileName);
		strncpy(m_szFileName, name, maxLen);
		if (sizeof(m_szFileName) > 0)
		{
			m_szFileName[maxLen - 1] = 0;
		}
	}

	return true;
}

demoheader_t* CDemoFile::ReadDemoHeader()
{
	// I have no fucking clue how tf2 opens the actual demo from the disk, its unreadable, so im going to just do it myself lmfao
	//CUtlBuffer( const void* pBuffer, int size, int nFlags = 0 );
	// it reads the name given as binary.
	std::string basePath = "F:\\Users\\Mitch\\Documents\\Coding Stuff\\My Projects or Things I Contribute to\\DemoReader\\x64\\Debug\\";
	auto finalPath = basePath + "original.dem";
	auto fullPathName = std::filesystem::path(finalPath);
	std::ifstream input(fullPathName, std::ios::in | std::ios::binary);
	if (input.is_open() == false)
	{
		// waht the fok

	}

	// get length of file:
	input.seekg(0, std::ios::end);
	long long length = (long long)input.tellg() + 1LL;
	auto lengthint = (int)length + 1;

	std::unique_ptr<char[]> buffer = std::make_unique< char[] >(length);

	memset(buffer.get(), 0LL, length);

	input.seekg(0, std::ios::beg);
	input.read(buffer.get(), length - 1LL);
	input.close();

	CUtlBuffer* buf = new CUtlBuffer(buffer.get(), length, 0x8);

	m_pBuffer = buf;


	bool bOk; 
	memset(&m_DemoHeader, 0, sizeof(m_DemoHeader));

	if (!m_pBuffer || !m_pBuffer->IsValid())
		return NULL;
	m_pBuffer->SeekGet(CUtlBuffer::SEEK_HEAD, 0);
	m_pBuffer->Get(&m_DemoHeader, sizeof(demoheader_t));
	bOk = m_pBuffer->IsValid();

	ByteSwap_demoheader_t(m_DemoHeader);

	if (!bOk)
		return NULL;  // reading failed

	if (strcmp(m_DemoHeader.demofilestamp, DEMO_HEADER_ID))
	{
		std::cout << m_szFileName << " has invalid demo header ID.\n";
		return NULL;
	}

	if (m_DemoHeader.networkprotocol != PROTOCOL_VERSION
#if defined( DEMO_BACKWARDCOMPATABILITY )
		&& m_DemoHeader.networkprotocol < PROTOCOL_VERSION_12
#endif
		)
	{
		std::cout << "ERROR: demo network protocol " << m_DemoHeader.networkprotocol << " outdated, engine version is " << PROTOCOL_VERSION << "\n";
		return NULL;
	}

	if ((m_DemoHeader.demoprotocol > DEMO_PROTOCOL) ||
		(m_DemoHeader.demoprotocol < 2))
	{
		std::cout << "ERROR: demo file protocol " << m_DemoHeader.demoprotocol << "outdated, engine vnoteersion is " << DEMO_PROTOCOL << "\n";

		return NULL;
	}

	return &m_DemoHeader;
}