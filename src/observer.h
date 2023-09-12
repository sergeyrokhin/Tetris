#pragma once

#include <memory>

class Observer
{
	struct IObserver {
		virtual ~IObserver() = default;
		virtual std::unique_ptr<IObserver> Copy() const = 0;
		//изображение изменилось, надо перерисовать
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Draw() = 0;
	};

	template <typename T> struct ObserverObject final : IObserver {
		T data_;
		std::shared_ptr<BattleField> field_ptr_;
		std::atomic<bool> stoped_ = { false };
		std::atomic<bool> redraw_ = { false };

		ObserverObject(T& x, std::shared_ptr<BattleField> &field_ptr) : data_(std::move(x)), field_ptr_(field_ptr){
			//конструктор забирает объект, 
			std::thread t(&ObserverObject::ExecProcessThread, this);
			t.detach();
		}

		ObserverObject(const ObserverObject &x) {
			std::cout << "ObserverObject" << std::endl;
			//std::swap(this->data_, x.data_);
			//std::swap(this->field_ptr_, x.field_ptr_);
		}

		void ExecProcessThread() {
			while ( ! stoped_.load(std::memory_order_relaxed))
			{
				if (redraw_.load(std::memory_order_relaxed))
				{
					redraw_.store(false, std::memory_order_relaxed);
					data_.Print(field_ptr_);
				}
			}
		}

		std::unique_ptr<IObserver> Copy() const override {
			return std::make_unique<ObserverObject>(*this);
		}

		void Start() override {
			if ( !stoped_.load(std::memory_order_relaxed))
			{
				stoped_.store(false, std::memory_order_relaxed);
			}
		}
		void Stop() override {
			stoped_.store(true, std::memory_order_relaxed);
		}

		void Draw() override {
			redraw_.store(true, std::memory_order_relaxed);
		}
	};

	std::unique_ptr<IObserver> self_;

public:
	template <typename T>
	//Observer(T x, std::shared_ptr<BattleField>& field_ptr) : self_(std::make_unique<ObserverObject<T>>(std::move(x), field_ptr)) {}
	Observer(T &x, std::shared_ptr<BattleField>& field_ptr) : self_(std::make_unique<ObserverObject<T>>(std::move(x), field_ptr)) {}

	// copy ctor, move ctor and assignment
public:
	Observer(const Observer& x) : self_(x.self_->Copy()) {}
	Observer(Observer&& x) noexcept = default;
	Observer& operator=(Observer x) noexcept {
		self_ = std::move(x.self_);
		return *this;
	}
	void Stop() { self_.get()->Stop(); }

public:
	//friend void draw(const Observer& x) {
	//	x.self_->Draw();
	//}
};

using ObserverSet = std::vector<Observer>;