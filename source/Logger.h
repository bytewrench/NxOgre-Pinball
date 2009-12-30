#ifndef _LOGGER
#define _LOGGER

class pinballLogger
{
public:
	pinballLogger(){
		logMgr = new LogManager;
		log = LogManager::getSingleton().createLog("gameLog.txt");
	}
	void logMessage(const String & message){
		log->logMessage(message);
	}
protected:
private:
	LogManager * logMgr;
	Log * log;
};
#endif