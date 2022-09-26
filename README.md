# SQL-wiki
Lista de comandos sql para postgresql.

# Criação de tabelas

- Tabelas normais
```
CREATE TABLE <table> (
   <table_id>          serial          PRIMARY KEY,
   <attribute_int>     integer         NOT NULL,
   <attribute_boolean> boolean         NOT NULL,
   <attribute_char>    varchar(<size>) NOT NULL,
   <attribute_locale>  point           NOT NULL,
   <attribute_time>    time            NOT NULL,
   <attribute_date>    date            NOT NULL
);
```

- Tabelas herdadas
```
CREATE TABLE <table> (
   <attribute_name>    <data_type>;
) INHERITS (parent_table);
//Além das colunas herdadas, será adicionado a coluna <attribute_name>
```


# Alteração de tabelas

- Adicionar coluna
```
ALTER TABLE <table>
ADD <column> datatype;
```

- Remover coluna
```
ALTER TABLE <table>
DROP COLUMN <column>;
```

- Renomear colunas
```
ALTER TABLE <table>
RENAME COLUMN column_name
TO new_column_name;
```

- Alterar tipo
```
ALTER TABLE <table>
ALTER COLUMN <column>
TYPE <data_type>;
```

- Referenciar chave primaria
```
ALTER TABLE <Child_table>
ADD CONSTRAINT <constraint_name>
FOREIGN KEY (<fk_columns>)
REFERENCES <parent_table> (<parent_key_columns>);
```

- Inserir dados
```
INSER INTO <table> VALUES (<data1, data2, data3>);
```

- Deletar dados
```
DELETE FROM <Table> WHERE <attribute>=<value>;
```

# Importação/Exportação de tabelas em csv

- Importar
```
\COPY <table(columns_a, column_b)> FROM '/path' DELIMITER ';' CSV;
```

- Exportar
```
\COPY (<query>) TO '/path/file.csv' DELIMITER ';' CSV;
```

# Consulta de dados

- Consulta simples
```
SELECT <column_name> FROM <table>
```
- Complementares de consulta
```
WHERE <condiction> //retorna dadosa com base na condição passada
ORDER BY <column_name> [DESC | ASC] //retorna dados ordenados em DESC ou ASC

```


