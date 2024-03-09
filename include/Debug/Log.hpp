#pragma once
#include <iostream>

enum LogType {
	Info,
	Debug,
	Warning,
	Error
};

class Logger {
public:
	template<LogType type, typename T, typename ... Ts>
	static void Log(const T& t, const Ts& ... ts) {
		switch (type) {
		case Info:
			std::cerr << "Info: ";
			break;
		case Debug: 
			std::cerr << "Debug: ";
			break;
		case Warning:
			std::cerr << "Warning: ";
			break;
		case Error:
			std::cerr << "Error: ";
			break;
		}
		Print(t, ts...);
	}

private:
	Logger() = delete;

	static void Print() {
		std::cerr << "\n";
	}

	template<typename T, typename ... Ts>
	static void Print(const T& t, const Ts& ... ts) {
		std::cerr << t << " ";
		Print(ts...);
	}
};