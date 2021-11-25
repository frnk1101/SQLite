#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <map>
#include <iterator>
// #include <sqlite3.h>
#include "sqlite3.h"
#include <sstream>


class C_Database {
public:

  C_Database() {
    std::cout << "C_Database\n==========" << std::endl;
  };

  ~C_Database() {
  };

  t_bool_map Init();

  t_bool_map getGroup();
  t_bool_map setGroup();
  t_bool_map setItem();
  t_bool_map TestDB();


private:
  t_bool_map SQLiteExecute(t_string_string data_in);
  t_bool_map AddNewDayTable();
  t_bool_map CreateTable();


protected:

};

t_bool_map C_Database::Init() {
  try {



    // get all tables
    t_string_string arguments = {
      {"db_name", "base.db"},
      {"db_path", "./"},
      {"db_query", "SELECT type,name,sql,tbl_name,'0' AS temp FROM sqlite_master UNION SELECT type,name,sql,tbl_name,'1' AS temp FROM sqlite_temp_master"}
    };

    // "INSERT INTO 'items'('name','category','description','temperature','price','tax','quantity','featured') VALUES ('dessert','food','chocolate','cold',1.80,0,33,0)"; // "SELECT * FROM items";

    t_bool_map sql_return;
    sql_return = SQLiteExecute(arguments);
    t_bool_map out;
    if (sql_return.begin()->first == true) {
      out = sql_return;
    }
    if (sql_return.begin()->first == false) {
      t_string_string arguments_c = {
        {"db_name", "base.db"},
        {"db_path", "./"},
        {"db_query", "INSERT INTO 'items'('name','category','description','temperature','price','tax','quantity','featured') VALUES ('dessert','food','chocolate','cold',1.80,0,33,0)"}
      };
      sql_return = SQLiteExecute(arguments_c);

      out = {
        {true,
          {
            {"ROW",
              {
                {"DB created", ""}
              }}
          }}
      };
    }

    return out;
  } catch (const std::exception& e) {
    t_bool_map out = {
      {false,
        {
          {"ERROR",
            {
              {"TYPE", "EXCEPTION"}
            }}
        }}
    };
    return out;
  }
};

t_bool_map C_Database::SQLiteExecute(t_string_string data_in) {
  try {
    std::string DB_NAME = "";
    std::string DB_PATH = "./";
    std::string DB_QUERY = "";
    t_string_map db_return;

    for (t_string_string::iterator i_element = data_in.begin(); i_element != data_in.end(); i_element++) {
      if (i_element->first == "db_name") {
        DB_NAME = i_element->second;
      }
      if (i_element->first == "db_query") {
        DB_QUERY = i_element->second;
      }
      if (i_element->first == "db_path") {
        DB_PATH = i_element->second;
      }
    }

    if (DB_NAME != "" || DB_QUERY != "") {
      const char *db_name = DB_NAME.c_str();
      const char *db_query = DB_QUERY.c_str();
      const char *db_path = DB_PATH.c_str();

      sqlite3 *db;
      sqlite3_open(db_name, &db);
      sqlite3_stmt *stmt;
      int rc = sqlite3_prepare_v2(db, db_query, -1, &stmt, NULL);
      if (rc != SQLITE_OK) {
        std::cout << "SQL:: ERROR1" << std::endl;
      }
      int sum_columns = sqlite3_column_count(stmt);
      int i = 1;

      while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        t_string_string row;
        for (int i = 0; i < sum_columns; i++) {
          std::string key = "";
          std::string value = "";
          std::stringstream ss_key;
          ss_key << sqlite3_column_name(stmt, i);
          key = ss_key.str();
          std::stringstream ss_value;
          ss_value << sqlite3_column_text(stmt, i);
          value = ss_value.str();
          row.insert(std::pair<std::string, std::string>{key, value});
        }
        std::stringstream row_n;
        row_n << i;
        db_return.insert(std::pair<std::string, t_string_string>{"ROW " + row_n.str(), row});
        // map/insert result: pair<iterator,bool>
        i++;
      }

      if (rc != SQLITE_DONE) {
        std::cout << "SQL:: ERROR2" << std::endl;
      }
      sqlite3_finalize(stmt);
      sqlite3_close(db);
    }


    t_bool_map out = {
      {true,
        {
          db_return
        }}
    };
    return out;
  } catch (const std::exception& e) {
    t_bool_map out = {
      {false,
        {
          {"ERROR",
            {
              {"TYPE", "EXCEPTION"}
            }}
        }}
    };
    return out;
  }

};



t_bool_map C_Database::TestDB() {
  try {
    bool SUCCESS = false;
    t_string_string arguments = {
      {"db_name", "base.db"},
      {"db_path", "./"},
      {"db_query", "SELECT * FROM items"}
    };

    t_bool_map sql_return = SQLiteExecute(arguments);
    if (sql_return.begin()->first == true) {
      SUCCESS = true;
    }


    t_bool_map out = sql_return;

    return out;
  } catch (const std::exception& e) {
    t_bool_map out = {
      {false,
        {
          {"ERROR",
            {
              {"TYPE", "EXCEPTION"}
            }}
        }}
    };
    return out;
  }
};


#endif /* DATABASE_HPP */
