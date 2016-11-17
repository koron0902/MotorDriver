/*
 * TaskManager.cpp
 *
 *  Created on: 2016/10/08
 *      Author: TERU
 */

#include "TaskManager.hpp"
namespace Device {

void TaskBase::BeginAll(){
	vTaskStartScheduler();
}

void TaskBase::EndAll(){
	vTaskEndScheduler();
}

void TaskBase::SuspendAll(){
	vTaskSuspendAll();
}

void TaskBase::ResumeAll(){
	//TODO
}



TaskBase::TaskBase(){

}



} /* namespace Device */
