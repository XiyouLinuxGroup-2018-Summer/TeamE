objs = server.o client.o chat.o sockio.o wrap.o mysql.o error.o
serv_objs = server.o chat.o sockio.o wrap.o mysql.o error.o
cli_objs = client.o sockio.o wrap.o error.o

all : sof cof
.PHONY : all

# 编译服务端
sof : $(serv_objs)
	cc -o sof $(serv_objs) `mysql_config --libs`

# 编译客户端
cof : $(cli_objs)
	cc -o cof $(cli_objs)

all : $(objs)
.PHONY : all

# 编译生成所有目标文件
$(objs) : %.o : %.c
	cc -c $< -o $@

# 删除所有目标文件和可执行文件，以便重新编译
.PHONY : clean
clean:
	-rm sof cof $(objs)
