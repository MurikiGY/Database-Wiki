#! /bin/bash

#set +x

FILE="testes/teste"
INI=1
FIM=10

OUT="output_valgrind"
DIFF="output_diff"

make        #Compilação

echo ""

for i in $(seq $INI $FIM)
do
  echo "resultado teste$i:"
  ./myht < $FILE$i.in > $FILE$i.sol             #Execução
  if $(diff $FILE$i.sol $FILE$i.out > $DIFF);   #Diff
  then
    echo "diff ok"
  else
    echo "esperado:"
    echo $(cat $FILE$i.out)
    echo "recebido:"
    echo $(cat $FILE$i.sol)
    cat $DIFF
  fi

  valgrind ./myht < $FILE$i.in > $OUT 2>&1      #Teste de Valgrind

  if $(grep -q "ERROR SUMMARY: 0 errors" $OUT);
  then
    #Nao teve erro, ve se teve vazamento
    if $(grep -q "All heap blocks were freed -- no leaks are possible" $OUT);
    then
      echo "valgrind ok, sem erros e sem vazamentos"
    else
      echo "vazamento:"
      tail $OUT
    fi
  
  else
    echo "erro no valgrind"
  fi
  echo ""

done

rm $OUT $DIFF

make purge
