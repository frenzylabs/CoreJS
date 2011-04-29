################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Http/client.cpp \
../src/Http/url.cpp 

OBJS += \
./src/Http/client.o \
./src/Http/url.o 

CPP_DEPS += \
./src/Http/client.d \
./src/Http/url.d 


# Each subdirectory must supply rules for building sources it contributes
src/Http/%.o: ../src/Http/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


