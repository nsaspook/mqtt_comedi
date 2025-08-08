#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/device_config.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/uart1.c mcc_generated_files/uart2.c mcc_generated_files/tmr6.c mcc_generated_files/spi1.c mcc_generated_files/dma1.c mcc_generated_files/memory.c mcc_generated_files/tmr5.c mcc_generated_files/tmr2.c mcc_generated_files/adc.c mcc_generated_files/spi2.c mcc_generated_files/tmr0.c mcc_generated_files/dac1.c mcc_generated_files/dma2.c mcc_generated_files/dma3.c mcc_generated_files/uart3.c main.c eadog.c timers.c mconfig.c rs232.c slaveo.c tic12400.c mc33996.c bmcdio.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/device_config.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/uart1.p1 ${OBJECTDIR}/mcc_generated_files/uart2.p1 ${OBJECTDIR}/mcc_generated_files/tmr6.p1 ${OBJECTDIR}/mcc_generated_files/spi1.p1 ${OBJECTDIR}/mcc_generated_files/dma1.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1 ${OBJECTDIR}/mcc_generated_files/tmr5.p1 ${OBJECTDIR}/mcc_generated_files/tmr2.p1 ${OBJECTDIR}/mcc_generated_files/adc.p1 ${OBJECTDIR}/mcc_generated_files/spi2.p1 ${OBJECTDIR}/mcc_generated_files/tmr0.p1 ${OBJECTDIR}/mcc_generated_files/dac1.p1 ${OBJECTDIR}/mcc_generated_files/dma2.p1 ${OBJECTDIR}/mcc_generated_files/dma3.p1 ${OBJECTDIR}/mcc_generated_files/uart3.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/eadog.p1 ${OBJECTDIR}/timers.p1 ${OBJECTDIR}/mconfig.p1 ${OBJECTDIR}/rs232.p1 ${OBJECTDIR}/slaveo.p1 ${OBJECTDIR}/tic12400.p1 ${OBJECTDIR}/mc33996.p1 ${OBJECTDIR}/bmcdio.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/device_config.p1.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d ${OBJECTDIR}/mcc_generated_files/uart1.p1.d ${OBJECTDIR}/mcc_generated_files/uart2.p1.d ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d ${OBJECTDIR}/mcc_generated_files/spi1.p1.d ${OBJECTDIR}/mcc_generated_files/dma1.p1.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d ${OBJECTDIR}/mcc_generated_files/dac1.p1.d ${OBJECTDIR}/mcc_generated_files/dma2.p1.d ${OBJECTDIR}/mcc_generated_files/dma3.p1.d ${OBJECTDIR}/mcc_generated_files/uart3.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/eadog.p1.d ${OBJECTDIR}/timers.p1.d ${OBJECTDIR}/mconfig.p1.d ${OBJECTDIR}/rs232.p1.d ${OBJECTDIR}/slaveo.p1.d ${OBJECTDIR}/tic12400.p1.d ${OBJECTDIR}/mc33996.p1.d ${OBJECTDIR}/bmcdio.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/device_config.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/uart1.p1 ${OBJECTDIR}/mcc_generated_files/uart2.p1 ${OBJECTDIR}/mcc_generated_files/tmr6.p1 ${OBJECTDIR}/mcc_generated_files/spi1.p1 ${OBJECTDIR}/mcc_generated_files/dma1.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1 ${OBJECTDIR}/mcc_generated_files/tmr5.p1 ${OBJECTDIR}/mcc_generated_files/tmr2.p1 ${OBJECTDIR}/mcc_generated_files/adc.p1 ${OBJECTDIR}/mcc_generated_files/spi2.p1 ${OBJECTDIR}/mcc_generated_files/tmr0.p1 ${OBJECTDIR}/mcc_generated_files/dac1.p1 ${OBJECTDIR}/mcc_generated_files/dma2.p1 ${OBJECTDIR}/mcc_generated_files/dma3.p1 ${OBJECTDIR}/mcc_generated_files/uart3.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/eadog.p1 ${OBJECTDIR}/timers.p1 ${OBJECTDIR}/mconfig.p1 ${OBJECTDIR}/rs232.p1 ${OBJECTDIR}/slaveo.p1 ${OBJECTDIR}/tic12400.p1 ${OBJECTDIR}/mc33996.p1 ${OBJECTDIR}/bmcdio.p1

