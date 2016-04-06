#include "CheevoTracker.h"

CheevoTracker::CheevoTracker() {
	load();
}

CheevoTracker::~CheevoTracker() {
	save();
}

bool CheevoTracker::isUnlocked(std::string const& name) {
	std::lock_guard<std::mutex> lock(mutex_);
	return achievements_.at(name).first;
}

int CheevoTracker::getGamerScore(std::string const& name) {
	std::lock_guard<std::mutex> lock(mutex_);
	return achievements_.at(name).second;
}

void CheevoTracker::unlock(std::string const& name) {
	std::lock_guard<std::mutex> lock(mutex_);
	achievements_[name].first = true;
}

void CheevoTracker::load() {
	std::lock_guard<std::mutex> lock(mutex_);
	try {
		//Create a connection
		driver_ = get_driver_instance();
		con_ = driver_->connect("tcp://127.0.0.1:3306", "root", "esse49");

		//Connect to the database
		con_->setSchema("engineeringDB");

		stmt_ = con_->createStatement();
		res_ = stmt_->executeQuery("SELECT Name, IsAchieved, GamerScore from Cheevos");

		while (res_->next())
		{
			//Access column data by numeric offset (1 is the first column)

			//Get achievement data from database
			std::string name = blobToString(res_->getBlob(1));
			bool isUnlocked = res_->getBoolean(2);
			int score = res_->getInt(3);

			//Add achievement to the map
			achievements_[name] = std::make_pair(isUnlocked, score);

			printf("Achievement loaded: %s. IsUnlocked: %s. GamerScore: %d\n", name.c_str(), isUnlocked ? "TRUE" : "FALSE", score);
		}
	}
	catch (sql::SQLException &e)
	{
		printSQLException(e);
	}

	delete res_;
	delete stmt_;
	delete con_;
}

void CheevoTracker::save() {
	std::lock_guard<std::mutex> lock(mutex_);
	try {
		//Create a connection
		driver_ = get_driver_instance();
		con_ = driver_->connect("tcp://127.0.0.1:3306", "root", "esse49");

		//Connect to the database
		con_->setSchema("engineeringDB");

		for (auto itr = achievements_.begin(); itr != achievements_.end(); ++itr)
		{

			char* buffer = new char[VARCHAR_LENGTH];
			//Update achievement in database
			sprintf(
				buffer,
				"UPDATE Cheevos SET IsAchieved=%s WHERE Name=%s",
				itr->second.first == true ? "1" : "0",
				itr->first.c_str()
			);

			stmt_ = con_->createStatement();
			res_ = stmt_->executeQuery(buffer);
			delete stmt_;
			delete res_;
		}

		//Save changes to this database
		con_->commit();
	}
	catch (sql::SQLException &e)
	{
		printSQLException(e);
	}

	delete con_;
}

std::string CheevoTracker::blobToString(std::istream* blob) {
	//no error checking
	char * str = new char[255];
	blob->getline(str, VARCHAR_LENGTH);
	std::string result = str;
	delete[] str;
	return result;
}

void CheevoTracker::printSQLException(sql::SQLException& e) {
	std::cout << "# ERR: SQLException in " << __FILE__;
	std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
	std::cout << "# ERR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
}
