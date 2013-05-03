.PHONY:		clean

PAYLOAD		= limera1n
TARGETS		= $(PAYLOAD).elf
OBJECTS		= $(PAYLOAD).o
ASFLAGS		= -D__ASSEMBLER__ -march=armv7-a
CPPFLAGS	= -I../../../../Include
LDFLAGS		= -nostdlib 
CROSS		= arm-none-eabi-
AS		= $(CROSS)gcc
CC		= $(CROSS)gcc
OBJCOPY		= $(CROSS)objcopy

all:	$(TARGETS)

$(TARGETS):	$(OBJECTS)
	@echo Linking $(TARGETS)...
	@$(CC) $(OBJECTS) -o $(TARGETS) $(LDFLAGS)
	@echo Copying to raw binary...
	@$(OBJCOPY) -O binary $(TARGETS) $(TARGETS).raw	
	@echo Creating header $(PAYLOAD).h...
	@xxd -i $(TARGETS).raw > ../../Static/$(PAYLOAD).h
	@echo Cleaning intermediate files...
	@rm -f $(TARGETS) $(OBJECTS) $(TARGETS).raw

%.o: %.S
	@echo Compiling $<...
	@$(AS) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<

clean:
	@echo Cleaning payload...
	@rm -f $(TARGETS) $(OBJECTS) $(TARGETS).raw
