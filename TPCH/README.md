# TPCH BENCHMARK

Antes de tudo, descompacte e acesse o diretorio de trabalho
```
unzip TPC-H-Tool.zip
cd TPC-H\ V3.0.1/dbgen/
```

Para realizar um benchmark com tpch primeiro é necessario compilar o binario `dbgen`. Para isso altere os seguintes parametros do arquivo `makefile.suite`
```
# vim makefile.suite
CC       = gcc
DATABASE = ORACLE
MACHINE  = LINUX
WORKLOAD = TPCH
```

Faça a compilação
```
make -f makefile.suite
```

Finalmente com o binario `dbgen` construido, é possivel gerar os dados com:
```
./dbgen -s 1 -vf
```

Com isso os arquivos `<tabela>.tbl` serão criados com os dados a serem inseridos.

Finalmente, teremos a seguinte estrutura:
dss.ddl -> Arquivo com as Create Tables para criação das tabelas.
*.tbl   -> Arquivos com os dados das tabelas.
queries -> Diretório com as consultas.


