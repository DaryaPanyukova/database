add_library(Database
        database.h database.cpp
        sqlparser.h sqlparser.cpp
        table.h table.cpp
        rpn.h rpn.cpp
        create_query.h create_query.cpp
        query.h
        drop_query.h drop_query.cpp
        insert_query.h insert_query.cpp
        delete_query.h delete_query.cpp
        update_query.h update_query.cpp
        select_query.h select_query.cpp)