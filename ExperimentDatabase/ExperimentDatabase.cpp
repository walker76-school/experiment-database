#include "stdafx.h"
#include <iostream>
#include <mysqlx/xdevapi.h>

using namespace std;
using namespace mysqlx;

int main(int argc, char* argv[]) {
    try {
        SessionSettings initialOptions(
            SessionOption::USER, "root",
            SessionOption::PWD, "ALW1mys1@%",
            SessionOption::HOST, "localhost",
            SessionOption::SSL_MODE, SSLMode::REQUIRED
        );

        cout << "Creating session from options...";
        Session sess(initialOptions);
        cout << "success." << endl;

        cout << "Creating schema Experiment if it doesn't exist...";
        Schema db = sess.createSchema("Experiment", true);
        cout << "success." << endl;

        cout << "Closing initial session...";
        sess.close();
        cout << "success." << endl;

        SessionSettings connectionOptions(
            SessionOption::USER, "root",
            SessionOption::PWD, "ALW1mys1@%",
            SessionOption::HOST, "localhost",
            SessionOption::DB, "Experiment",
            SessionOption::SSL_MODE, SSLMode::REQUIRED
        );

        cout << "Creating session from options...";
        Session conn(connectionOptions);
        cout << "success." << endl;

        cout << "Retrieving database...";
        db = conn.getSchema("Experiment");
        cout << "success." << endl;

        cout << "Creating table Experiment...";
        SqlStatement createTable = conn.sql(
                "CREATE TABLE IF NOT EXISTS Experiment (" \
                "ExperimentId VARCHAR(255) NOT NULL," \
                "ManagerID CHAR(6) NOT NULL," \
                "StartDate DATE NOT NULL," \
                "DataEntryDate DATE NOT NULL," \
                "PRIMARY KEY(ExperimentId)" \
                ") "
            );
        SqlResult resultCode = createTable.execute();
        if (resultCode.getWarningsCount() <= 0) {
            cout << "success." << endl;
        }
        else {
            cout << "failure with warnings" << endl;
            return 1;
        }

        cout << "Creating table ParamterTypes...";
        createTable = conn.sql(
            "CREATE TABLE IF NOT EXISTS ParameterTypes (" \
            "ExperimentId VARCHAR(255) NOT NULL," \
            "ParameterName VARCHAR(255) NOT NULL," \
            "Type VARCHAR(255) NOT NULL," \
            "Required BOOLEAN NOT NULL," \
            "CONSTRAINT PK_ParameterTypes PRIMARY KEY (ExperimentId,ParameterName)," \
            "FOREIGN KEY(ExperimentId) REFERENCES Experiment(ExperimentId)" \
            ") "
        );
        resultCode = createTable.execute();
        if (resultCode.getWarningsCount() <= 0) {
            cout << "success." << endl;
        }
        else {
            cout << "failure with warnings" << endl;
            return 1;
        }

        cout << "Creating table ResultTypes...";
        createTable = conn.sql(
            "CREATE TABLE IF NOT EXISTS ResultTypes (" \
            "ExperimentId VARCHAR(255) NOT NULL," \
            "ResultName VARCHAR(255) NOT NULL," \
            "Type VARCHAR(255) NOT NULL," \
            "Required BOOLEAN NOT NULL," \
            "CONSTRAINT PK_ResultTypes PRIMARY KEY (ExperimentId,ResultName)," \
            "FOREIGN KEY(ExperimentId) REFERENCES Experiment(ExperimentId)" \
            ") "
        );
        resultCode = createTable.execute();
        if (resultCode.getWarningsCount() <= 0) {
            cout << "success." << endl;
        }
        else {
            cout << "failure with warnings" << endl;
            return 1;
        }

        cout << "Creating table Runs...";
        createTable = conn.sql(
            "CREATE TABLE IF NOT EXISTS Runs (" \
            "ExperimentId VARCHAR(255) NOT NULL," \
            "TimeOfRun DATETIME NOT NULL," \
            "ExperimenterSSN CHAR(6) NOT NULL," \
            "Success BOOLEAN NOT NULL," \
            "CONSTRAINT PK_Runs PRIMARY KEY (ExperimentId,TimeOfRun)," \
            "FOREIGN KEY(ExperimentId) REFERENCES Experiment(ExperimentId)" \
            ") "
        );
        resultCode = createTable.execute();
        if (resultCode.getWarningsCount() <= 0) {
            cout << "success." << endl;
        }
        else {
            cout << "failure with warnings" << endl;
            return 1;
        }

        cout << "Creating table RunsParameter...";
        createTable = conn.sql(
            "CREATE TABLE IF NOT EXISTS RunsParameter (" \
            "ExperimentId VARCHAR(255) NOT NULL," \
            "TimeOfRun DATETIME NOT NULL," \
            "ParameterName VARCHAR(255) NOT NULL," \
            "Value VARCHAR(255) NOT NULL," \
            "CONSTRAINT PK_RunsParameter PRIMARY KEY (ExperimentId,TimeOfRun,ParameterName)," \
            "FOREIGN KEY(ExperimentId, TimeOfRun) REFERENCES Runs(ExperimentId, TimeOfRun)," \
            "FOREIGN KEY(ExperimentId, ParameterName) REFERENCES ParameterTypes(ExperimentId, ParameterName)" \
            ") "
        );
        resultCode = createTable.execute();
        if (resultCode.getWarningsCount() <= 0) {
            cout << "success." << endl;
        }
        else {
            cout << "failure with warnings" << endl;
            return 1;
        }

        cout << "Creating table RunsResult...";
        createTable = conn.sql(
            "CREATE TABLE IF NOT EXISTS RunsResult (" \
            "ExperimentId VARCHAR(255) NOT NULL," \
            "TimeOfRun DATETIME NOT NULL," \
            "ResultName VARCHAR(255) NOT NULL," \
            "Value VARCHAR(255) NOT NULL," \
            "CONSTRAINT PK_RunsResult PRIMARY KEY (ExperimentId,TimeOfRun,ResultName)," \
            "FOREIGN KEY(ExperimentId) REFERENCES Experiment(ExperimentId)," \
            "FOREIGN KEY(ExperimentId, TimeOfRun) REFERENCES Runs(ExperimentId, TimeOfRun)," \
            "FOREIGN KEY(ExperimentId, ResultName) REFERENCES ResultTypes(ExperimentId, ResultName)" \
            ") "
        );
        resultCode = createTable.execute();
        if (resultCode.getWarningsCount() <= 0) {
            cout << "success." << endl;
        }
        else {
            cout << "failure with warnings" << endl;
            return 1;
        }
        /*
        // Accessing an existing table
        cout << "Accessing table MyGuests...";
        Table myTable = db.getTable("MyGuests", true);
        cout << "success." << endl;

        // Find a row in the SQL Table
        cout << "Table accessed, retrieving data...";
        RowResult myResult = myTable.select("*").execute();
        cout << "success." << endl;

        if (myResult.count() > 0) {
            // Print result
            Row row = myResult.fetchOne();
            cout << "     _id: " << row[0] << endl;
            cout << "    name: " << row[1] << endl;
            cout << "birthday: " << row[2] << endl;
        }
        else {
            cout << "No data retrieved from the table" << endl;
        }*/
        
    } catch (const Error &err) {
        cout << "ERROR: " << err << endl;
        return 1;
    } catch (std::exception &ex) {
        cout << "STD EXCEPTION: " << ex.what() << endl;
        return 1;
    } catch (const char *ex) {
        cout << "EXCEPTION: " << ex << endl;
        return 1;
    }
    return 0;
}
