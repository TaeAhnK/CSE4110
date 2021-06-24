#include <stdio.h>
#include <mysql.h>
#include <string.h>

#pragma comment(lib, "libmysql.lib")

// Database Info
const char* host = "localhost";
const char* user = "20170175";
const char* pw = "cs1234";
const char* db = "project2";
int today = 20210614;

// Connect to DB
MYSQL* connection = NULL;
MYSQL conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;

// SQL Files
const char* startFile = "20170175_start.txt";
const char* endFile = "20170175_end.txt";

// Function Prototypes
void DBGenerate();
void DBDrop();
void type();
void type1();
void type2();
void type3();
void type4();
void type5();
void type6();
void type7();

char query[1000];

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	if (mysql_init(&conn) == NULL)
		printf("Error : mysql_init() error\n");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);

	if (connection == NULL) {
		printf("%d Error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	DBGenerate();
	type();
	DBDrop();

	mysql_close(connection);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Generate DB with SQL create insert file
void DBGenerate() {
	int state = 0;
	FILE* fp = NULL;

	fopen_s(&fp, startFile, "r");
	if (fp == NULL) {
		printf("Error: Cannot open 20170175_start.txt");
		exit(1);
	}

	while (!feof(fp)) {
		fgets(&query, sizeof(query), fp);
		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}
		memset(query, '\0', sizeof(query));
	}

	fclose(fp);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Delete DB with SQL drop delete file
void DBDrop() {
	int state = 0;
	FILE* fp = NULL;

	fopen_s(&fp, endFile, "r");
	if (fp == NULL) {
		printf("Error: Cannot open 20170175_end.txt");
		exit(1);
	}

	while (!feof(fp)) {
		fgets(&query, sizeof(query), fp);

		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
			memset(query, '\0', sizeof(query));
		}
	}

	fclose(fp);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE
void type() {
	int typeNum;
	
	while (1) {
		// Select Menu 
		printf("---------- SELECT QUERY TYPES ----------\n\n");
		printf("1. TYPE 1\n");
		printf("2. TYPE 2\n");
		printf("3. TYPE 3\n");
		printf("4. TYPE 4\n");
		printf("5. TYPE 5\n");
		printf("6. TYPE 6\n");
		printf("7. TYPE 7\n");
		printf("0. QUIT\n");
		// Enter the TYPE Number
		
		scanf_s("%d%*c", &typeNum, sizeof(typeNum));

		// QUIT
		if (typeNum == 0) {
			printf("----------------- EXIT -----------------\n");
			return;
		}

		// TYPE
		else if (typeNum == 1) {
			type1();
		}
		else if (typeNum == 2) {
			type2();
		}
		else if (typeNum == 3) {
			type3();
		}
		else if (typeNum == 4) {
			type4();
		}
		else if (typeNum == 5) {
			type5();
		}
		else if (typeNum == 6) {
			type6();
		}
		else if (typeNum == 7) {
			type7();
		}

		// Exception
		else {
			printf("Error: Input Numbers between 0 and 7\n");
		}
	}
}

// TYPE 1
void type1() {
	int typeNum;
	char brand[30] ;
	int k;
	int year = today / 10000;
	int state = 0;
	memset(brand, '\0', sizeof(brand));
	memset(query, '\0', sizeof(query));

	// 1
	printf("---------------- TYPE 1 ----------------\n");
	printf("** Show the sales trends for a particular brand over the past k years. **\n");
	printf("  Enter the Brand (Ex: Model S, Model 3...): ");
	scanf_s("%[^\n]s", brand, sizeof(brand));
	printf("  Enter the k : ");
	scanf_s("%d", &k, sizeof(k));
	sprintf(query, "SELECT model.model_name, model.model_year, COUNT(model_name) as total_sold FROM vehicle JOIN model ON model.model_ID = vehicle.model_ID JOIN sells_vehicle ON sells_vehicle.VIN = vehicle.VIN WHERE model.model_type = \"%s\" and (sells_vehicle.date BETWEEN \"%d-01-01\" AND \"%d-12-31\") GROUP BY model.model_name, model_year;", brand, year-k+1, year);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%15s|%-5s|%-5s|\n", "Model", "Year", "Sold");
		printf("-----------------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%-15s|%-5s|%-5s|\n", sql_row[0], sql_row[1], sql_row[2]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));

	printf("---------- SELECT QUERY TYPES ----------\n\n");
	printf("1. TYPE 1-1\n");
	printf("0. Return\n");
	scanf_s("%d", &typeNum, sizeof(typeNum));
	
	if (typeNum == 0) {
		return;
	}

	// 1-1
	printf("--------------- TYPE 1-1 ---------------\n");
	printf("** Then break these data out by gender of the buyer. **\n");
	sprintf(query, "SELECT model_name, model_year, COUNT(model_name) as total_sold, count(case when gender = \"male\" then 1 end) as male, count(case when gender = \"female\" then 1 end) as female FROM vehicle JOIN model ON model.model_ID = vehicle.model_ID JOIN sells_vehicle ON sells_vehicle.VIN = vehicle.VIN JOIN customer ON customer.customer_ID = vehicle.customer_ID WHERE model.model_type = \"%s\" and (sells_vehicle.date BETWEEN \"%d-01-01\" AND \"%d-12-31\") GROUP BY model.model_name, model_year;", brand, year-k+1, year);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%15s|%-5s|%-5s|%-5s|%-6s|\n", "Model", "Year", "Sold", "Male", "Female");
		printf("------------------------------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%-15s|%-5s|%-5s|%-5s|%-6s|\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));
	
	printf("---------- SELECT QUERY TYPES ----------\n\n");
	printf("1. TYPE 1-1-1\n");
	printf("0. Return\n");
	scanf_s("%d", &typeNum, sizeof(typeNum));

	if (typeNum == 0) {
		return;
	}

	// 1-1-1
	printf("-------------- TYPE 1-1-1 --------------\n");
	printf("** Then by income range. **\n");
	sprintf(query, "SELECT model_name, model_year, COUNT(model_name) as total_sold, count(case when income between 0 and 10000 then 1 end) as \"~10,000\", count(case when income between 10001 and 50000 then 1 end) as \"~50,000\", count(case when income between 50001 and 100000 then 1 end) as \"~100,000\", count(case when income between 100001 and 150000 then 1 end) as \"~150,000\", count(case when income between 150001 and 200000 then 1 end) as \"~200,000\", count(case when income between 200001 and 10000000 then 1 end) as \"200,000+\", count(case when income > 10000000 then 1 end) as \"10,000,000+\" FROM vehicle JOIN model ON model.model_ID = vehicle.model_ID JOIN sells_vehicle ON sells_vehicle.VIN = vehicle.VIN JOIN customer ON customer.customer_ID = vehicle.customer_ID WHERE model.model_type = \"%s\" and (sells_vehicle.date BETWEEN \"%d-01-01\" AND \"%d-12-31\") GROUP BY model.model_name, model_year;", brand, year - k + 1, year);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%15s|%-5s|%-5s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|\n", "Model", "Year", "Sold", "~10,000", "~50,000", "~100,000", "~150,000", "~200,000", "200,000+", "10,000,000+");
		printf("-----------------------------------------------------------------------------------------------------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%-15s|%-5s|%-5s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));
	return;
}

// TYPE 2
void type2() {
	int typeNum;
	char query[1000];
	int k;
	int state = 0;
	int month = today/100;

	// 2
	printf("---------------- TYPE 2 ----------------\n");
	printf("** Show sales trends for various brands over the past k months. **\n");
	printf("  Enter the k : ");
	scanf_s("%d", &k, sizeof(k));
	sprintf(query, "SELECT model.model_type, model.model_name, model.model_year, COUNT(model_name) as total_sold FROM vehicle JOIN model ON model.model_ID = vehicle.model_ID JOIN sells_vehicle ON sells_vehicle.VIN = vehicle.VIN WHERE(sells_vehicle.date BETWEEN date_sub(\"%d\", interval %d month) AND \"%d\") GROUP BY model.model_type, model.model_name, model_year; ", today, k, today);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%12s|%15s|%-5s|%-5s|\n", "Brand", "Model", "Year", "Sold");
		printf("------------------------------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%-12s|%-15s|%-5s|%-5s|\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));
	
	printf("---------- SELECT QUERY TYPES ----------\n\n");
	printf("1. TYPE 2-1\n");
	printf("0. Return\n");
	scanf_s("%d", &typeNum, sizeof(typeNum));

	if (typeNum == 0) {
		return;
	}

	// 2-1
	printf("--------------- TYPE 2-1 ---------------\n");
	printf("** Then break these data out by gender of the buyer. **\n");
	sprintf(query, "SELECT model_type, model_name, model_year, COUNT(model_name) as total_sold, count(case when gender = \"male\" then 1 end) as male, count(case when gender = \"female\" then 1 end) as female FROM vehicle JOIN model ON model.model_ID = vehicle.model_ID JOIN sells_vehicle ON sells_vehicle.VIN = vehicle.VIN JOIN customer ON customer.customer_ID = vehicle.customer_ID WHERE(sells_vehicle.date BETWEEN date_sub(\"%d\", interval %d month) AND \"%d\") GROUP BY model.model_type, model.model_name, model_year; ", today, k, today);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%12s|%15s|%-5s|%-5s|%-5s|%-6s|\n", "Brand", "Model", "Year", "Sold", "Male", "Female");
		printf("-------------------------------------------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%12s|%15s|%-5s|%-5s|%-5s|%-6s|\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));
	

	printf("---------- SELECT QUERY TYPES ----------\n\n");
	printf("1. TYPE 2-1-1\n");
	printf("0. Return\n");
	scanf_s("%d", &typeNum, sizeof(typeNum));

	if (typeNum == 0) {
		return;
	}

	// 2-1-1
	printf("-------------- TYPE 2-1-1 --------------\n");
	printf("** Then by income range. **\n");
	sprintf(query, "SELECT model_type, model_name, model_year, COUNT(model_name) as total_sold, count(case when income between 0 and 10000 then 1 end) as \"~10,000\", count(case when income between 10001 and 50000 then 1 end) as \"~50,000\", count(case when income between 50001 and 100000 then 1 end) as \"~100,000\", count(case when income between 100001 and 150000 then 1 end) as \"~150,000\", count(case when income between 150001 and 200000 then 1 end) as \"~200,000\", count(case when income between 200001 and 10000000 then 1 end) as \"200,000+\", count(case when income > 10000000 then 1 end) as \"10,000,000+\" FROM vehicle JOIN model ON model.model_ID = vehicle.model_ID JOIN sells_vehicle ON sells_vehicle.VIN = vehicle.VIN JOIN customer ON customer.customer_ID = vehicle.customer_ID WHERE sells_vehicle.date BETWEEN date_sub(\"%d\", interval %d month) AND \"%d\" GROUP BY model.model_type, model.model_name, model_year; ", today, k, today);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%12s|%15s|%-5s|%-5s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|\n", "Brand", "Model", "Year", "Sold", "~10,000", "~50,000", "~100,000", "~150,000", "~200,000", "200,000+", "10,000,000+");
		printf("------------------------------------------------------------------------------------------------------------------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%12s|%-15s|%-5s|%-5s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|%-11s|\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));

	return;
}

// TYPE 3
void type3() {
	int typeNum;
	int k;
	int state = 0;
	char supplier[30];
	int date1;
	int date2;

	// 3
	printf("---------------- TYPE 3 ----------------\n");
	printf("** Find that transmissions made by supplier (company name) between two given dates are defective. **\n");
	printf("  Enter the Supplier : ");
	scanf_s("%[^\n]s", supplier, sizeof(supplier));
	printf("  Enter the Start Date (YYYYMMDD) : ");
	scanf_s("%d", &date1, sizeof(date1));
	printf("  Enter the End Date (YYYYMMDD) : ");
	scanf_s("%d", &date2, sizeof(date2));
	sprintf(query, "SELECT part_ID FROM sup_makes_part NATURAL JOIN part WHERE(date BETWEEN \"%d\" AND \"%d\") AND part_type = \"Transmission\" AND sup_makes_part.supplier_name = \"%s\";", date1, date2, supplier);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%6s|\n", "PartID");
		printf("--------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%6s|\n", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));

	printf("---------- SELECT QUERY TYPES ----------\n\n");
	printf("1. TYPE 3-1\n");
	printf("2. TYPE 3-2\n");
	printf("0. Return\n");
	scanf_s("%d", &typeNum, sizeof(typeNum));

	if (typeNum == 0) {
		return;
	}
	else if (typeNum == 1) {
		// 3-1
		printf("--------------- TYPE 3-1 ---------------\n");
		printf("** Find the VIN of each car containing such a transmission and the customer to which it was sold. **\n");
		sprintf(query, "SELECT VIN, customer_ID FROM sup_makes_part NATURAL JOIN part NATURAL JOIN uses_part NATURAL JOIN vehicle WHERE(date BETWEEN \"%d\" AND \"%d\") AND part_type = \"Transmission\" AND sup_makes_part.supplier_name = \"%s\"; ", date1, date2, supplier);
		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			printf("|%6s|%11s|\n", "VIN", "CustomerID");
			printf("--------------------\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("|%4s|%11s|\n", sql_row[0], sql_row[1]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n");
		memset(query, '\0', sizeof(query));

	}
	else if (typeNum == 2) {
		// 3-2
		printf("--------------- TYPE 3-2 ---------------\n");
		printf("** Find the dealer who sold the VIN and transmission for each vehicle containing these transmissions **\n");
		sprintf(query, "SELECT dealer_ID, vehicle.VIN FROM sup_makes_part NATURAL JOIN part  NATURAL JOIN uses_part NATURAL JOIN vehicle JOIN sells_vehicle ON vehicle.VIN = sells_vehicle.VIN WHERE(sup_makes_part.date BETWEEN \"%d\" AND \"%d\") AND part.part_type = \"Transmission\" AND sup_makes_part.supplier_name = \"%s\"; \; ", date1, date2, supplier);
		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			printf("|%9s|%6s|\n", "DealerID", "VIN");
			printf("------------------\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("|%9s|%5s|\n", sql_row[0], sql_row[1]);
			}
			mysql_free_result(sql_result);
		}
		printf("\n\n");
		memset(query, '\0', sizeof(query));
	}

	return;
}

// TYPE 4
void type4() {
	int k;
	int state = 0;
	int year;

	// 4
	printf("---------------- TYPE 4 ----------------\n");
	printf(" ** Find the top k brands by dollar-amount sold by the year. **\n");
	printf("  Enter k : ");
	scanf("%d", &k);
	printf("  Enter the Year (YYYY): ");
	scanf("%d", &year);
	sprintf(query, "SELECT model.model_type, sum(price) AS \"total_sold($)\" FROM vehicle JOIN model ON vehicle.model_ID = model.model_ID JOIN sells_vehicle ON vehicle.VIN = sells_vehicle.VIN WHERE YEAR(sells_vehicle.date) = %d GROUP BY model.model_type ORDER BY SUM(price) DESC LIMIT %d; ", year, k);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%9s|%10s|\n", "DealerID", "totalSold");
		printf("----------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%9s|%10s|\n", sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));

	return;
}

// TYPE 5
void type5() {
	int k;
	int state = 0;
	int year;

	// 5
	printf("---------------- TYPE 5 ----------------\n");
	printf(" ** Find the top k brands by unit sales by the year. **\n");
	printf("  Enter k : ");
	scanf("%d", &k);
	printf("  Enter the Year (YYYY): ");
	scanf("%d", &year);
	sprintf(query, "SELECT model.model_type, count(sells_vehicle.VIN) as sold FROM vehicle JOIN model ON vehicle.model_ID = model.model_ID JOIN sells_vehicle ON vehicle.VIN = sells_vehicle.VIN WHERE YEAR(sells_vehicle.date) = %d GROUP BY model.model_type ORDER BY count(sells_vehicle.VIN) DESC LIMIT %d; ", year, k);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%9s|%10s|\n", "DealerID", "totalSold");
		printf("----------------------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%9s|%10s|\n", sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));

	return;
}

// TYPE 6
void type6() {
	int state = 0;

	// 6
	printf("---------------- TYPE 6 ----------------\n");
	printf(" ** In what month(s) do convertibles sell best? **\n");
	sprintf(query, "SELECT MONTH(date) AS Month FROM sells_vehicle GROUP BY MONTH(date) HAVING COUNT(MONTH(date)) = (SELECT MAX(monthcount) FROM(SELECT MONTH(date) AS mon, COUNT(MONTH(date)) AS monthcount FROM sells_vehicle GROUP BY MONTH(date)) AS monthdata); ");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%6s|\n", "Month");
		printf("--------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%6s|\n", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));
	return;
}

// TYPE 7
void type7() {
	int state = 0;

	// 7
	printf("---------------- TYPE 7 ----------------\n");
	printf(" ** Find those dealers who keep a vehicle in inventory for the longest average time. **\n");
	sprintf(query, "SELECT sells_vehicle.dealer_ID as dealer_ID FROM sells_vehicle JOIN vehicle ON sells_vehicle.VIN = vehicle.VIN JOIN makes_vehicle ON vehicle.VIN = makes_vehicle.VIN GROUP BY sells_vehicle.dealer_ID HAVING AVG(DATEDIFF(sells_vehicle.date, makes_vehicle.date)) = (SELECT MAX(storetime) FROM(SELECT sells_vehicle.dealer_ID, AVG(DATEDIFF(sells_vehicle.date, makes_vehicle.date)) AS storetime FROM sells_vehicle JOIN vehicle ON sells_vehicle.VIN = vehicle.VIN JOIN makes_vehicle ON vehicle.VIN = makes_vehicle.VIN GROUP BY sells_vehicle.dealer_ID) AS storedata); ");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("|%8s|\n", "DealerID");
		printf("----------\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("|%8s|\n", sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	printf("\n\n");
	memset(query, '\0', sizeof(query));
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////