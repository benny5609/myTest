include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = ../BaseCommand.o ../UDPTest.o 

LIB_BASE_OBJS = BaseCommand.o UDPTest.o 
LIB_BASE = libTcpTest.so

			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE): $(PATS)
	$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
cl:	
	rm -rf *.o 
