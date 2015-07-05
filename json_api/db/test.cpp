#include <sqlite/connection.hpp>
#include <sqlite/execute.hpp>
#include <sqlite/query.hpp>
#include <iostream>

int main() {
  try {

    sqlite::connection con("test.db");
    sqlite::execute(con, "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);", true);
    sqlite::execute ins(con, "INSERT INTO test VALUES(?, ?);");

    ins % sqlite::nil % "Hello";
    ins();

    sqlite::query q(con, "SELECT * FROM test;");
    
    boost::shared_ptr<sqlite::result> result = q.get_result();
    while(result->next_row()) {
      std::cout << "ID:   " << result->get_int(0) << "\n"
                << "Name: " << result->get_string(1) << std::endl;
    }

  }
  catch(std::exception const & e) {
    std::cout << "An error occurred: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
