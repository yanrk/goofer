/********************************************************
 * Description : sqlite db class which base on sqlite3
 * Date        : 2018-04-27 14:17:00
 * Author      : yanrk
 * Email       : yanrkchina@163.com
 * Blog        : blog.csdn.net/cxxmaker
 * Version     : 1.0
 * History     :
 * Copyright(C): 2013 - 2020
 ********************************************************/

#include <utility>
#include "log/log.h"
#include "filesystem/file.h"
#include "filesystem/directory.h"
#include "database/sqlite.h"
#include "sqlite3.h"

NAMESPACE_GOOFER_BEGIN

SQLiteDB::SQLiteDB()
    : m_path()
    , m_sqlite(nullptr)
{

}

SQLiteDB::~SQLiteDB()
{
    close();
}

bool SQLiteDB::create(const char * path, bool overwrite)
{
    if (nullptr == path)
    {
        RUN_LOG_ERR("create sqlite failed while path is nullptr");
        return (false);
    }

    close();

    bool path_is_directory = false;
    if (goofer_path_is_directory(path, path_is_directory))
    {
        if (path_is_directory)
        {
            goofer_remove_directory_recursive(path);
            if (goofer_access_safe(path))
            {
                RUN_LOG_ERR("create sqlite failed while remove directory (%s) failed", path);
                return (false);
            }
        }
        else
        {
            if (overwrite && !goofer_unlink_safe(path))
            {
                RUN_LOG_ERR("create sqlite failed while unlink file (%s) failed", path);
                return (false);
            }
        }
    }
    else
    {
        const std::string dirname(goofer_extract_directory(path));
        goofer_create_directory_recursive(dirname);
        if (!goofer_access_safe(dirname.c_str()))
        {
            RUN_LOG_ERR("create sqlite failed while create directory (%s) failed", dirname.c_str());
            return (false);
        }
    }

    m_path = path;

    int result = sqlite3_open(path, &m_sqlite);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("create sqlite failed while sqlite open (%s) failed, error (%d: %s)", path, result, sqlite3_errstr(result));
        close();
        return (false);
    }

    return (true);
}

bool SQLiteDB::open(const char * path)
{
    if (nullptr == path)
    {
        RUN_LOG_ERR("open sqlite failed while path is nullptr");
        return (false);
    }

    close();

    if (!goofer_access_safe(path))
    {
        RUN_LOG_ERR("open sqlite failed while path (%s) is not exist", path);
        return (false);
    }

    m_path = path;

    int result = sqlite3_open(path, &m_sqlite);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("open sqlite failed while sqlite open (%s) failed, error (%d: %s)", path, result, sqlite3_errstr(result));
        close();
        return (false);
    }

    return (true);
}

bool SQLiteDB::close()
{
    int result = SQLITE_OK;
    if (nullptr != m_sqlite)
    {
        int result = sqlite3_close(m_sqlite);
        if (SQLITE_OK != result)
        {
            RUN_LOG_ERR("close sqlite (%s) failed, error (%d: %s)", m_path.c_str(), result, sqlite3_errstr(result));
        }
        m_sqlite = nullptr;
        m_path.clear();
    }
    return (SQLITE_OK == result);
}

bool SQLiteDB::is_open() const
{
    return (nullptr != m_sqlite);
}

int SQLiteDB::error() const
{
    return (nullptr != m_sqlite ? sqlite3_errcode(m_sqlite) : 999);
}

const char * SQLiteDB::what() const
{
    return (nullptr != m_sqlite ? sqlite3_errmsg(m_sqlite) : "sqlite is closed");
}

bool SQLiteDB::execute(const char * operate, const char * sql)
{
    if (nullptr == operate)
    {
        operate = "execute sql";
    }

    if (nullptr == sql)
    {
        RUN_LOG_ERR("%s failed while sql is nullptr", operate);
        return (false);
    }

    if (nullptr == m_sqlite)
    {
        RUN_LOG_ERR("%s failed while sqlite is closed", operate);
        return (false);
    }

    char * error_message = nullptr;
    int result = sqlite3_exec(m_sqlite, sql, nullptr, nullptr, &error_message);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("%s failed while sqlite exec (%s) failed, error (%d: %s) message (%s)", operate, sql, result, sqlite3_errstr(result), nullptr != error_message ? error_message : "unknown");
    }
    if (nullptr != error_message)
    {
        sqlite3_free(error_message);
        error_message = nullptr;
    }

    return (SQLITE_OK == result);
}

bool SQLiteDB::begin_transaction()
{
    return (execute("begin transaction", "BEGIN TRANSACTION;"));
}

