#ifdef DEBUG
    #define DEBUG_ENABLED 1
#else
    #define DEBUG_ENABLED 0
#endif

//todo: implement log to file?
class Logger {

	public:
		void debug(const char* message);
		void error(const char* message);
		void info(const char* message);
	private:
		bool debugEnabled = DEBUG_ENABLED;
};

