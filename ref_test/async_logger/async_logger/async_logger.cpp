///
/// a sample about aysnc_operator.
///
/// Kevin Lynx
/// 6.26.2008
///
#include "kl_async_operator.h"
#include <string>
#include <iostream>
#include <cstdlib>

///
/// log struct.
///
struct LogNode
{
	std::string _str;

	LogNode( const std::string &str ) : _str( str )
	{
	}
};

typedef kl_common::async_operator<LogNode> AsyncLogType;

void log_operate( AsyncLogType::list_type &_list )
{
	AsyncLogType::list_type::container_type my_container;
	_list.pop_all( my_container, false );
	std::size_t size = my_container.size();

	if( size == 0 )
	{
		return ;
	}

	for( std::size_t i = 0; i < size; ++ i )
	{
		LogNode log = my_container.front();
		my_container.pop_front();

		std::cout << log._str << std::endl;
	}
}

int main()
{
	AsyncLogType async_logger;
	async_logger.execute( AsyncLogType::operator_type( log_operate ) );

	for( int i = 0; i < 10; ++ i )
	{
		async_logger.list().push_back( LogNode( "INFO : Log string..." ) );
	}

	system( "pause" );
	async_logger.exit();
	return 0;
}