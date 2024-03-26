# Реляционная БД с поддержкой SQL запросов

Реляционная база данных в памяти с поддержкой упрощенных SQL запросов. Имеет возможностью сохранения и загрузки текущего состояния БД в файл.

## Парсинг
Для реализации парсинга запросов был использован рекурсивный спуск. Файл [BNF.txt](https://github.com/DaryaPanyukova/database/blob/main/BNF.txt) содержит грамматику языка запросов SQL в форме [Бэкуса-Наура](https://ru.wikipedia.org/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D0%B0_%D0%91%D1%8D%D0%BA%D1%83%D1%81%D0%B0_%E2%80%94_%D0%9D%D0%B0%D1%83%D1%80%D0%B0)

### Поддерживаемый синтаксис

Ключевые слова:

- SELECT
- FROM
- WHERE
- (LEFT|RIGHT|INNER)JOIN
- CREATE TABLE
- DROP TABLE
- AND
- OR
- IS
- NOT
- NULL
- ON
- UPDATE
- INSERT
- VALUES
- DELETE
- PRIMARY KEY
- FOREIGN KEY

Поддерживаемые типы данных:

- bool
- int
- float
- double
- varchar


На данный момент не реализованы:

- вложенные подзапросы
- Join более чем для 2 таблиц
