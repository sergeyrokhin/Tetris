#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <atomic>
#include <thread>

class BattleField;

class Observer
{
	struct IObserver {
		virtual ~IObserver() = default;
		virtual std::unique_ptr<IObserver> Copy() const = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Draw() = 0;
		virtual bool IsRun() = 0;
	};

	template <typename T> struct ObserverObject final : IObserver {
		T data_;
		std::shared_ptr<BattleField> field_ptr_;
		std::atomic<bool> stoped_ = { false };
		std::atomic<bool> redraw_ = { true };
		std::atomic<bool> thread_run_ = { false };

		ObserverObject() = delete;
		
		ObserverObject(T&& x, std::shared_ptr<BattleField> &field_ptr) : data_(std::move(x)), field_ptr_(field_ptr){
		}

		ObserverObject(ObserverObject&& x) : data_(std::move(x.data_)), field_ptr_(x.field_ptr_)
		{
		}

		ObserverObject(const ObserverObject& x) : data_(x.data_), field_ptr_(x.field_ptr_)
		{
		}

		~ObserverObject() = default;

		void ExecProcessThread() {
			thread_run_.store(true, std::memory_order_relaxed);
			while ( ! stoped_.load(std::memory_order_relaxed))
			{
				if (redraw_.load(std::memory_order_relaxed))
				{
					redraw_.store(false, std::memory_order_relaxed);
					data_.Print(*field_ptr_.get());
				}
				wait();
			}
			data_.Print(*field_ptr_.get());
			thread_run_.store(false, std::memory_order_relaxed);
		}

		std::unique_ptr<IObserver> Copy() const override {
			return std::make_unique<ObserverObject>(*this);
		}

		void Start() override {
			if ( !stoped_.load(std::memory_order_relaxed))
			{
				stoped_.store(false, std::memory_order_relaxed);
				//конструктор забирает объект, 
				std::thread t(&ObserverObject::ExecProcessThread, this);
				t.detach();
			}
		}
		void Stop() override {
			stoped_.store(true, std::memory_order_relaxed);
		}

		void Draw() override {
			redraw_.store(true, std::memory_order_relaxed);
		}

		bool IsRun() override {
			return thread_run_.load(std::memory_order_relaxed);
		}
	};

	std::unique_ptr<IObserver> self_;

public:
	template <typename T>
	Observer(T &&x, std::shared_ptr<BattleField> &field_ptr) : self_(std::make_unique<ObserverObject<T>>(std::move(x), field_ptr))
	{
	}
	// copy ctor, move ctor and assignment
	Observer(const Observer&& x) : self_(x.self_->Copy()) {}
	Observer& operator=(Observer&& x) noexcept {
		self_ = std::move(x.self_);
		return *this;
	}
	void Stop() { Draw(); self_.get()->Stop(); }
	void Draw() { self_.get()->Draw(); }
	void Start() { self_.get()->Start(); }
	bool IsRun() { return self_.get()->IsRun(); }
};

using ObserverSet = std::vector<Observer>;