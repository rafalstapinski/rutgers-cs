import mysql.connector
import sshtunnel
import config

with sshtunnel.SSHTunnelForwarder(
    (config.ssh_host, config.ssh_port),
    ssh_username=config.ssh_user,
    ssh_password=config.ssh_pass,
    remote_bind_address=(config.remote_bind_address, config.remote_bind_port),
    local_bind_address=(config.local_bind_address, config.local_bind_port),
) as tunnel:
    connection = mysql.connector.connect(
        user=config.db_user,
        password=config.db_pass,
        host=config.remote_bind_address,
        database=config.db_name,
        port=config.remote_bind_port,
    )

    cursor = connection.cursor()

    cursor.execute("SELECT * FROM testtable")

    for thing in cursor:
        print(thing)