bool SQLiteDB::end_transaction()
{
    return (execute("end transaction", "END TRANSACTION;"));
}

SQLiteReader SQLiteDB::create_reader(const char * sql)
{
    return (SQLiteReader(m_sqlite, sql));
}

SQLiteWriter SQLiteDB::create_writer(const char * sql)
{
    return (SQLiteWriter(m_sqlite, sql));
}

SQLiteStatement::SQLiteStatement()
    : m_sql()
    , m_sqlite(nullptr)
    , m_statement(nullptr)
    , m_field_index(0)
    , m_column_index(0)
{

}

SQLiteStatement::SQLiteStatement(sqlite3 * sqlite, const char * sql)
    : m_sql()
    , m_sqlite(nullptr)
    , m_statement(nullptr)
    , m_field_index(0)
    , m_column_index(0)
{
    if (nullptr == sqlite)
    {
        RUN_LOG_ERR("create statement failed while sqlite is nullptr");
    }
    else if (nullptr == sql)
    {
        RUN_LOG_ERR("create statement failed while sql is nullptr");
    }
    else
    {
        m_sql = sql;
        m_sqlite = sqlite;
        int result = sqlite3_prepare_v2(m_sqlite, m_sql.c_str(), static_cast<int>(m_sql.size()), &m_statement, nullptr);
        if (SQLITE_OK != result)
        {
            RUN_LOG_ERR("create statement failed while sqlite (%s) prepare failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
            m_sql.clear();
            m_sqlite = nullptr;
            m_statement = nullptr;
        }
    }
}

SQLiteStatement::SQLiteStatement(SQLiteStatement && other)
    : m_sql()
    , m_sqlite(nullptr)
    , m_statement(nullptr)
    , m_field_index(0)
    , m_column_index(0)
{
    std::swap(m_sql, other.m_sql);
    std::swap(m_sqlite, other.m_sqlite);
    std::swap(m_statement, other.m_statement);
    std::swap(m_field_index, other.m_field_index);
    std::swap(m_column_index, other.m_column_index);
}

SQLiteStatement & SQLiteStatement::operator = (SQLiteStatement && other)
{
    if (&other != this)
    {
        clear();
        std::swap(m_sql, other.m_sql);
        std::swap(m_sqlite, other.m_sqlite);
        std::swap(m_statement, other.m_statement);
        std::swap(m_field_index, other.m_field_index);
        std::swap(m_column_index, other.m_column_index);
    }
    return (*this);
}

SQLiteStatement::~SQLiteStatement()
{
    clear();
}

void SQLiteStatement::clear()
{
    if (nullptr != m_statement)
    {
        int result = sqlite3_finalize(m_statement);
        if (SQLITE_OK != result)
        {
            RUN_LOG_ERR("destroy statement failed while sqlite (%s) finalize failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
        }
        m_sql.clear();
        m_sqlite = nullptr;
        m_statement = nullptr;
        m_field_index = 0;
        m_column_index = 0;
    }
}

bool SQLiteStatement::good() const
{
    return (nullptr != m_statement);
}

bool SQLiteStatement::reset()
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("reset failed while statement is nullptr");
        return (false);
    }

    m_field_index = 0;

    int result = sqlite3_reset(m_statement);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("reset failed while sqlite (%s) reset failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
        return (false);
    }

    return (true);
}

bool SQLiteStatement::bind(int field_index, int field_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("bind failed while statement is nullptr");
        return (false);
    }

    int result = sqlite3_bind_int(m_statement, field_index + 1, field_value);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("bind failed while sqlite (%s) bind int failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
        return (false);
    }

    return (true);
}

bool SQLiteStatement::bind(int field_index, int64_t field_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("bind failed while statement is nullptr");
        return (false);
    }

    int result = sqlite3_bind_int64(m_statement, field_index + 1, field_value);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("bind failed while sqlite (%s) bind int64 failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
        return (false);
    }

    return (true);
}

bool SQLiteStatement::bind(int field_index, double field_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("bind failed while statement is nullptr");
        return (false);
    }

    int result = sqlite3_bind_double(m_statement, field_index + 1, field_value);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("bind failed while sqlite (%s) bind double failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
        return (false);
    }

    return (true);
}

bool SQLiteStatement::bind(int field_index, const std::string & field_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("bind failed while statement is nullptr");
        return (false);
    }

    int result = sqlite3_bind_text(m_statement, field_index + 1, field_value.c_str(), static_cast<int>(field_value.size()), SQLITE_TRANSIENT);
    if (SQLITE_OK != result)
    {
        RUN_LOG_ERR("bind failed while sqlite (%s) bind text failed, error (%d: %s)", m_sql.c_str(), result, sqlite3_errstr(result));
        return (false);
    }

    return (true);
}

