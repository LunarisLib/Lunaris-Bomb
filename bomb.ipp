#pragma once
#include "bomb.h"

namespace Lunaris {

	inline bomb::bomb(const std::function<void(void)> f)
		: boom(f)
	{
		if (!boom) throw std::runtime_error("Expected a valid function @ bomb!");
	}

	inline bomb::~bomb()
	{
		std::lock_guard<std::mutex> luck(saf);
		if (boom) {
			boom();
		}
	}

	inline bomb::bomb(bomb&& boo) noexcept
	{
		std::lock_guard<std::mutex> luck(boo.saf);
		boom = std::move(boo.boom);
	}

	inline void bomb::operator=(bomb&& boo) noexcept
	{
		std::unique_lock<std::mutex> l1(saf, std::defer_lock);
		std::unique_lock<std::mutex> l2(boo.saf, std::defer_lock);
		std::lock(l1, l2);

		if (boom) {
			boom();
		}
		boom = std::move(boo.boom);
	}

	inline std::function<void(void)> bomb::defuse()
	{
		std::lock_guard<std::mutex> luck(saf);
		return std::move(boom);
	}

	inline bool bomb::is_defused() const
	{
		std::lock_guard<std::mutex> luck(saf);
		return !boom.operator bool();
	}

	inline bool bomb::valid() const
	{
		return !is_defused();
	}

	inline bool bomb::empty() const
	{
		return is_defused();
	}

	inline timed_bomb::_bomb::_bomb(std::function<void()> f)
		: _bmb(f)
	{
	}

	inline timed_bomb::timed_bomb(const std::function<void(void)> f, const double dt)
		: movable_bomb(std::make_shared<_bomb>(f))
	{
		if (dt <= 0.0) { // already timedout lol
			auto f = movable_bomb->_bmb.defuse();
			if (f) f();
		}
		else {
			movable_bomb->_thr = std::thread([bmb = movable_bomb, dt]() mutable { std::this_thread::sleep_for(std::chrono::duration<double>(dt)); auto f = bmb->_bmb.defuse(); if (f) f(); });
			movable_bomb->_thr.detach(); // detached always. No worries, the thread should die sometime soon.
		}
	}

	inline timed_bomb::~timed_bomb()
	{
		auto f = defuse(); // this will trigger the bomb right there, so not the thread. This defuse is safe.
		if (f) {
			f(); // boom if exists
		}
	}

	inline timed_bomb::timed_bomb(timed_bomb&& oth) noexcept
		: movable_bomb(std::move(oth.movable_bomb))
	{
	}

	inline void timed_bomb::operator=(timed_bomb&& oth) noexcept
	{
		auto f = defuse(); // this will trigger the bomb right there, so not the thread. This defuse is safe.
		if (f) {
			f(); // boom if exists
		}
		movable_bomb = std::move(oth.movable_bomb);
	}

	inline std::function<void(void)> timed_bomb::defuse()
	{
		return movable_bomb ? movable_bomb->_bmb.defuse() : std::function<void(void)>{};
	}

	inline bool timed_bomb::is_defused() const
	{
		return movable_bomb ? movable_bomb->_bmb.is_defused() : true;
	}

	inline bool timed_bomb::valid() const
	{
		return !is_defused();
	}

	inline bool timed_bomb::empty() const
	{
		return is_defused();
	}

}