//DBS211 ZCC
//Group 9 
//Group members: Chi Ming Lai  Written functions: int findAccount(), void displayAccount(), void displayAllAccounts(), void insertAccount(), void updateAccount(), void deleteAccount()
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Email: cmlai@myseneca.ca / cmudgil@myseneca.ca / cliu155@myseneca.ca
//Project milestone 3

#include <iostream>
#include <iomanip>
#include <string>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using  namespace oracle::occi;
using namespace std;

struct Account {
	int Acc_id = 0;
	string Acc_loginName;
	string Acc_password;
	string Acc_balance;
	string Acc_Tier;
	int ACC_customer_ID = 0;
};


int menu(void); // display the menu and prompt the user to enter the choice

// ----- Chi Ming Lai
int findAccount(Connection* conn, const int Accid, struct Account* acc = nullptr); // return 1 and account found, 0 otherwise
void displayAccount(Connection* conn, struct Account acc); // display the account if found
void displayAllAccounts(Connection* conn); // display all data in the account table
void insertAccount(Connection* conn, struct Account acc); // insert a row to the account
void updateAccount(Connection* conn); // update "balance" attribute of the account
void deleteAccount(Connection* conn); // delete an account record
// Helper function
int findCustomer(Connection* conn, const int LinkCus);
void getNfindAccount(Connection* conn);
// ----- end here


