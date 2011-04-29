################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Socket/Socket.cpp \
../src/Socket/clientSocket.cpp \
../src/Socket/serverSocket.cpp 

OBJS += \
./src/Socket/Socket.o \
./src/Socket/clientSocket.o \
./src/Socket/serverSocket.o 

CPP_DEPS += \
./src/Socket/Socket.d \
./src/Socket/clientSocket.d \
./src/Socket/serverSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/Socket/%.o: ../src/Socket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


