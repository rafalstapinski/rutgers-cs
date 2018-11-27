from config import credentials
import mysql.connector


class Resource:
    def connect_db(self):
        return mysql.connector.connect(
            user=credentials.db_user,
            password=credentials.db_pass,
            host=credentials.db_host,
            database=credentials.db_name,
        )
