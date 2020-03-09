#pragma pack(push,1)
struct netHeader
{
	BYTE code;					// ���� �ڵ�
	WORD len;					// ��Ŷ�� ���� (��� ����)
	BYTE randCode;		// XOR code
	BYTE checkSum;		// checkSum
};
#pragma pack(pop)

struct Session
{
	Session()
	{
		Sock = INVALID_SOCKET;
		Index = 0;

		recvFlag = 0;
		sendFlag = 0;
		usingFlag = 0;
		disconnectFlag = 0;
		sendCount = 0;
		sendDisconnectFlag = 0;
		sendPQCS = 0;

	}
	SOCKET  Sock;
	unsigned __int64 Index;

	// IOCP �۾� ���� ������

	volatile LONG recvFlag;				// Recv ������ üũ. 
	volatile LONG sendFlag;				// send ���� �� üũ.
	volatile LONG usingFlag;				// � �����忡�� ������ ����ϰ� �ִ��� üũ.
	volatile LONG disconnectFlag;		// disconnect üũ

	LONG sendCount;				// send�� ����ȭ ������ �� 
	volatile LONG sendDisconnectFlag;	// ������ ���� üũ
	volatile LONG sendPQCS;				// PQCS�� �̿��ؼ� SEND �Լ� ȣ��� ȣ�⿩�� üũ

	winBuffer recvQ;
	boost::lockfree::queue<Sbuf*> sendQ;
	boost::lockfree::queue<Sbuf*> completeSendQ;
	OVERLAPPED recvOver, sendOver;
};


class virtualIOCP
{
private:
	Session *sessionArr;		// session ���� �迭. �����ڿ��� �����Ҵ�

	// Counter
	LONG clientCounter;		// ���� Connect�� Client ��
	int	limitCount;		// �ʴ� Connect ����
	int maxClient;					// Max client count.
	int threadCount;				// worker ��
	LONG connectFailCount;

	// Handle
	HANDLE hcp;					// IOCP HANDLE
	HANDLE *threadArr;
	HANDLE connectThreadHandle;
	
	// Connect 
	char	IP[16];
	unsigned short	Port;
	SOCKADDR_IN addr;
	LINGER lingerOption;
	bool nagleOption;

	// Encryption Key
	BYTE Code, Key1, Key2;

private:
	LONG connectTPS;				// �ʴ� Connect Ƚ�� 
	LONG sendTPS;					// �ʴ� Send Ƚ��
	LONG recvTPS;					// �ʴ� Recv Ƚ��

public:
	boost::lockfree::stack<unsigned __int64> *connectStack;
	boost::lockfree::stack<unsigned __int64> *indexStack;

	// Test
	LONG connectStackCount;

private:
	static unsigned __stdcall connectThread(LPVOID _data);			// ACCEPT �۾��� �ϴ� ������
	static unsigned __stdcall workerThread(LPVOID _data);		// IOCP ������

	Session* insertSession(SOCKET _sock, unsigned __int64 _Index);

	void loadConfig(const char *_configData);

private:
	void recvPost(Session *_ss);
	void sendPost(Session *_ss);
	void completeRecv(LONG _trnas, Session *_ss);
	void completeSend(LONG _trnas, Session *_ss);

	void clientShutdown(Session *_ss);
	void disconnect(Session *_ss);
	void disconnect(SOCKET _sock);

	Session* acquirLock(unsigned __int64 _index);
	void releaseLock(Session *_ss);

protected:
	bool quit;
	bool ctQuit;
	LONG connectTotal;
	LONG pconnectTPS;
	LONG psendTPS;
	LONG precvTPS;

protected:
	LONG getConnectTotal(void);
	LONG getConnectFail(void);
	LONG getConnetTPS(void);
	LONG getSendTPS(void);
	LONG getRecvTPS(void);
	void setTPS(void);

	bool callConnect(void);

protected:
	bool		Start(const char *_configData);
	bool		Stop(void);					
	LONG	GetClientCount(void);
	void		SendPacket(unsigned __int64 _index, Sbuf *_buf, bool _type = false);

	void clientShutdown(unsigned __int64 _Index);		// �ڽ� Ŭ�������� ȣ���ϴ� �Լ� 

	virtual void OnClientJoin(unsigned __int64 _Index) = 0;	// accept -> ����ó�� �Ϸ� �� ȣ��
	virtual void OnClientLeave(unsigned __int64 _Index) = 0;		// disconnect �� ȣ��
	virtual bool OnConnectionRequest(char *_ip, unsigned short _port) = 0; // accept �� [false : Ŭ���̾�Ʈ �ź� / true : ���� ���]
	virtual void OnRecv(unsigned __int64 _Index, Sbuf *_buf) = 0;		// ���� �Ϸ� ��

	virtual void OnError(int _errorCode, WCHAR *_string) = 0;		// �����޼��� ����
};