int main(void)
{
	// OCCI Variables
	Environment* env = nullptr;
	Connection* conn = nullptr;
	// User Variables
	string str;
	string usr = "dbs211_221zc20"; // this is your login assigned to you
	string pass = "22022219"; // this is your password assigned to you
	string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";

	try {
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(usr, pass, srv);
		std::cout << "Connection is Successful!: Group 9: Chi Ming Lai / Chao Liu / Chirag Mudgil" << std::endl;
		std::cout << "DBS211 Project Milestone 2" << std::endl;
		Account acc = { 0 };
		bool isExit = false;
		do {

			int option = menu();
			switch (option) {
			case 1:
				getNfindAccount(conn);
				break;
			case 2: 
				displayAccount(conn, acc);
				break;
			case 3:
				displayAllAccounts(conn);
				break;
			case 4:
				insertAccount(conn, acc);
				break;
			case 5:
				updateAccount(conn);
				break;
			case 6:
				deleteAccount(conn);
				break;
			case 7:
				isExit = true;
				break;
			default:
				std::cout << "\nInvalid Choice!" << std::endl;
				break;
			}
		} while (!isExit);
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}

int menu(void) {

	int input;
	std::cout << "\n******Online store menu******" << std::endl;

	std::cout << "1.  Find Account" << std::endl;
	std::cout << "2.  Account Report" << std::endl;
	std::cout << "3.  Display All Account" << std::endl;
	std::cout << "4.  Add Account" << std::endl;
	std::cout << "5.  Update Account" << std::endl;
	std::cout << "6.  Delete Account" << std::endl;
	std::cout << "7.  Exit" << std::endl;

	std::cout << "Enter your choice: ";

	do {
		std::cin.clear();
		std::cin >> input;
		std::cin.ignore(100, '\n');
	} while (std::cin.fail());

	return input;

}


int findAccount(Connection* conn, const int Accid, struct Account* acc) {
	try {
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT account_id, loginName, password, Balance, tier, link_customerID FROM account WHERE account_id =" + to_string(Accid)); // Query

		if (rs->next()) {

			if (acc != nullptr) {
				// store all the data in Account type
				acc->Acc_id = rs->getInt(1);
				acc->Acc_loginName = rs->getString(2);
				acc->Acc_password = rs->getString(3);
				acc->Acc_balance = rs->getString(4);
				acc->Acc_Tier = rs->getString(5);
				acc->ACC_customer_ID = rs->getInt(6);
			}
			return 1;
		}
		else {
			return 0;
		}

		conn->terminateStatement(stmt);
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

	return 0;

}

void displayAccount(Connection* conn, struct  Account acc) {
	int accountID;
	std::cout << "Enter your Account ID: ";
	std::cin >> accountID;

	if (!std::cin || findAccount(conn, accountID, &acc) == 0) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Account " << accountID << " does not exist." << std::endl;
	}
	else {

		std::cout << "\nValid Input\n" << std::endl;
		std::cout << "Search in Table...\n" << std::endl;
		try {
			Statement* stmt2 = conn->createStatement();
			ResultSet* rs2 = stmt2->executeQuery("SELECT account_id, loginName, password, Balance, tier, link_customerID FROM account WHERE Account_id =" + to_string(accountID));
			while (rs2->next()) {
				std::cout << "\n---------------- Account Information ----------------" << std::endl;

				acc.Acc_id = rs2->getInt(1);
				acc.Acc_loginName = rs2->getString(2);
				acc.Acc_password = rs2->getString(3);
				acc.Acc_balance = rs2->getString(4);
				acc.Acc_Tier = rs2->getString(5);
				acc.ACC_customer_ID = rs2->getInt(6);

				std::cout << std::endl;
				std::cout << "Account Number: " << acc.Acc_id;

				std::cout << std::endl;
				std::cout << "Login Name: " << acc.Acc_loginName;

				std::cout << std::endl;
				std::cout << "Password: " << acc.Acc_password;

				std::cout << std::endl;
				std::cout << "Balance: " << acc.Acc_balance;

				std::cout << std::endl;
				std::cout << "Account Tier: " << acc.Acc_Tier;

				std::cout << std::endl;
				std::cout << "Linked customer ID: " << acc.ACC_customer_ID << std::endl;
			}
			conn->terminateStatement(stmt2);
		}
		catch (SQLException& sqlExcp) {
			std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
		}
	}
}

void displayAllAccounts(Connection* conn) {

	try {

		Statement* stmt3 = conn->createStatement();
		ResultSet* rs3 = stmt3->executeQuery("SELECT account_id, loginName, password, Balance, tier, link_customerID FROM account ORDER BY account_id");
		std::cout.fill(' ');
		std::cout << "\nAcID  " << "LoginName" << setw(17) << "Password" << setw(20) << "Balance" << setw(11) << "Tier  " << setw(12) << "Linked Cust ID" << std::endl;
		std::cout.fill('-');
		std::cout.width(77);
		std::cout << "" << std::endl;
		std::cout.fill(' ');

		Account acc;
		while (rs3->next()) {
			acc.Acc_id = rs3->getInt(1);
			acc.Acc_loginName = rs3->getString(2);
			acc.Acc_password = rs3->getString(3);
			acc.Acc_balance = rs3->getString(4);
			acc.Acc_Tier = rs3->getString(5);
			acc.ACC_customer_ID = rs3->getInt(6);
			
			std::cout.setf(ios::left);
			std::cout.width(5);
			std::cout  << acc.Acc_id << " ";
			std::cout.width(17);
			std::cout << acc.Acc_loginName << " ";
			std::cout.width(20);
			std::cout  << acc.Acc_password << " ";
			std::cout.width(11);
			std::cout  << acc.Acc_balance << " ";
			std::cout.width(7);
			std::cout  << acc.Acc_Tier << " ";
			std::cout.width(7);
			cout.unsetf(ios::left);
			std::cout  << acc.ACC_customer_ID << " " << std::endl;




		}
		std::cout.fill('-');
		std::cout.width(77);
		std::cout << "" << std::endl;

		conn->terminateStatement(stmt3);
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

}

void insertAccount(Connection* conn, struct Account acc) {
	try {
		//check ac id is valid
		std::cout << "\naccount ID: ";
		std::cin >> acc.Acc_id;
		std::cin.ignore();
		//check if the parent key existed
		std::cout << "Link customer ID ( 1 - 10 ): ";
		std::cin >> acc.ACC_customer_ID;
		std::cin.ignore();
		if (findAccount(conn, acc.Acc_id) == 1) {
			std::cout << "\n A account with the same account number exists" << std::endl;
		}
		else if (findCustomer(conn, acc.ACC_customer_ID) == 0) {
			std::cout << "\n The Linked customer is not existed" << std::endl;
		}
		else {
			Statement* stmt = conn->createStatement();
			stmt->setSQL("INSERT INTO account(account_id, loginName, password, balance, tier, link_customerID) VALUES(:1,:2,:3,:4,:5,:6)");
			/*std::cout << "\nAccount ID: ";
			std::cin >> acc.Acc_id;*/
			std::cout << "Login Name: ";
			std::getline(std::cin, acc.Acc_loginName);
			std::cout << "Password: ";
			std::getline(std::cin, acc.Acc_password);
			std::cout << "Balance: ";
			std::cin >> acc.Acc_balance;
			std::cin.ignore();
			std::cout << "Tier: ";
			std::getline(std::cin, acc.Acc_Tier);
			

			// set into stmt statment
			stmt->setInt(1, acc.Acc_id);
			stmt->setString(2, acc.Acc_loginName);
			stmt->setString(3, acc.Acc_password);
			stmt->setString(4, acc.Acc_balance);
			stmt->setString(5, acc.Acc_Tier);
			stmt->setInt(6, acc.ACC_customer_ID);

			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe new account is added successfully. " << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
void updateAccount(Connection* conn) {
	int acc_id;
	std::cout << "Enter the account id for update: ";
	std::cin >> acc_id;
	std::cin.ignore();
	try {
		Account temp;
		string password;
		if (findAccount(conn, acc_id) == 1) {
			Statement* stmt = conn->createStatement();
			ResultSet* rs = stmt->executeQuery("SELECT account_id, loginName, password, link_customerID FROM account WHERE Account_id =" + to_string(acc_id));
			while (rs->next()) {
				temp.Acc_id = rs->getInt(1);
				temp.Acc_loginName = rs->getString(2);
				temp.Acc_password = rs->getString(3);
				temp.ACC_customer_ID = rs->getInt(4);
				std::cout << std::endl;
				std::cout << "Account Number: " << temp.Acc_id;
				std::cout << std::endl;
				std::cout << "Login Name: " << temp.Acc_loginName;
				std::cout << std::endl;
				std::cout << "Password: " << temp.Acc_password;
				std::cout << std::endl;
				std::cout << "Link customer ID: " << temp.ACC_customer_ID << std::endl;
			}
			std::cout << "Enter a new password to update the records: ";
			std::cin >> password;
			std::cin.ignore(1000, '\n');
			stmt->setSQL("UPDATE account SET password =:1 WHERE account_id=:2");
			stmt->setString(1, password);
			stmt->setInt(2, acc_id);
			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe account's password is updated successfully." << std::endl;
		}
		else {
			std::cout << "\nAccount " << acc_id << " does not exist." << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
void deleteAccount(Connection* conn) {
	struct Account temp;
	int acc_id;
	std::cout << "Enter the account id for delete: ";
	std::cin >> acc_id;
	std::cin.ignore();
	try {
		if (findAccount(conn, acc_id) == 1) {
			Statement* stmt = conn->createStatement();
			stmt->setSQL("DELETE FROM account WHERE account_id =:1");
			stmt->setInt(1, acc_id);
			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe account with ID " << acc_id << " is deleted successfully." << std::endl;
		}
		else {
			std::cout << "\nThe account with ID " << acc_id << " does not exist." << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}

// Helper functions
int findCustomer(Connection* conn, const int LinkCus) {
	try {
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT id FROM customer WHERE id =" + to_string(LinkCus)); // Query
		if (rs->next()) {
			return 1;
		}
		else {
			return 0;
		}
		conn->terminateStatement(stmt);
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();

	}
	return 0;
}

void getNfindAccount(Connection* conn) {
	int find, acc_id;
	std::cout << "Enter an account ID: ";
	std::cin >> acc_id;
	find = findAccount(conn, acc_id);
	if (find == 1) {
		std::cout << "\nValid Account ID" << std::endl;
	}
	else {
		std::cout << "\nInvalid Account ID" << std::endl;
	}
}

// Functions made by Chi Ming Lai (End)