# Source Files
SOURCEFILES=mcc_generated_files/device_config.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/uart1.c mcc_generated_files/uart2.c mcc_generated_files/tmr6.c mcc_generated_files/spi1.c mcc_generated_files/dma1.c mcc_generated_files/memory.c mcc_generated_files/tmr5.c mcc_generated_files/tmr2.c mcc_generated_files/adc.c mcc_generated_files/spi2.c mcc_generated_files/tmr0.c mcc_generated_files/dac1.c mcc_generated_files/dma2.c mcc_generated_files/dma3.c mcc_generated_files/uart3.c main.c eadog.c timers.c mconfig.c rs232.c slaveo.c tic12400.c mc33996.c bmcdio.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F57Q84
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/device_config.p1: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/device_config.p1 mcc_generated_files/device_config.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/device_config.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/uart1.p1: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/uart1.p1 mcc_generated_files/uart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/uart1.d ${OBJECTDIR}/mcc_generated_files/uart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/uart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/uart2.p1: mcc_generated_files/uart2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/uart2.p1 mcc_generated_files/uart2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/uart2.d ${OBJECTDIR}/mcc_generated_files/uart2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/uart2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr6.p1: mcc_generated_files/tmr6.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr6.p1 mcc_generated_files/tmr6.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr6.d ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi1.p1: mcc_generated_files/spi1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/spi1.p1 mcc_generated_files/spi1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi1.d ${OBJECTDIR}/mcc_generated_files/spi1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dma1.p1: mcc_generated_files/dma1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dma1.p1 mcc_generated_files/dma1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dma1.d ${OBJECTDIR}/mcc_generated_files/dma1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dma1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr5.p1: mcc_generated_files/tmr5.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr5.p1 mcc_generated_files/tmr5.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr5.d ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr2.p1: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr2.p1 mcc_generated_files/tmr2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr2.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adc.p1: mcc_generated_files/adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/adc.p1 mcc_generated_files/adc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adc.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi2.p1: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/spi2.p1 mcc_generated_files/spi2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi2.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr0.p1: mcc_generated_files/tmr0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr0.p1 mcc_generated_files/tmr0.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr0.d ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dac1.p1: mcc_generated_files/dac1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dac1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dac1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dac1.p1 mcc_generated_files/dac1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dac1.d ${OBJECTDIR}/mcc_generated_files/dac1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dac1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dma2.p1: mcc_generated_files/dma2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dma2.p1 mcc_generated_files/dma2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dma2.d ${OBJECTDIR}/mcc_generated_files/dma2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dma2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dma3.p1: mcc_generated_files/dma3.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma3.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dma3.p1 mcc_generated_files/dma3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dma3.d ${OBJECTDIR}/mcc_generated_files/dma3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dma3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/uart3.p1: mcc_generated_files/uart3.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/uart3.p1 mcc_generated_files/uart3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/uart3.d ${OBJECTDIR}/mcc_generated_files/uart3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/uart3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/eadog.p1: eadog.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eadog.p1.d 
	@${RM} ${OBJECTDIR}/eadog.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/eadog.p1 eadog.c 
	@-${MV} ${OBJECTDIR}/eadog.d ${OBJECTDIR}/eadog.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/eadog.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timers.p1: timers.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.p1.d 
	@${RM} ${OBJECTDIR}/timers.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/timers.p1 timers.c 
	@-${MV} ${OBJECTDIR}/timers.d ${OBJECTDIR}/timers.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timers.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mconfig.p1: mconfig.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mconfig.p1.d 
	@${RM} ${OBJECTDIR}/mconfig.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mconfig.p1 mconfig.c 
	@-${MV} ${OBJECTDIR}/mconfig.d ${OBJECTDIR}/mconfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mconfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rs232.p1: rs232.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rs232.p1.d 
	@${RM} ${OBJECTDIR}/rs232.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/rs232.p1 rs232.c 
	@-${MV} ${OBJECTDIR}/rs232.d ${OBJECTDIR}/rs232.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rs232.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/slaveo.p1: slaveo.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/slaveo.p1.d 
	@${RM} ${OBJECTDIR}/slaveo.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/slaveo.p1 slaveo.c 
	@-${MV} ${OBJECTDIR}/slaveo.d ${OBJECTDIR}/slaveo.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/slaveo.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/tic12400.p1: tic12400.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tic12400.p1.d 
	@${RM} ${OBJECTDIR}/tic12400.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/tic12400.p1 tic12400.c 
	@-${MV} ${OBJECTDIR}/tic12400.d ${OBJECTDIR}/tic12400.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/tic12400.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mc33996.p1: mc33996.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mc33996.p1.d 
	@${RM} ${OBJECTDIR}/mc33996.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mc33996.p1 mc33996.c 
	@-${MV} ${OBJECTDIR}/mc33996.d ${OBJECTDIR}/mc33996.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mc33996.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/bmcdio.p1: bmcdio.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bmcdio.p1.d 
	@${RM} ${OBJECTDIR}/bmcdio.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit5   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/bmcdio.p1 bmcdio.c 
	@-${MV} ${OBJECTDIR}/bmcdio.d ${OBJECTDIR}/bmcdio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/bmcdio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/mcc_generated_files/device_config.p1: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/device_config.p1 mcc_generated_files/device_config.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/device_config.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/uart1.p1: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/uart1.p1 mcc_generated_files/uart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/uart1.d ${OBJECTDIR}/mcc_generated_files/uart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/uart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/uart2.p1: mcc_generated_files/uart2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/uart2.p1 mcc_generated_files/uart2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/uart2.d ${OBJECTDIR}/mcc_generated_files/uart2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/uart2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr6.p1: mcc_generated_files/tmr6.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr6.p1 mcc_generated_files/tmr6.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr6.d ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi1.p1: mcc_generated_files/spi1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/spi1.p1 mcc_generated_files/spi1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi1.d ${OBJECTDIR}/mcc_generated_files/spi1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dma1.p1: mcc_generated_files/dma1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dma1.p1 mcc_generated_files/dma1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dma1.d ${OBJECTDIR}/mcc_generated_files/dma1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dma1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr5.p1: mcc_generated_files/tmr5.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr5.p1 mcc_generated_files/tmr5.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr5.d ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr2.p1: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr2.p1 mcc_generated_files/tmr2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr2.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adc.p1: mcc_generated_files/adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/adc.p1 mcc_generated_files/adc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adc.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi2.p1: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/spi2.p1 mcc_generated_files/spi2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi2.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr0.p1: mcc_generated_files/tmr0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr0.p1 mcc_generated_files/tmr0.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr0.d ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dac1.p1: mcc_generated_files/dac1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dac1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dac1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dac1.p1 mcc_generated_files/dac1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dac1.d ${OBJECTDIR}/mcc_generated_files/dac1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dac1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dma2.p1: mcc_generated_files/dma2.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dma2.p1 mcc_generated_files/dma2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dma2.d ${OBJECTDIR}/mcc_generated_files/dma2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dma2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/dma3.p1: mcc_generated_files/dma3.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/dma3.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/dma3.p1 mcc_generated_files/dma3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/dma3.d ${OBJECTDIR}/mcc_generated_files/dma3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/dma3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/uart3.p1: mcc_generated_files/uart3.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/uart3.p1 mcc_generated_files/uart3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/uart3.d ${OBJECTDIR}/mcc_generated_files/uart3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/uart3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/eadog.p1: eadog.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eadog.p1.d 
	@${RM} ${OBJECTDIR}/eadog.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/eadog.p1 eadog.c 
	@-${MV} ${OBJECTDIR}/eadog.d ${OBJECTDIR}/eadog.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/eadog.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timers.p1: timers.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.p1.d 
	@${RM} ${OBJECTDIR}/timers.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/timers.p1 timers.c 
	@-${MV} ${OBJECTDIR}/timers.d ${OBJECTDIR}/timers.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timers.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mconfig.p1: mconfig.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mconfig.p1.d 
	@${RM} ${OBJECTDIR}/mconfig.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mconfig.p1 mconfig.c 
	@-${MV} ${OBJECTDIR}/mconfig.d ${OBJECTDIR}/mconfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mconfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rs232.p1: rs232.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rs232.p1.d 
	@${RM} ${OBJECTDIR}/rs232.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/rs232.p1 rs232.c 
	@-${MV} ${OBJECTDIR}/rs232.d ${OBJECTDIR}/rs232.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rs232.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/slaveo.p1: slaveo.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/slaveo.p1.d 
	@${RM} ${OBJECTDIR}/slaveo.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/slaveo.p1 slaveo.c 
	@-${MV} ${OBJECTDIR}/slaveo.d ${OBJECTDIR}/slaveo.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/slaveo.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/tic12400.p1: tic12400.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/tic12400.p1.d 
	@${RM} ${OBJECTDIR}/tic12400.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/tic12400.p1 tic12400.c 
	@-${MV} ${OBJECTDIR}/tic12400.d ${OBJECTDIR}/tic12400.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/tic12400.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mc33996.p1: mc33996.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mc33996.p1.d 
	@${RM} ${OBJECTDIR}/mc33996.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mc33996.p1 mc33996.c 
	@-${MV} ${OBJECTDIR}/mc33996.d ${OBJECTDIR}/mc33996.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mc33996.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/bmcdio.p1: bmcdio.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bmcdio.p1.d 
	@${RM} ${OBJECTDIR}/bmcdio.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/bmcdio.p1 bmcdio.c 
	@-${MV} ${OBJECTDIR}/bmcdio.d ${OBJECTDIR}/bmcdio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/bmcdio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=pickit5  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O2 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -msummary=-psect,-class,-mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/bmc_slave.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
