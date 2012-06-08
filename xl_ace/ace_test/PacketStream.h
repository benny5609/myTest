#ifndef _PACKAGESTREAM_HPP
#define _PACKAGESTREAM_HPP

#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include "typedefs.h"
#include "TraceLogger.h"

typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> ACE_SVC_HANDLER;

struct Header {
	Header (void) : opcode (0), size (0), guid(0L), compress(0) {}
	Header (uint32 o, uint32 s, uint64 g) : opcode (o), size (s), guid(g), compress(0) {}

	uint16 opcode;  /// ��Ϣ������
	uint16 size;    /// ��Ϣ���Ĵ�С
	uint64 guid;    /// ��Ҫ����Ŀ�Ķ˵�GUID
	uint8  compress;
};

#define STAT_NEW_COUNT \
	{	\
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_new_mutex, ) \
		++m_new_count;	\
	}

#define STAT_DELETE_COUNT \
	{	\
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_delete_mutex, ) \
		++m_delete_count;	\
	}

/***
 * @class PacketStream
 *
 * @brief �������װ�࣬�������ڰ�ͷ�Ͱ���Ĺ���
 */
class PacketStream
{
public:		
	PacketStream (void)
	: m_buffer (NULL)
	, m_total_size (0)
	, m_total_buffer(NULL)
	, m_svc_handler(NULL)
	{
		STAT_NEW_COUNT
	}

	/// ����һ��������Э���
	PacketStream (uint32 opcode, uint64 guid, const std::string &msg)
	: m_header(opcode, msg.length(), guid)
	, m_svc_handler(NULL)
	{
		STAT_NEW_COUNT
		if ((m_new_count % 100000) == 0)
		{
			TRACELogp(LM_INFO, ACE_TEXT("packet new count number is <%s>\n"), boost::lexical_cast<string>(m_new_count).c_str());
		}
		m_total_size = head_size() + msg.length();
		m_total_buffer = new char[m_total_size];
		memcpy (m_total_buffer, &m_header, m_head_size);
		memcpy (m_total_buffer + head_size (), msg.c_str (), msg.length ());
		m_buffer = m_total_buffer + m_head_size;
	}

	~PacketStream (void)
	{
		STAT_DELETE_COUNT
		if ((m_delete_count % 100000) == 0)
		{
			TRACELogp(LM_INFO, ACE_TEXT("packet delete count number is <%s>\n"), boost::lexical_cast<string>(m_delete_count).c_str());
		}
		delete []m_total_buffer;
		m_total_buffer = NULL;
		m_buffer = NULL;
	}

	/// ��ȡ������
	uint32 opcode (void) const
	{
		return m_header.opcode;
	}

	/// ��ȡ��ͷ��С
	uint32 head_size (void) const
	{
		return m_head_size;
	}

	/// ��ȡʵ��guid
	uint64 guid (void) const
	{
		return m_header.guid;
	}

	/// ��ȡ�����С
	uint32 body_size (void) const
	{
		return m_header.size;
	}

	/// ��ȡ��ͷ�Ӱ���һ���ô�С
	uint32 stream_size (void) const
	{
		return m_total_size;
	}

	/// �������н���һ��ͷ���ṹʱ���ô˷���
	char * re_head (void)
	{
		return reinterpret_cast<char*>(&m_header);
	}

	/// ���ô˷������׽ӿڽ���һ��������
	char * re_body (void)
	{
		if ((m_total_buffer == NULL) 
		 && (m_header.size > 0))
		{
			m_total_size = m_head_size + m_header.size;
			m_total_buffer = new char[m_total_size];
			m_buffer = m_total_buffer + m_head_size;
		}
		return m_buffer;
	}

	/// ��ȡ���յ�����Ϣ��
	const char * pe_body (void) const
	{
		return m_buffer;
	}

	/// ��������
	bool parse_protocol (google::protobuf::Message &msg) const
	{
		google::protobuf::io::ArrayInputStream istream (m_buffer, m_header.size);

		if (! m_header.compress)
		{
			if (msg.ParseFromZeroCopyStream (&istream))
				return true;
			else
				return false;
		}
		else
		{
			uint8 feature = m_header.compress & 0x0F;
			if ((feature == 1) || (feature == 2))
			{
				/// ������ǰʹ�õ�ѹ����ʽ
				google::protobuf::io::GzipInputStream::Format format = 
					google::protobuf::io::GzipInputStream::Format (feature);

				google::protobuf::io::GzipInputStream gstream (&istream, format);
				if (msg.ParseFromZeroCopyStream (&gstream))
					return true;
				else
					return false;
			}
			else
			{
				return false;
			}
		}
	}

	/// ����ʱӦ�õ��ô˷���������װ�õ�Э��һ�鷢��
	const char * stream (void) const
	{
		if (NULL != m_total_buffer)
		{
			memcpy (m_total_buffer, &m_header, m_head_size);
			return m_total_buffer;
		}
		else
		{
			return reinterpret_cast<const char*>(&m_header);
		}
	}

	/// �������Ϣ
	void reset()
	{
		delete [] m_total_buffer;
		m_total_buffer = NULL;
		m_buffer = NULL;
	}

	void setEventHandler(ACE_SVC_HANDLER * svc_handler)
	{
		m_svc_handler = svc_handler;
	}

	ACE_SVC_HANDLER * getEventHandler()
	{
		return m_svc_handler;
	}

	/// ��¡һ��
	PacketStream * clone()
	{
		PacketStream * ps = new PacketStream();
		ps->m_header = this->m_header;
		ps->m_total_size = this->m_total_size;
		ps->m_svc_handler = this->m_svc_handler;
		ps->m_total_buffer = new char[ps->m_total_size];

		memcpy(ps->m_total_buffer, this->m_total_buffer, ps->m_total_size);
		ps->m_buffer = ps->m_total_buffer + m_head_size;
		return ps;
	}

protected:
	Header m_header;
	char * m_buffer;
	char * m_total_buffer;
	int m_total_size;
	static int m_head_size;
	ACE_SVC_HANDLER * m_svc_handler;

	static uint64 m_new_count;
	static uint64 m_delete_count;
	static ACE_Thread_Mutex m_new_mutex;
	static ACE_Thread_Mutex m_delete_mutex;
};

#endif