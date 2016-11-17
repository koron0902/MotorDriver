/*
 * TaskManager.h
 *
 *  Created on: 2016/10/08
 *      Author: TERU
 */

#ifndef DEVICE_INC_TASKMANAGER_HPP_
#define DEVICE_INC_TASKMANAGER_HPP_
#include <FreeRTOS.h>
#include <string>
#include <functional>
#include <memory>

#include <apply.hpp>

namespace Device {

//FreeRTOSのラッパ(呼び出しに遅延あり)
class TaskBase {
public:
	//静的領域
	static constexpr uint32_t StackSizeDefault = 128;
	static constexpr uint32_t PriorityDefault = configMAX_PRIORITIES - 1;

	static void BeginAll();
	static void EndAll();
	static void SuspendAll();
	static void ResumeAll(); //TDOO 関数が見つからない
private:
protected:
	//動的領域
	std::string m_name;xTaskHandle m_handle{nullptr};
	uint32_t m_stack{0};
	uint32_t m_priority{PriorityDefault};

public:
	TaskBase(); //直接の作成を禁止する。
	TaskBase(const TaskBase&) = default;
	virtual ~TaskBase() {};

	inline const std::string& GetName() const {
		return m_name;
	}
	inline xTaskHandle GetHandle() const {
		return m_handle;
	}
	//補助関数


	inline void SetPriority(portBASE_TYPE pri){
		vTaskPrioritySet(m_handle,pri);
	}

	inline portBASE_TYPE GetPriority(){
		return uxTaskPriorityGet(m_handle);
	}




private:
};

template<class ...Args> class Task: public TaskBase {
protected:
	typedef std::function<void(Args...)> func_t;
	std::tuple<Args...> m_args;
	func_t m_func;

	static void ISR(void *ptr){
		Task<Args...> *that=(Task*)ptr;
		common::apply(that->m_args,that->m_func);
		vTaskDelete(that->m_handle);
	}
public:
	Task(const std::string& name,const func_t& func=nullptr,
			uint32_t stack = StackSizeDefault, uint32_t priority =
					PriorityDefault) :m_func(func) {
		m_name=name;
		m_stack = stack;
		m_priority = priority;
	}
	Task& Hock(const func_t& func){
		m_func=func;
		return *this;
	}

	Task(const Task&) = default;
	virtual ~Task() = default;
	virtual void operator ()(Args ...args){
		m_args=std::tuple<Args...>(args...);
		xTaskCreate(ISR,m_name.c_str(),m_stack,this,m_priority,&m_handle);//
	}
};

template<> class Task<void(void)>: public TaskBase {
protected:
	typedef std::function<void(void)> func_t;
	func_t m_func;

	static void ISR(void *ptr){
		Task *that=(Task*)ptr;
		that->m_func();
		vTaskDelete(that->m_handle);
	}
public:
	Task(const std::string& name,const func_t& func=nullptr,
			uint32_t stack = StackSizeDefault, uint32_t priority =
					PriorityDefault) :m_func(func) {
		m_name=name;
		m_stack = stack;
		m_priority = priority;
	}
	Task& Hock(const func_t& func){
		m_func=func;
		return *this;
	}

	Task(const Task&) = default;
	virtual ~Task() = default;
	virtual void operator ()(void){
		xTaskCreate(ISR,m_name.c_str(),m_stack,this,m_priority,&m_handle);//
	}
};

} /* namespace Device */

#endif /* DEVICE_INC_TASKMANAGER_HPP_ */
