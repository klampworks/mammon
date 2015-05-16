#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/custom_query.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <sqlite3.h>

namespace sql = sqlpp::sqlite3;
int main()
{
    sql::connection_config config;
    config.path_to_database = "hello.sqlite3";
    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    config.debug = true;
	sql::connection db(config);
}
