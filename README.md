# SQL-wiki
Lista de comandos sql para postgresql.

- [Acesso ao banco](#acesso-ao-banco)
- [Alias de tabelas](#alias-de-tabelas)
- [Transações](#transações)
- [Criação de Tabelas](#criação-de-tabelas)
- [Alteração de Tabelas](#alteração-de-tabelas)
- [Consulta de Dados](#consulta-de-dados)
- [Junção de Tabelas](#Junção-de-tabelas)
- [Importação/Exportação em csv](#importaçãoexportação-de-tabelas-em-csv)
- [Backup por Dump](#backup-por-dump)
- [Tamanho do Banco](#tamanho-do-banco)

## Acesso ao banco
### Postgresql
```
sudo -i -u postgres         //alternar para o usuario postgres
psql                        //acessar o prompt do banco
sudo -u postgres psql       //acessar um prompt sem alternar de conta
psql -h <host> -U <user>    //conexão via ssh
```



## Alias de Tabelas
Quando colocado em frente a uma tabela uma string que não seja uma palavra reservada, será tratado como um nome alternativo para a tabela.
Imagine uma tabela \<tableA\> que possui um contador de id \<tableA\>.tableA_id.
```
SELECT * FROM <tableA> tb_a where tb_a.tableA_id > 50;
```



## Transações
Uma transação representa uma unidade de trabalho executada dentro de um sistema de gerenciamento de banco de dados, sobre um banco de dados, e tratada de maneira coerente e confiável, independente de outras transações. Uma transação geralmente representa qualquer alteração em um banco de dados.
```
BEGIN;       //inicia uma transação
COMMIT;      //registra as alterações realizadas
ROLLBACK;    //descarta transação
```



## Criação de tabelas

### Tabelas normais
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

### Tabelas herdadas
```
CREATE TABLE <table> (
   <attribute_name>    <data_type>;
) INHERITS (parent_table);
//Além das colunas herdadas, será adicionado a coluna <attribute_name>
```

### Copia de tabelas
```
select *
into <new_table_name>
from <table_to_be_copied>;
```



## Alteração de tabelas

### Adicionar coluna
```
ALTER TABLE <table>
ADD <column> datatype;
```

### Remover coluna
```
ALTER TABLE <table>
DROP COLUMN <column>;
```

### Renomear colunas
```
ALTER TABLE <table>
RENAME COLUMN <column_name>
TO <new_column_name>;
```

### Alterar tipo
```
ALTER TABLE <table>
ALTER COLUMN <column>
TYPE <data_type>;
```

### Permitir/Bloquear valores nulos
```
ALTER TABLE <table> ALTER COLUMN <column_name> SET NOT NULL;
ALTER TABLE <tbale> ALTER COLUMN <column_name> DROP NOT NULL;
```

### Referenciar chave primaria
```
ALTER TABLE <Child_table>
ADD CONSTRAINT <constraint_name>
FOREIGN KEY (<fk_columns>)
REFERENCES <parent_table> (<parent_key_columns>);
```

### Alterar strings para lowercase
```
UPDATE <table> SET <column_name> = lower(<column_name>);
```

### Inserir dados
```
INSER INTO <table> VALUES (<data1, data2, data3>);
```

### Deletar dados
```
DELETE FROM <table> WHERE <attribute>=<value>;
```



## Consulta de dados

### Consulta simples
```
SELECT <column_name1>, <column_name2>  FROM <table>;  //retorna todos os dados das colunas <column_name1> e <column_name2>
SELECT *  FROM <table>;                               //retorna todos os dados da tabela
```

### Consulta com parametros
```
...WHERE <condiction>...;                //retorna dados com base na condição passada
...ORDER BY <column_name> [DESC | ASC];  //retorna dados ordenados em DESC ou ASC
...DISTINCT <column_name>...;            //retorna dados sem repetições
```

### Consulta de repetições
```
SELECT <column_name>
FROM <table>
GROUP BY <column_name>
HAVING COUNT (<column_name) > 1;
```



## Junção de tabelas
Existem várias formas de juntar duas tabelas, abaixo as duas mais comuns:

### Inner join
```
//tb_1.key e tb_2.key são as chaves primarias de cada tabela
SELECT <select_list> FROM <table1> tb_1 INNER JOIN <table2> tb_2 on tb_1.key = tb_2.key;
```

### Left Join
```
SELECT <select_list> FROM <table1> tb_1 LEFT JOIN <table2> tb_2 on tb_1.key = tb_2.key;
```



## Importação/Exportação de tabelas em csv

### Importar
```
\COPY <table(columns_a, column_b)> FROM '/path' DELIMITER ';' CSV;
```

### Exportar
```
\COPY (<query>) TO '/path/file.csv' DELIMITER ';' CSV;
```



## Backup por dump


## Tamanho do Banco

### Postrgresql
```
SELECT pg_database.datname, pg_size_pretty(pg_database_size(pg_database.datname)) AS size FROM pg_database;
```