bool SQLiteStatement::bind(int field_value)
{
    return (bind(m_field_index++, field_value));
}

bool SQLiteStatement::bind(int64_t field_value)
{
    return (bind(m_field_index++, field_value));
}

bool SQLiteStatement::bind(double field_value)
{
    return (bind(m_field_index++, field_value));
}

bool SQLiteStatement::bind(const std::string & field_value)
{
    return (bind(m_field_index++, field_value));
}

SQLiteReader::SQLiteReader()
    : SQLiteStatement()
{

}

SQLiteReader::SQLiteReader(sqlite3 * sqlite, const char * sql)
    : SQLiteStatement(sqlite, sql)
{

}

bool SQLiteReader::read()
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("read failed while statement is nullptr");
        return (false);
    }

    m_column_index = 0;

    int result = sqlite3_step(m_statement);
    if (SQLITE_ROW != result && SQLITE_DONE != result)
    {
        RUN_LOG_ERR("read failed while sqlite (%s) step failed, error (%d: %s, %d: %s)", m_sql.c_str(), result, sqlite3_errstr(result), sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
    }
    return (SQLITE_ROW == result);
}

bool SQLiteReader::column(int column_index, int & column_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("column failed while statement is nullptr");
        return (false);
    }

    if (column_index >= sqlite3_column_count(m_statement))
    {
        RUN_LOG_ERR("column failed while sqlite (%s) column index is overflow (%d >= %d)", m_sql.c_str(), column_index, sqlite3_column_count(m_statement));
        return (false);
    }

    column_value = sqlite3_column_int(m_statement, column_index);
    return (true);
}

bool SQLiteReader::column(int column_index, int64_t & column_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("column failed while statement is nullptr");
        return (false);
    }

    if (column_index >= sqlite3_column_count(m_statement))
    {
        RUN_LOG_ERR("column failed while sqlite (%s) column index is overflow (%d >= %d)", m_sql.c_str(), column_index, sqlite3_column_count(m_statement));
        return (false);
    }

    column_value = sqlite3_column_int64(m_statement, column_index);
    return (true);
}

bool SQLiteReader::column(int column_index, double & column_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("column failed while statement is nullptr");
        return (false);
    }

    if (column_index >= sqlite3_column_count(m_statement))
    {
        RUN_LOG_ERR("column failed while sqlite (%s) column index is overflow (%d >= %d)", m_sql.c_str(), column_index, sqlite3_column_count(m_statement));
        return (false);
    }

    column_value = sqlite3_column_double(m_statement, column_index);
    return (true);
}

bool SQLiteReader::column(int column_index, std::string & column_value)
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("column failed while statement is nullptr");
        return (false);
    }

    if (column_index >= sqlite3_column_count(m_statement))
    {
        RUN_LOG_ERR("column failed while sqlite (%s) column index is overflow (%d >= %d)", m_sql.c_str(), column_index, sqlite3_column_count(m_statement));
        return (false);
    }

    const char * column_text = reinterpret_cast<const char *>(sqlite3_column_text(m_statement, column_index));
    int column_size = sqlite3_column_bytes(m_statement, column_index);
    column_value.assign(column_text, column_size);
    return (true);
}

bool SQLiteReader::column(int & column_value)
{
    return (column(m_column_index++, column_value));
}

bool SQLiteReader::column(int64_t & column_value)
{
    return (column(m_column_index++, column_value));
}

bool SQLiteReader::column(double & column_value)
{
    return (column(m_column_index++, column_value));
}

bool SQLiteReader::column(std::string & column_value)
{
    return (column(m_column_index++, column_value));
}

SQLiteWriter::SQLiteWriter()
    : SQLiteStatement()
{

}

SQLiteWriter::SQLiteWriter(sqlite3 * sqlite, const char * sql)
    : SQLiteStatement(sqlite, sql)
{

}

bool SQLiteWriter::write()
{
    if (nullptr == m_statement)
    {
        RUN_LOG_ERR("write failed while statement is nullptr");
        return (false);
    }

    int result = sqlite3_step(m_statement);
    if (SQLITE_ERROR == result || SQLITE_MISUSE == result)
    {
        RUN_LOG_ERR("write failed while sqlite (%s) step failed, error (%d: %s, %d: %s)", m_sql.c_str(), result, sqlite3_errstr(result), sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
        return (false);
    }

    return (true);
}

NAMESPACE_GOOFER_END
