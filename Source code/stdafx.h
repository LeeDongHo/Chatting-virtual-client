// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma comment(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <mstcpip.h>
#include <locale>
#include <codecvt>
#include <vector>
#include <cstdarg>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include "edhConfig.h"
#include "ChattingProtocol.h"
#include "MonitorProtocol.h"

#include "APIHook.h"
#include "CCrashDump.h"
#include "log.h"
#include "resourceGetter.h"

#include "boost\lockfree\queue.hpp"
#include "boost\lockfree\stack.hpp"

#include "winQueue.h"
#include "Sbuf.h"
#include "memoryPool.h"

#include "LanClient.h"
#include "MonitorClient.h"

#include "VirtualIOCP.h"
#include "virtualClient.h"
