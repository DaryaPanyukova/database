#include <gtest/gtest.h>
#include <lib/database.h>

// --------------- Select -----------------

TEST(TestSelect, All) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");


    auto t = a.Request("SELECT * FROM Customers;");
    ASSERT_TRUE(t == t);
}

TEST(TestSelect, Column) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");
    auto lhs = a.Request("SELECT CustomerName, k FROM Customers;");

    auto rhs = a.Request("CREATE TABLE Customers ("
                         "CustomerName VARCHAR,"
                         "k DOUBLE);");

    ASSERT_TRUE(lhs == rhs);
}

TEST(TestSelect, Where) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('aboba', 400, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('bruh', 400, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('lil peep', 400, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('lil nasx', 400, false);");

    auto lhs = a.Request("SELECT * FROM Customers WHERE flag = true;");

    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    auto rhs = a.Request(
            "INSERT INTO Customers (CustomerName, PostalCode, flag) "
            " VALUES ('aboba', 400, true);"
            "INSERT INTO Customers (CustomerName, PostalCode, flag) "
            " VALUES ('bruh', 400, true);"
            "INSERT INTO Customers (CustomerName, PostalCode, flag)"
            " VALUES ('lil peep', 400, true);");

    ASSERT_TRUE(lhs == rhs);
}

TEST(TestSelect, WhereExpression) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "     VALUES ('aboba', 400, true);"
              "INSERT INTO Customers (PostalCode, flag) "
              "     VALUES (400, true);"
              "INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "     VALUES ('lil peep', 400, true);"
              "INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "     VALUES ('lil nasx', 400, false);");

    auto lhs = a.Request("SELECT * FROM Customers WHERE flag=true AND"
                         " CustomerName IS NOT NULL;");


    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    auto rhs = a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
                         " VALUES ('aboba', 400, true);"
                         "INSERT INTO Customers (CustomerName, PostalCode, flag) "
                         " VALUES ('lil peep', 400, true);");

    ASSERT_TRUE(lhs == rhs);
}

TEST(TestSelect, Join) {
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

    auto lhs = a.Request("SELECT Orders.OrderID, Customers.CustomerName, "
                         "Orders.Num FROM Orders INNER JOIN Customers "
                         "ON Orders.CustomerID=Customers.CustomerID;");



    a.Request("CREATE TABLE Other (\n"
              "    OrderID INT,\n"
              "    CustomerName VARCHAR,\n"
              "    Num INT);");

    auto rhs = a.Request("INSERT INTO Other (OrderID, CustomerName, Num) "
                         "VALUES (10308, 'Ana', 18);");

    ASSERT_TRUE(lhs == rhs);
}


// --------------- Create -----------------

TEST(TestCreate, All) {
    Database a;
    a.Request("CREATE TABLE Persons (\n"
              "    PersonID INT,\n"
              "    k DOUBLE,\n"
              "    Address VARCHAR,\n"
              "    Dead BOOL);");
    ASSERT_TRUE(a.isCorrect());
}

TEST(TestCreate, Constraints) {
    Database a;
    a.Request("CREATE TABLE Persons (\n"
              "    ID INT PRIMARY KEY, \n"
              "    LastName VARCHAR NOT NULL UNIQUE,\n"
              "    FirstName VARCHAR UNIQUE DEFAULT 'a',\n"
              "    Age DOUBLE DEFAULT 5.2)");
    ASSERT_TRUE(a.isCorrect());
}

TEST(TestCreate, ForeignKey) {
    Database a;
    a.Request("CREATE TABLE Persons (\n"
              "    ID INT PRIMARY KEY, \n"
              "  FOREIGN KEY (PersonID) REFERENCES Persons(PersonID), \n"
              "  LastName DOUBLE NOT NULL,\n"
              "    FirstName VARCHAR UNIQUE,\n"
              "    Age INT DEFAULT 0,"
              "     FOREIGN KEY (PersonID) REFERENCES Persons(PersonID));");
    ASSERT_TRUE(a.isCorrect());
}



// --------------- Update -----------------

TEST(TestUpdate, Simple) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Cardinal', 400, 4.2, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Bruh', 200, 5.1, false);");

    a.Request("UPDATE Customers SET CustomerName = 'Alfred Schmidt', k = 0.4;");
    ASSERT_TRUE(a.isCorrect());
}


TEST(TestUpdate, Expression) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Cardinal', 400, 4.2, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Bruh', 200, 5.1, false);");

    a.Request(
            "UPDATE Customers SET CustomerName = 'Alfred Schmidt', k = 0.1 * PostalCode;");
    ASSERT_TRUE(a.isCorrect());
}


TEST(TestUpdate, Where) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Cardinal', 400, 4.2, false);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Bruh', 200, 5.1, false);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Brr', 350, 5.4, false);");

    a.Request("UPDATE Customers\n"
              "    SET flag = true, WHERE k > 5 AND PostalCode > 300;");
    ASSERT_TRUE(a.isCorrect());
}

// --------------- Insert  -----------------

TEST(TestInsert, Simple) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, k, flag) "
              "VALUES ('Cardinal', 400, 4.2, true);");
    ASSERT_TRUE(a.isCorrect());
}


TEST(TestInsert, NotNull) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT NOT NULL, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, k, flag) "
              "VALUES ('Cardinal', 4.2, true);");
    ASSERT_FALSE(a.isCorrect());
}


TEST(TestInsert, Default) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT NOT NULL DEFAULT 100, "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, k, flag) "
              "VALUES ('Cardinal', 4.2, true);");
    ASSERT_TRUE(a.isCorrect());
}


TEST(TestInsert, Unique) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT UNIQUE "
              "k DOUBLE, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, k, PostalCode, flag) "
              "VALUES ('Cardinal', 4.2, 5, true);");

    a.Request("INSERT INTO Customers (CustomerName, k, PostalCode, flag) "
              "VALUES ('Cardinal', 4.2, 5, true);");

    ASSERT_FALSE(a.isCorrect());
}

// --------------- Delete -----------------
TEST(TestDelete, Simple) {
    Database a;
    a.Request("CREATE TABLE Customers ("
              "CustomerName VARCHAR,"
              "PostalCode INT, "
              "flag BOOL);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('aboba', 400, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('bruh', 400, true);");

    a.Request("INSERT INTO Customers (CustomerName, PostalCode, flag) "
              "VALUES ('lil peep', 400, true);");


    a.Request("DELETE FROM Customers WHERE CustomerName='aboba';");
    ASSERT_TRUE(a.isCorrect());
}

// --------------- Drop -----------------
TEST(TestDrop, Simple) {
    Database a;
    a.Request("CREATE TABLE Shippers(key INT);");
    a.Request("DROP TABLE Shippers;");
    ASSERT_TRUE(a.isCorrect());
}

