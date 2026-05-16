/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#n
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
#t* @brief This function handles Non maskable interrupt.
#t*/
void NMI_Handler(void)
{
#t/* USER CODE BEGIN NonMaskableInt_IRQn 0 */

#n#t/* USER CODE END NonMaskableInt_IRQn 0 */
#t/* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
#t/* USER CODE END NonMaskableInt_IRQn 1 */
}#n
/**
#t* @brief This function handles Hard fault interrupt.
#t*/
void HardFault_Handler(void)
{
#t/* USER CODE BEGIN HardFault_IRQn 0 */

#n#t/* USER CODE END HardFault_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_HardFault_IRQn 0 */
#t#t/* USER CODE END W1_HardFault_IRQn 0 */
#t}
}#n
/**
#t* @brief This function handles Memory management fault.
#t*/
void MemManage_Handler(void)
{
#t/* USER CODE BEGIN MemoryManagement_IRQn 0 */

#n#t/* USER CODE END MemoryManagement_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
#t#t/* USER CODE END W1_MemoryManagement_IRQn 0 */
#t}
}#n
/**
#t* @brief This function handles Pre-fetch fault, memory access fault.
#t*/
void BusFault_Handler(void)
{
#t/* USER CODE BEGIN BusFault_IRQn 0 */

#n#t/* USER CODE END BusFault_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_BusFault_IRQn 0 */
#t#t/* USER CODE END W1_BusFault_IRQn 0 */
#t}
}#n
/**
#t* @brief This function handles Undefined instruction or illegal state.
#t*/
void UsageFault_Handler(void)
{
#t/* USER CODE BEGIN UsageFault_IRQn 0 */

#n#t/* USER CODE END UsageFault_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
#t#t/* USER CODE END W1_UsageFault_IRQn 0 */
#t}
}#n
/**
#t* @brief This function handles System service call via SWI instruction.
#t*/
void SVC_Handler(void)
{
#t/* USER CODE BEGIN SVCall_IRQn 0 */

#n#t/* USER CODE END SVCall_IRQn 0 */
#t/* USER CODE BEGIN SVCall_IRQn 1 */

#t/* USER CODE END SVCall_IRQn 1 */
}#n
/**
#t* @brief This function handles Debug monitor.
#t*/
void DebugMon_Handler(void)
{
#t/* USER CODE BEGIN DebugMonitor_IRQn 0 */

#n#t/* USER CODE END DebugMonitor_IRQn 0 */
#t/* USER CODE BEGIN DebugMonitor_IRQn 1 */

#t/* USER CODE END DebugMonitor_IRQn 1 */
}#n
/**
#t* @brief This function handles Pendable request for system service.
#t*/
void PendSV_Handler(void)
{
#t/* USER CODE BEGIN PendSV_IRQn 0 */

#n#t/* USER CODE END PendSV_IRQn 0 */
#t/* USER CODE BEGIN PendSV_IRQn 1 */

#t/* USER CODE END PendSV_IRQn 1 */
}#n
/**
#t* @brief This function handles System tick timer.
#t*/
void SysTick_Handler(void)
{
#t/* USER CODE BEGIN SysTick_IRQn 0 */

#n#t/* USER CODE END SysTick_IRQn 0 */
#tHAL_IncTick();
#t/* USER CODE BEGIN SysTick_IRQn 1 */

#t/* USER CODE END SysTick_IRQn 1 */
}#n

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
#t* @brief This function handles USART1 global interrupt.
#t*/
void USART1_IRQHandler(void)
{
#t/* USER CODE BEGIN USART1_IRQn 0 */

#n#t/* USER CODE END USART1_IRQn 0 */
#tHAL_UART_IRQHandlerFreeMarker template error (DEBUG mode; use RETHROW in production!):
The following has evaluated to null or missing:
==> timeBaseSource [in template "stm32xx_it_c.ftl" at line 278, column 30]
----
Tip: If the failing expression is known to legally refer to something that's sometimes null or missing, either specify a default value like myOptionalVar!myDefault, or use [#if myOptionalVar??]when-present[#else]when-missing[/#if]. (These only cover the last step of the expression; to cover the whole expression, use parenthesis: (myOptionalVar.foo)!myDefault, (myOptionalVar.foo)??
----
----
FTL stack trace ("~" means nesting-related):
- Failed at: #if timeBaseSource == vector.ipName &... [in template "stm32xx_it_c.ftl" at line 278, column 25]
----
Java stack trace (for programmers):
----
freemarker.core.InvalidReferenceException: [... Exception message was already printed; see it above ...]
at freemarker.core.InvalidReferenceException.getInstance(InvalidReferenceException.java:134)
at freemarker.core.EvalUtil.compare(EvalUtil.java:198)
at freemarker.core.EvalUtil.compare(EvalUtil.java:115)
at freemarker.core.ComparisonExpression.evalToBoolean(ComparisonExpression.java:78)
at freemarker.core.AndExpression.evalToBoolean(AndExpression.java:36)
at freemarker.core.IfBlock.accept(IfBlock.java:49)
at freemarker.core.Environment.visit(Environment.java:334)
at freemarker.core.Environment.visit(Environment.java:340)
at freemarker.core.Environment.visit(Environment.java:376)
at freemarker.core.IteratorBlock$IterationContext.executedNestedContentForCollOrSeqListing(IteratorBlock.java:291)
at freemarker.core.IteratorBlock$IterationContext.executeNestedContent(IteratorBlock.java:271)
at freemarker.core.IteratorBlock$IterationContext.accept(IteratorBlock.java:244)
at freemarker.core.Environment.visitIteratorBlock(Environment.java:644)
at freemarker.core.IteratorBlock.acceptWithResult(IteratorBlock.java:108)
at freemarker.core.IteratorBlock.accept(IteratorBlock.java:94)
at freemarker.core.Environment.visit(Environment.java:370)
at freemarker.core.Environment.visitAndTransform(Environment.java:501)
at freemarker.core.CompressedBlock.accept(CompressedBlock.java:42)
at freemarker.core.Environment.visit(Environment.java:334)
at freemarker.core.Environment.visit(Environment.java:340)
at freemarker.core.Environment.process(Environment.java:313)
at freemarker.template.Template.process(Template.java:383)
at com.st.microxplorer.codegenerator.CodeEngine.freemarkerDo(CodeEngine.java:420)
at com.st.microxplorer.codegenerator.CodeEngine.genCode(CodeEngine.java:273)
at com.st.microxplorer.codegenerator.CodeGenerator.generateOutputCode(CodeGenerator.java:6459)
at com.st.microxplorer.codegenerator.CodeGenerator.generatePeriphConfigCode(CodeGenerator.java:3081)
at com.st.microxplorer.codegenerator.CodeGenerator.generateCodeFiles(CodeGenerator.java:2300)
at com.st.microxplorer.codegenerator.CodeGenerator.generateDefaultConfig(CodeGenerator.java:10991)
at com.st.microxplorer.codegenerator.CodeGenerator.generateCode(CodeGenerator.java:1593)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.generateCode(ProjectBuilder.java:3343)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createCode(ProjectBuilder.java:2235)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createProject(ProjectBuilder.java:819)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createProject(ProjectBuilder.java:608)
at com.st.microxplorer.plugins.projectmanager.engine.MainProjectManager.startGenerateCode(MainProjectManager.java:1497)
at com.st.microxplorer.plugins.projectmanager.engine.MainProjectManager$3.onCommandEntered(MainProjectManager.java:283)
at com.st.components.util.CommandLineManager$3.run(CommandLineManager.java:175)
at java.desktop/java.awt.event.InvocationEvent.dispatch(InvocationEvent.java:308)
at java.desktop/java.awt.EventQueue.dispatchEventImpl(EventQueue.java:773)
at java.desktop/java.awt.EventQueue$4.run(EventQueue.java:720)
at java.desktop/java.awt.EventQueue$4.run(EventQueue.java:714)
at java.base/java.security.AccessController.doPrivileged(AccessController.java:400)
at java.base/java.security.ProtectionDomain$JavaSecurityAccessImpl.doIntersectionPrivilege(ProtectionDomain.java:87)
at java.desktop/java.awt.EventQueue.dispatchEvent(EventQueue.java:742)
at java.desktop/java.awt.EventDispatchThread.pumpOneEventForFilters(EventDispatchThread.java:203)
at java.desktop/java.awt.EventDispatchThread.pumpEventsForFilter(EventDispatchThread.java:124)
at java.desktop/java.awt.EventDispatchThread.pumpEventsForHierarchy(EventDispatchThread.java:113)
at java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:109)
at java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:101)
at java.desktop/java.awt.EventDispatchThread.run(EventDispatchThread.java:90)
