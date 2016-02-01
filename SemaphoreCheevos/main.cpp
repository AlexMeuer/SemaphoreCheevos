#include <stdlib.h>
#include <iostream>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define VARCHAR_LENGTH 255

using namespace std;

//res->getString(..) fails, this is the alternative.
string blobToString(istream* blob) {
	//no error checking
	char * str = new char[255];
	blob->getline(str, VARCHAR_LENGTH);
	string result = str;
	delete[] str;
	return result;
}

int main() {

	try
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		//Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "esse49");

		//Connect to the database
		con->setSchema("engineeringDB");

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT Name, IsAchieved from Cheevos");
		while (res->next())
		{
			//Access column data by alias or column name
			//cout << res->getString("_message") << endl;

			//Access column data by numeric offset (1 is the first column)
			cout << "Cheevo: " << blobToString(res->getBlob(1)) << endl;
			cout << "Achieved: " << (res->getBoolean(2) ? "TRUE" : "FALSE") << endl;


		}

		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cin.get();

	return EXIT_SUCCESS;
}