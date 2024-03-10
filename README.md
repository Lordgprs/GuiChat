## Chat
#### Описание проекта
В данном проекте представлен многопользовательский чат с хранением информации о пользователях и истории сообщений в базе данных PostgreSQL
#### Структура проекта
##### Конфигурационные файлы: chat.conf. 
Формат:

 - DBHost=<database host\>
 - DBName=<database name\>
 - DBUser=<database user\>
 - DBPassword=<database password\>

##### SQL-схема

Перед началом работы необходимо импортировать схему базы данных:

    psql -U<db user> -h<db host> <dbname> < schema.sql

##### Клиентская часть
bin/client

Клиент чата. Позволяет пользователям регистрироваться и оставлять сообщения

##### Серверная часть
bin/server

Панель администратора. Позволяет удалять сообщения и пользователей, а также отключать пользователей.