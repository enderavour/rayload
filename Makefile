CC=cl
ASM=nasm

all: dynload.obj getpeb.obj
	link /SUBSYSTEM:CONSOLE /NODEFAULTLIB /ALIGN:16 /MERGE:.data=.text /MERGE:.pdata=.text /MERGE:.rdata=.text dynload.obj getpeb.obj /OUT:dynload.exe
	del *.obj

dynload.obj: dynload.c
	$(CC) /Os /GS- /GR- /EHs- /c dynload.c

getpeb.obj: getpeb.asm
	$(ASM) -f win64 -o getpeb.obj getpeb.asm