//DBS211 ZCC
//Group 9 
//Group members: Chi Ming Lai  Written functions: int findAccount(), void displayAccount(), void displayAllAccounts(), void insertAccount(), void updateAccount(), void deleteAccount()
// ------------------------------------------------------------------------------------------------------------------------------------------------------
//Chao Liu written: int findProduct(), void getNfindProduct(), void displayProduct(), void displayAllProducts()£¬void insertProduct(), void updateProduct(), void deleteProduct();
// ------------------------------------------------------------------------------------------------------------------------------------------------------
//Chirag Mudgil
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

struct Product {
	int product_Id = 0;
	string product_name;
	float product_price = 0.0f;
	string product_desciption;
	string product_status;
	int product_InCart = 0;
};

struct Purchase {
	int purchase_ID = 0;
	string purchase_FirstName;
	string purchase_LastName;
	string purchase_productQuantity;
	string purchase_paymentGateway;
	string purchase_Summary;
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
//
// Chao Liu
int findProduct(Connection* conn, const int productId, struct Product* product = nullptr);
void getNfindProduct(Connection* conn);
void displayProduct(Connection* conn, struct Product product);
void displayAllProducts(Connection* conn);
void insertProduct(Connection* conn, struct Product product);
void updateProduct(Connection* conn);
void deleteProduct(Connection* conn);
int findShoppingcart(Connection* conn, const int LinkCart);
// -----end here
// 
// Chirag Mudgil
int findPurchase(Connection* conn, const int purchase_ID, struct Purchase* purchase = nullptr);
void getNfindPurchase(Connection* conn);
void displayPurchase(Connection* conn, struct Purchase purchase);
void displayAllPurchase(Connection* conn);
void insertPurchase(Connection* conn, struct Purchase purchase);
void updatePurchase(Connection* conn);
void deletePurchase(Connection* conn);
int findPurchaseSummary(Connection* conn);
// -----end here

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
		std::cout << "DBS211 Project Milestone 3" << std::endl;
		Account acc = { 0 };
		Product product = { 0 };
		Purchase purchase = { 0 };
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
				getNfindProduct(conn);
				break;
			case 8:
				displayProduct(conn, product);
				break;
			case 9:
				displayAllProducts(conn);
				break;
			case 10:
				insertProduct(conn, product);
				break;
			case 11:
				updateProduct(conn);
				break;
			case 12:
				deleteProduct(conn);
				break;
			case 13:
				getNfindPurchase(conn);
				break;
			case 14:
				displayPurchase(conn, purchase);
				break;
			case 15:
				displayAllPurchase(conn);
				break;
			case 16:
				insertPurchase(conn, purchase);
				break;
			case 17:
				updatePurchase(conn);
				break;
			case 18:
				deletePurchase(conn);
			case 19:
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
	std::cout << "--------------------------" << std::endl;
	std::cout << "7.  Find product" << endl;
	std::cout << "8.  Product detail" << endl;
	std::cout << "9.  Display All Product Details" << endl;
	std::cout << "10. Add New Product" << endl;
	std::cout << "11. Update Product" << endl;
	std::cout << "12. Delete Product" << endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "13. Find Purchase" << endl;
	std::cout << "14. Purchase Detail" << endl;
	std::cout << "15. Display All the Purchase Details" << endl;
	std::cout << "16. Add New Purchase" << endl;
	std::cout << "17. Update Purchase" << endl;
	std::cout << "18. Delete Purchase" << endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "19. Exit" << std::endl;

