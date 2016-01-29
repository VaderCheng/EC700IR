M ?= dm8127
#������ĺ꼴�ɽ���ARM�����µĽ������
ifeq ($(M),dm6467)
	CROSS_COMPILE := arm_v5t_le-
else
	CROSS_COMPILE := arm-arago-linux-gnueabi-
endif

CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
AR := $(CROSS_COMPILE)ar
STRIP := $(CROSS_COMPILE)strip

#�رձ�������
ifneq ($(V),1)
	Q=@
	W=-w
endif

#C/C++����ĺ궨��
CFLAG := -DLINUX -DVERSIONHEADER=\"3.1.208.\" -DHAS_DOG $(USER_VERSION)
NEON_CFLAG := -mfpu=neon -mfloat-abi=softfp

ifdef CROSS_COMPILE
	CFLAG += -O2 -march=armv7-a -mcpu=cortex-a8 $(NEON_CFLAG)
else
	CFLAG + -g
endif

ifdef DIR
	WORK_BASE := $(DIR)
endif

#ͷ�ļ�·��
INC := -I.\
		-I$(WORK_BASE)\
		-I$(WORK_BASE)/Plugin/tinyxml\
		-I$(WORK_BASE)/Plugin/FreeType\
		-I$(WORK_BASE)/swpa/inc\
		-I$(WORK_BASE)/SWFC/inc\
		-I$(WORK_BASE)/BasicModule/inc\
		-I$(WORK_BASE)/Plugin/libjpeg/inc \
		-I$(WORK_BASE)/swpa/src/dm8127/driver \
		$(shell find . -type d|grep -v svn| grep -E "public|$M" |awk -F. '{print $$2}'|awk '{print "-I$(CURDIR)"$$1}')

#��ͬ�������µĺ궨��
ifdef CROSS_COMPILE
	CFLAG += -DCROSS_COMPILE
	LIBPATH := $(WORK_BASE)/lib/ARM
	BINPATH := $(WORK_BASE)/bin/ARM
else
	LIBPATH := $(WORK_BASE)/lib/Linux
	BINPATH := $(WORK_BASE)/bin/Linux
endif


#���ӿ�·��
LDFLAG := -L$(LIBPATH)\
       -lFilter_$(M)\
       -lSWFC\
       -lswpa_$(M)\
       -lPlugin\
       -lpthread\
       -lrt\
       -ldl\
       -ljpeg\
	   -lcrypto\
	   -lssl


#���ݺ궨��ѡ��Դ�ļ�
ifndef M
	SRC    :=  $(shell find . -iname '*.c' -o -iname '*.cpp')
	CFLAG += -DDM6467 -DDM8127
else
ifeq ($(M),dm6467)
	SRC    :=  $(shell find . -iname '*.c' -o -iname '*.cpp'|grep -v dm8127)
	CFLAG += -DDM6467
endif
		
ifeq ($(M),dm8127)
	SRC    :=  $(shell find . -iname '*.c' -o -iname '*.cpp'|grep -v dm6467)
	CFLAG += -DDM8127 -DWITH_OPENSSL -DWITH_DOM
endif
endif

CXXFLAG := $(CFLAG)

#Ŀ���ļ�
OBJ=$(patsubst %.c,%.o, $(patsubst %.cpp,%.o, $(SRC))) 
LIBS=$(wildcard $(LIBPATH)/*.a)
#�������
.c.o:
	@echo "compile $< -> $@"
	$(Q)$(CC) $(CFLAG) -o $@ -c $< $(INC) $(W)
	
.cpp.o:
	@echo "compile $< -> $@"
	$(Q)$(CXX) $(CXXFLAG) -o $@ -c $< $(INC) $(W)

#����ͷ�ļ������ļ�
DEPENDFILE=$(patsubst %.c,%.d, $(patsubst %.cpp,%.d, $(SRC)))

-include $(DEPENDFILE)

%.d: %.c
	@echo "generate depend file $< -> $@" 
	$(Q)$(CC) -MM $(INC) $(NEON_CFLAG) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$*\.o $@ : ,g' < $@.$$$$ > $@; \
	rm -r $@.$$$$

%.d: %.cpp
	@echo "generate depend file $< -> $@" 
	$(Q)$(CXX) -MM $(INC) $(NEON_CFLAG) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$*\.o $@ : ,g' < $@.$$$$ > $@; \
	rm -r $@.$$$$
