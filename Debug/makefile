################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include test/testmodule/subdir.mk
-include src/subdir.mk
-include src/Threading/subdir.mk
-include src/System/subdir.mk
-include src/Socket/subdir.mk
-include src/Path/subdir.mk
-include src/Module/subdir.mk
-include src/Http/subdir.mk
-include src/File/subdir.mk
-include src/Events/subdir.mk
-include src/Env/subdir.mk
-include src/Directory/subdir.mk
-include modules/Mysql/subdir.mk
-include modules/Memcache/subdir.mk
-include .sconf_temp/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libCoreJS.dylib

# Tool invocations
libCoreJS.dylib: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C++ Linker'
	g++ -dynamiclib -o "libCoreJS.dylib" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libCoreJS.dylib
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