	std::cout << "\nEnter your choice: ";

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
			std::cout << acc.Acc_id << " ";
			std::cout.width(17);
			std::cout << acc.Acc_loginName << " ";
			std::cout.width(20);
			std::cout << acc.Acc_password << " ";
			std::cout.width(11);
			std::cout << acc.Acc_balance << " ";
			std::cout.width(7);
			std::cout << acc.Acc_Tier << " ";
			std::cout.width(7);
			cout.unsetf(ios::left);
			std::cout << acc.ACC_customer_ID << " " << std::endl;




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

////Functions made by Chao Liu
int findProduct(Connection* conn, const int product_Id, struct Product* product) {
	try {
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT productId, productName, productPrice, productDesciption, productStatus, productInCart FROM product WHERE productId =" + to_string(product_Id)); // Query

		if (rs->next()) {

			if (product != nullptr) {
				// store all the data in Account type
				product->product_Id = rs->getInt(1);
				product->product_name = rs->getString(2);
				product->product_price = rs->getFloat(3);
				product->product_desciption = rs->getString(4);
				product->product_status = rs->getString(5);
				product->product_InCart = rs->getInt(6);
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

void getNfindProduct(Connection* conn) {
	int find, productId;
	std::cout << "Enter an Product ID: ";
	std::cin >> productId;
	find = findProduct(conn, productId);
	if (find == 1) {
		std::cout << "\nValid Product ID" << std::endl;
	}
	else {
		std::cout << "\nInvalid Product ID" << std::endl;
	}
}

void displayProduct(Connection* conn, struct Product product) {
	int productID;
	std::cout << "Enter the Product ID: ";
	std::cin >> productID;

	if (!std::cin || findProduct(conn, productID, &product) == 0) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Product " << productID << " does not exist." << std::endl;
	}
	else {

		std::cout << "\nValid Input\n" << std::endl;
		std::cout << "Search in Table...\n" << std::endl;
		try {
			Statement* stmt2 = conn->createStatement();
			ResultSet* rs2 = stmt2->executeQuery("SELECT productId, productName, productPrice, productDesciption, productStatus, productInCart FROM product WHERE productId =" + to_string(productID)); // Query
			while (rs2->next()) {
				std::cout << "\n---------------- Product Information ----------------" << std::endl;

				product.product_Id = rs2->getInt(1);
				product.product_name = rs2->getString(2);
				product.product_price = rs2->getFloat(3);
				product.product_desciption = rs2->getString(4);
				product.product_status = rs2->getString(5);
				product.product_InCart = rs2->getInt(6);

				std::cout << std::endl;
				std::cout << "Product Number: " << product.product_Id;

				std::cout << std::endl;
				std::cout << "Product Name: " << product.product_name;

				std::cout << std::endl;
				std::cout << "Product Price: " << product.product_price;

				std::cout << std::endl;
				std::cout << "Product Desciption: " << product.product_desciption;

				std::cout << std::endl;
				std::cout << "Product Status: " << product.product_status;

				std::cout << std::endl;
				std::cout << "The number of cart which product in: " << product.product_InCart << std::endl;
			}
			conn->terminateStatement(stmt2);
		}
		catch (SQLException& sqlExcp) {
			std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
		}
	}
}

void displayAllProducts(Connection* conn) {

	try {

		Statement* stmt3 = conn->createStatement();
		ResultSet* rs3 = stmt3->executeQuery("SELECT * FROM product ORDER BY productId"); // Query
		std::cout.fill(' ');
		std::cout << "\nID    " << "Product_name" << setw(23) << "Product_Price" << setw(20) << "Product_Desciption" << setw(27) << "Product_Status  " << setw(17) << "Product_InCart" << std::endl;
		std::cout.fill('-');
		std::cout.width(106);
		std::cout << "" << std::endl;

		Product product;
		while (rs3->next()) {
			product.product_Id = rs3->getInt(1);
			product.product_name = rs3->getString(2);
			product.product_price = rs3->getFloat(3);
			product.product_desciption = rs3->getString(4);
			product.product_status = rs3->getString(5);
			product.product_InCart = rs3->getInt(6);

			std::cout.fill(' ');
			std::cout.width(5);
			std::cout << left << product.product_Id << " ";
			std::cout.fill(' ');
			std::cout.width(21);
			std::cout << left << product.product_name << " ";
			std::cout.fill(' ');
			std::cout.width(14);
			std::cout << left << product.product_price << " ";
			std::cout.fill(' ');
			std::cout.width(28);
			std::cout << left << product.product_desciption << " ";
			std::cout.fill(' ');
			std::cout.width(18);
			std::cout << left << product.product_status << " ";
			std::cout.fill(' ');
			std::cout.width(7);
			std::cout << right << product.product_InCart << " " << std::endl;
		}
		std::cout.fill('-');
		std::cout.width(106);
		std::cout << "" << std::endl;

		conn->terminateStatement(stmt3);
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

}
void insertProduct(Connection* conn, struct Product product) {
	try {
		std::cout << "\nProduct ID: ";
		std::cin >> product.product_Id;
		std::cin.ignore();
		std::cout << "Link shoppingcart ID ( 1 - 10 ): ";
		std::cin >> product.product_InCart;
		std::cin.ignore();
		if (findProduct(conn, product.product_Id) == 1) {
			std::cout << "\n A product with the same product ID exists" << std::endl;
		}
		else if (findShoppingcart(conn, product.product_InCart) == 0) {
			std::cout << "\n The Linked shoppingcart is not existed" << std::endl;
		}
		else {
			Statement* stmt = conn->createStatement();
			stmt->setSQL("INSERT INTO product(productId, productName, productPrice, productDesciption, productStatus, productInCart) VALUES(:1,:2,:3,:4,:5,:6)");
			std::cout << "Product Name: ";
			std::getline(std::cin, product.product_name);
			std::cout << "Product Price: ";
			std::cin >> product.product_price;
			std::cin.ignore();
			std::cout << "Product Desciption: ";
			std::getline(std::cin, product.product_desciption);
			std::cout << "Product Status (IN STOCK or Discontinued): ";
			std::getline(std::cin, product.product_status);


			// set into stmt statment
			stmt->setInt(1, product.product_Id);
			stmt->setString(2, product.product_name);
			stmt->setFloat(3, product.product_price);
			stmt->setString(4, product.product_desciption);
			stmt->setString(5, product.product_status);
			stmt->setInt(6, product.product_InCart);

			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe new product is added successfully. " << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
void updateProduct(Connection* conn) {
	int pro_id;
	std::cout << "Enter the product id for update: ";
	std::cin >> pro_id;
	std::cin.ignore();
	try {
		Product product;
		float price;
		if (findProduct(conn, pro_id) == 1) {
			Statement* stmt = conn->createStatement();
			ResultSet* rs = stmt->executeQuery("SELECT productId, productName, productPrice, productInCart FROM product WHERE productId =" + to_string(pro_id));
			while (rs->next()) {
				product.product_Id = rs->getInt(1);
				product.product_name = rs->getString(2);
				product.product_price = rs->getFloat(3);
				product.product_InCart = rs->getInt(4);

				std::cout << std::endl;
				std::cout << "Product Number: " << product.product_Id;
				std::cout << std::endl;
				std::cout << "Product Name: " << product.product_name;

				std::cout << std::endl;
				std::cout << "Product Price: " << product.product_price;

				std::cout << std::endl;
				std::cout << "The number of cart which product in: " << product.product_InCart << std::endl;
			}
			std::cout << "Enter a new number to update the price: ";
			std::cin >> price;
			std::cin.ignore(1000, '\n');
			stmt->setSQL("UPDATE product SET productPrice =:1 WHERE productId=:2");
			stmt->setFloat(1, price);
			stmt->setInt(2, pro_id);
			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe product's price is updated successfully." << std::endl;
		}
		else {
			std::cout << "\nProduct " << pro_id << " does not exist." << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
void deleteProduct(Connection* conn) {
	struct Product product;
	int pro_id;
	std::cout << "Enter the product id for delete: ";
	std::cin >> pro_id;
	std::cin.ignore();
	try {
		if (findProduct(conn, pro_id) == 1) {
			Statement* stmt = conn->createStatement();
			stmt->setSQL("DELETE FROM product WHERE productId =:1");
			stmt->setInt(1, pro_id);
			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe product with ID " << pro_id << " is deleted successfully." << std::endl;
		}
		else {
			std::cout << "\nThe product with ID " << pro_id << " does not exist." << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}

int findShoppingcart(Connection* conn, const int LinkCart) {
	try {
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT cartId FROM shoppingcart WHERE cartId =" + to_string(LinkCart)); // Query
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
////Functions made by Chao Liu (END)


//Functions made by Chirag Mudgil - fixed by Chao Liu
int findPurchase(Connection* conn, const int purchcase_ID, struct Purchase* purchase) {
	try {
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT id, firstname, lastname, product_quantity, payment_gateway,summary FROM purchase WHERE id =" + to_string(purchcase_ID)); // Query

		if (rs->next()) {

			if (purchase != nullptr) {
				// store all the data in Account type
				purchase->purchase_ID = rs->getInt(1);
				purchase->purchase_FirstName = rs->getString(2);
				purchase->purchase_LastName = rs->getString(3);
				purchase->purchase_productQuantity = rs->getString(4);
				purchase->purchase_paymentGateway = rs->getString(5);
				purchase->purchase_Summary = rs->getString(6);
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

void getNfindPurchase(Connection* conn) {
	int find, purchase_Id;
	std::cout << "Enter a Purchase ID: ";
	std::cin >> purchase_Id;
	find = findPurchase(conn, purchase_Id);
	if (find == 1) {
		std::cout << "\nValid Purchase ID" << std::endl;
	}
	else {
		std::cout << "\nInvalid Purchase ID" << std::endl;
	}
}

void displayPurchase(Connection* conn, struct Purchase purchase) {
	int purchase_ID;
	std::cout << "Enter the Purchase ID: ";
	std::cin >> purchase_ID;

	if (!std::cin || findPurchase(conn, purchase_ID, &purchase) == 0) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << "Purchase " << purchase_ID << " does not exist." << std::endl;
	}
	else {

		std::cout << "\nValid Input\n" << std::endl;
		std::cout << "Search in Table...\n" << std::endl;
		try {
			Statement* stmt2 = conn->createStatement();
			ResultSet* rs2 = stmt2->executeQuery("SELECT id, firstname, lastname, product_quantity, payment_gateway,summary FROM purchase WHERE id =" + to_string(purchase_ID)); // Query
			while (rs2->next()) {
				std::cout << "\n---------------- Purchase Information ----------------" << std::endl;

				purchase.purchase_ID = rs2->getInt(1);
				purchase.purchase_FirstName = rs2->getString(2);
				purchase.purchase_LastName = rs2->getString(3);
				purchase.purchase_productQuantity = rs2->getString(4);
				purchase.purchase_paymentGateway = rs2->getString(5);
				purchase.purchase_Summary = rs2->getString(6);


				std::cout << std::endl;
				std::cout << "Purchase ID: " << purchase.purchase_ID;

				std::cout << std::endl;
				std::cout << "Purchase First Name: " << purchase.purchase_FirstName;

				std::cout << std::endl;
				std::cout << "Purchase Last Name: " << purchase.purchase_LastName;

				std::cout << std::endl;
				std::cout << "Purchase Product Quantity: " << purchase.purchase_productQuantity;

				std::cout << std::endl;
				std::cout << "Purchase Payment Gateway: " << purchase.purchase_paymentGateway;

				std::cout << std::endl;
				std::cout << "Purchase Summary: " << purchase.purchase_Summary << std::endl;
			}
			conn->terminateStatement(stmt2);
		}
		catch (SQLException& sqlExcp) {
			std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
		}
	}
}

void displayAllPurchase(Connection* conn) {

	try {

		Statement* stmt3 = conn->createStatement();
		ResultSet* rs3 = stmt3->executeQuery("SELECT * FROM purchase ORDER BY id"); // Query
		std::cout.fill(' ');
		std::cout << "\nID    " << "Firstname  " << setw(11) << "Lastname  " << setw(10) << "Quantity  " << setw(8) << "Payment Method  " << setw(2) << "Summary" << std::endl;
		std::cout.fill('-');
		std::cout.width(77);
		std::cout << "" << std::endl;

		Purchase purchase;
		while (rs3->next()) {

			purchase.purchase_ID = rs3->getInt(1);
			purchase.purchase_FirstName = rs3->getString(2);
			purchase.purchase_LastName = rs3->getString(3);
			purchase.purchase_productQuantity = rs3->getString(4);
			purchase.purchase_paymentGateway = rs3->getString(5);
			purchase.purchase_Summary = rs3->getString(6);

			std::cout.fill(' ');
			std::cout.width(5);
			std::cout << left << purchase.purchase_ID << " ";
			std::cout.fill(' ');
			std::cout.width(11);
			std::cout << left << purchase.purchase_FirstName << " ";
			std::cout.fill(' ');
			std::cout.width(10);
			std::cout << left << purchase.purchase_LastName << " ";
			std::cout.fill(' ');
			std::cout.width(8);
			std::cout << left << purchase.purchase_productQuantity << " ";
			std::cout.fill(' ');
			std::cout.width(15);
			std::cout << left << purchase.purchase_paymentGateway << " ";
			std::cout.fill(' ');
			std::cout.width(7);
			std::cout << right << purchase.purchase_Summary << " " << std::endl;
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
void insertPurchase(Connection* conn, struct Purchase purchase) {
	try {
		std::cout << "\nPurhcase ID: ";
		std::cin >> purchase.purchase_ID;
		std::cin.ignore();
		std::cout << "Link purchase ID ( 1 - 10 ): ";
		std::cin >> purchase.purchase_ID;
		std::cin.ignore();
		if (findPurchase(conn, purchase.purchase_ID) == 1) {
			std::cout << "\n A purchase with the same purchase ID exists" << std::endl;
		}

		else {
			Statement* stmt = conn->createStatement();
			stmt->setSQL("INSERT INTO purchase(Id, firstName, lastName, productQuantity, paymentGateway, purchaseSummary) VALUES(:1,:2,:3,:4,:5,:6)");
			std::cout << "Purchse Id: ";

			std::cout << "Purchase First Name: ";
			std::cin >> purchase.purchase_FirstName;
			std::cin.ignore();
			std::cout << "Purchase Last Name: ";
			std::getline(std::cin, purchase.purchase_LastName);
			std::cout << "Product Quantity (IN STOCK or Discontinued): ";
			std::getline(std::cin, purchase.purchase_productQuantity);


			// set into stmt statment
			stmt->setInt(1, purchase.purchase_ID);
			stmt->setString(2, purchase.purchase_FirstName);
			stmt->setString(3, purchase.purchase_LastName);
			stmt->setString(4, purchase.purchase_productQuantity);
			stmt->setString(5, purchase.purchase_paymentGateway);
			stmt->setString(6, purchase.purchase_Summary);

			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe new purchase is added successfully. " << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
void updatePurchase(Connection* conn) {
	int pur_id;
	std::cout << "Enter the purchase id for update: ";
	std::cin >> pur_id;
	std::cin.ignore();
	try {
		Purchase purchase;
		float price;
		if (findPurchase(conn, pur_id) == 1) {
			Statement* stmt = conn->createStatement();
			ResultSet* rs = stmt->executeQuery("SELECT Id, FirstName, lastName, purchaseSummary FROM purchase WHERE Id =" + to_string(pur_id));
			while (rs->next()) {
				purchase.purchase_ID = rs->getInt(1);
				purchase.purchase_FirstName = rs->getString(2);
				purchase.purchase_LastName = rs->getFloat(3);
				purchase.purchase_Summary = rs->getInt(4);

				std::cout << std::endl;
				std::cout << "Purchase Number: " << purchase.purchase_ID;
				std::cout << std::endl;
				std::cout << "First Name: " << purchase.purchase_FirstName;

				std::cout << std::endl;
				std::cout << "Purchase last name: " << purchase.purchase_LastName;

				std::cout << std::endl;
				std::cout << "The summary of the payment: " << purchase.purchase_Summary << std::endl;
			}
			std::cout << "Enter a new number to update the price: ";
			std::cin >> price;
			std::cin.ignore(1000, '\n');
			stmt->setSQL("UPDATE purchase SET firstName =:1 WHERE Id=:2");
			stmt->setFloat(1, price);
			stmt->setInt(2, pur_id);
			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe purchase firstName is updated successfully." << std::endl;
		}
		else {
			std::cout << "\nPurchase " << pur_id << " does not exist." << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
void deletePurchase(Connection* conn) {
	struct Purchase purchase;
	int pur_id;
	std::cout << "Enter the purchase id for delete: ";
	std::cin >> pur_id;
	std::cin.ignore();
	try {
		if (findPurchase(conn, pur_id) == 1) {
			Statement* stmt = conn->createStatement();
			stmt->setSQL("DELETE FROM purchase WHERE Id =:1");
			stmt->setInt(1, pur_id);
			stmt->executeUpdate();
			conn->commit();
			conn->terminateStatement(stmt);
			std::cout << "\nThe purchase with ID " << pur_id << " is deleted successfully." << std::endl;
		}
		else {
			std::cout << "\nThe purchase with ID " << pur_id << " does not exist." << std::endl;
		}
	}
	catch (SQLException& sqlExcp) {
		std::cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}

int findPurchaseSummary(Connection* conn, const int PurchaseSummary) {
	try {
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT Id FROM purchaseSummary WHERE Id =" + to_string(PurchaseSummary)); // Query
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
