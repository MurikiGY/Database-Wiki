# DATABASE-wiki
Lista de funcionalidades de um SGDB. (postgres em sua maioria)

- [Normalização](#normalização-de-tabelas)
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
- [Cluster em disco externo](#Cluster-em-disco-externo-postgres)

## Dependencias funcionais


## Normalização de tabelas
### Primeira forma normal

### Segunda forma normal

### Terceira forma normal

### Forma normal Boyce-Codd


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
``` sql
SELECT * FROM <tableA> tb_a where tb_a.tableA_id > 50;
```



## Transações
Uma transação representa uma unidade de trabalho executada dentro de um sistema de gerenciamento de banco de dados, sobre um banco de dados, e tratada de maneira coerente e confiável, independente de outras transações. Uma transação geralmente representa qualquer alteração em um banco de dados.
``` sql
BEGIN;       //inicia uma transação
COMMIT;      //registra as alterações realizadas
ROLLBACK;    //descarta transação
```



## Criação de tabelas

### Tabelas normais
``` sql
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
``` sql
CREATE TABLE <table> (
   <attribute_name>    <data_type>;
) INHERITS (parent_table);
//Além das colunas herdadas, será adicionado a coluna <attribute_name>
```

### Copia de tabelas
``` sql
select *
into <new_table_name>
from <table_to_be_copied>;
```



## Alteração de tabelas

### Adicionar coluna
``` sql
ALTER TABLE <table>
ADD <column> datatype;
```

### Remover coluna
``` sql
ALTER TABLE <table>
DROP COLUMN <column>;
```

### Renomear colunas
``` sql
ALTER TABLE <table>
RENAME COLUMN <column_name>
TO <new_column_name>;
```

### Alterar tipo
``` sql
ALTER TABLE <table>
ALTER COLUMN <column>
TYPE <data_type>;
```

### Permitir/Bloquear valores nulos
``` sql
ALTER TABLE <table> ALTER COLUMN <column_name> SET NOT NULL;
ALTER TABLE <tbale> ALTER COLUMN <column_name> DROP NOT NULL;
```

### Referenciar chave primaria
``` sql
ALTER TABLE <Child_table>
ADD CONSTRAINT <constraint_name>
FOREIGN KEY (<fk_columns>)
REFERENCES <parent_table> (<parent_key_columns>);
```

### Alterar strings para lowercase
``` sql
UPDATE <table> SET <column_name> = lower(<column_name>);
```

### Inserir dados
``` sql
INSER INTO <table> VALUES (<data1, data2, data3>);
```

### Deletar dados
``` sql
DELETE FROM <table> WHERE <attribute>=<value>;
```

### Alterar horarios
``` sql
UPDATE <table> set <column> = <column> +-  interval ' <interval> '
```



## Consulta de dados

### Consulta simples
``` sql
SELECT <column_name1>, <column_name2>  FROM <table>;  //retorna todos os dados das colunas <column_name1> e <column_name2>
SELECT *  FROM <table>;                               //retorna todos os dados da tabela
```

### Consulta com parametros
``` sql
...WHERE <condiction>...;                //retorna dados com base na condição passada
...ORDER BY <column_name> [DESC | ASC];  //retorna dados ordenados em DESC ou ASC
...DISTINCT <column_name>...;            //retorna dados sem repetições
```

### Consulta de repetições
``` sql
SELECT <column_name>
FROM <table>
GROUP BY <column_name>
HAVING COUNT (<column_name) > 1;
```



## Junção de tabelas
Existem várias formas de juntar duas tabelas, abaixo as duas mais comuns:

### Inner join
``` sql
//tb_1.key e tb_2.key são as chaves primarias de cada tabela
SELECT <select_list> FROM <table1> tb_1 INNER JOIN <table2> tb_2 on tb_1.key = tb_2.key;
```

### Left Join
``` sql
SELECT <select_list> FROM <table1> tb_1 LEFT JOIN <table2> tb_2 on tb_1.key = tb_2.key;
```



## Importação/Exportação de tabelas em csv

### Importar
``` sql
\COPY <table(columns_a, column_b)> FROM '/path' DELIMITER ';' CSV;
```

### Exportar
``` sql
\COPY (<query>) TO '/path/file.csv' DELIMITER ';' CSV;
```



## Backup por dump


## Tamanho do Banco

### Postrgresql
Listar tamanho dos bancos do cluster
``` sql
SELECT pg_database.datname, pg_size_pretty(pg_database_size(pg_database.datname)) AS size FROM pg_database;
```
Listar o tamanho das tabelas de um banco
``` sql
# Acesse o banco
\c <database_name>
\d+
```

## Cluster em disco externo postgres

### Montagem de um disco externo
Ao ser conectado um disco ao computador é preciso montá-lo em um diretório do sistema de arquivos.
Considerando a situação onde há um ssd com o sistema operacional instalado e um HD com uma partição única configurada e devidamente formatada, verifique com o comando `lsblk` o esquema de partições.\
Sendo /dev/sda o ssd onde há o sistema operacional e /dev/sdb o disco externo (sendo /dev/sdb1 a partição) é possível montar em um diretório o disco com o comando `mount <partição> <diretório>`.\
Caso se deseje montar o disco na inicialização do computador é possível alterar o arquivo `/etc/fstab`.

Exemplo da montagem da partição /dev/sdb1 no diretório /HDD/space1
```
# Static information about the filesystems.
# See fstab(5) for details.

# <file system> <dir> <type> <options> <dump> <pass>
# /dev/sda2
UUID=<some_numbers>	    /         	  ext4      	rw,relatime	 0 1

# /dev/sda3
UUID=<some_numbers>	    /home     	  ext4      	rw,relatime	 0 2

# /dev/sda1
UUID=<some_numbers>	     none      	swap      	 defaults  	  0 0

# Adicione aqui a montagem de novas partições
# /dev/sdb1
/dev/sdb1					                      /HDD/space1  ext4		   defaults	    0 3
```

### Criação e inicialização do cluster do postgres
Para trabalhar com o postgres em um disco externo, primeiramente garanta que NÂO há outro cluster do postgres rodando junto: `systemctl status postgresql.service`.

Mude para o usuário postgres: `su postgres`.\
Crie um diretório onde será armazenado o cluster: `mkdir /HDD/space1/postgres/`.\
Crie o cluster: `initdb -D /HDD/space1/postgres/`.\
Inicie o cluster: `pg_ctl -D /HDD/space1/postgres/ -l arquivolog start`.

Caso ocorra algum erro, verifique o arquivo de log `arquivolog` gerado.

Possívelmente ocorrerão erros como:
```
FATAL:  could not create lock file "/run/postgresql/.s.PGSQL.5432.lock": Arquivo ou diretório inexistente
```
Para casos assim, verifique se o diretório `/run/postgresql/` existe e pertence ao usuário postgres.

Por fim, acesse o cluster com `psql` e verifique o local dos dados com `SHOW data_directory`.

Caso queira reiniciar o cluster: `pg_ctl -D /HDD/space1/postgres/ -l arquivolog restart`


## Bibliografia
- https://database.guide/add-hours-to-a-time-value-in-postgresql/
