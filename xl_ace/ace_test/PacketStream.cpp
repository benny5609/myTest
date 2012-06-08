
#include "PacketStream.h"

int PacketStream::m_head_size = sizeof(Header);

uint64 PacketStream::m_new_count = 0;
uint64 PacketStream::m_delete_count = 0;
ACE_Thread_Mutex PacketStream::m_new_mutex;
ACE_Thread_Mutex PacketStream::m_delete_mutex;
