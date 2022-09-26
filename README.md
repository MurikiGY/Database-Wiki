# SQL-wiki

# Criação de tabelas
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

# Modificar tabelas

- Adicionar coluna
```
ALTER TABLE <Table>
ADD <column> datatype;
```

- Remover coluna
```
ALTER TABLE <Table>
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
ALTER TABLE <Table>
ALTER COLUMN <column>
[SET DEFAULT <value> | DROP DEFAULT];
```

- Referenciar chave primaria
```
ALTER TABLE <Child_table>
ADD CONSTRAINT <constraint_name>
FOREIGN KEY (<fk_columns>)
REFERENCES <parent_table> (<parent_key_columns>);
```

- Deletar tuplas
```
DELETE FROM <Table> where <attribute>=<value>;
```
