all: FS_buffer.a
# LINUX-X86
 PF_CC=gcc
 PF_CCFLAG=-DOS_LINUX -DM_GENERIC_INT32 -march=i586 -fPIC -O -g  -Wall
 PF_EXEFLAG=
 PF_EXELIB=
######################################################################
# LINUX-X86_64
#PF_CC=gcc
#PF_CCFLAG=-DOS_LINUX -DM_GENERIC_INT32 -m64 -fPIC -O -Wall -g
#PF_EXEFLAG=-m64
#PF_EXELIB=

PROJ_CC=$(PF_CC)
PROJ_CCFLAG=$(PF_CCFLAG) -I.
clean:
	rm -f *.a *.o
.c.o:
	$(PROJ_CC) $(PROJ_CCFLAG) -c $< -o $@
FS_buffer.a: FS_buffer.o
	ar -r FS_buffer.a FS_buffer.o
