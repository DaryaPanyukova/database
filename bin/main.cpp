#include "lib/database.h"

int main() {
    Database a;

    a.Request("CREATE TABLE Orders (\n"
              "    OrderID INT,\n"
              "    CustomerID INT,\n"
              "    Num INT);");

    a.Request("INSERT INTO Orders (OrderID, CustomerID, Num) "
              "VALUES (10308, 2, 18);");
    a.Request("INSERT INTO Orders (OrderID, CustomerID, Num) "
              "VALUES (10309, 37, 19);");
    a.Request("INSERT INTO Orders (OrderID, CustomerID, Num) "
              "VALUES (10310, 77, 20);");


// --------------------------------------------
    a.Request("CREATE TABLE Customers (\n"
              "    CustomerID INT,\n"
              "    CustomerName VARCHAR,\n"
              "    Country VARCHAR);");

    a.Request("INSERT INTO Customers (CustomerID, CustomerName, Country) "
              "VALUES (1, 'Alfreds', 'Germany');");
    a.Request("INSERT INTO Customers (CustomerID, CustomerName, Country) "
              "VALUES (2, 'Ana', 'Mexico');");
    a.Request("INSERT INTO Customers (CustomerID, CustomerName, Country) "
              "VALUES (3, 'Antonio', 'Mexico');");


// -------------------------------------------
   auto lhs = a.Request("SELECT Orders.OrderID, Customers.CustomerName, "
              "Orders.Num FROM Orders INNER JOIN Customers "
              "ON Orders.CustomerID=Customers.CustomerID;");

    std::cout << lhs;
    std::string filename = "D:\\ITMOlabs\\basics_c++\\tmp.txt";
    a.SaveToFile(lhs, filename);

    std::cout << "\n\n";

    a.ImportTable("table", filename);
    auto rhs = a.Request("SELECT * FROM table;");
    std::cout << rhs;
}
