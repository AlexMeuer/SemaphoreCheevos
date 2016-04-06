#ifndef CHEEVO_TRACKER_H
#define CHEEVO_TRACKER_H

#include <stdlib.h>
#include <iostream>
#include <string>

#include <mutex>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define VARCHAR_LENGTH 255

class CheevoTracker {
public:
	CheevoTracker();
	~CheevoTracker();

	bool isUnlocked(std::string const& name);

	int getGamerScore(std::string const& name);

	void unlock(std::string const& name);

private:
	sql::Driver *driver_;
	sql::Connection *con_;
	sql::Statement *stmt_;
	sql::ResultSet *res_;

	std::map<std::string, std::pair<bool, int>> achievements_;

	std::mutex mutex_;

	void load();
	void save();

	static std::string blobToString(std::istream* blob);

	static void printSQLException(sql::SQLException &e);
};
#endif