#!/bin/bash

# Vérifie si -q est activé

QUIET=false

usage="

$(basename "$0") [-h] [-q] [Dossier de tests (01, 02, ..., 18)] -- ce programme compile et exécute le code du fichier exo3.c sur chacun des tests précisés en argument.
Pour chaque test, il est expliqué si le test a réussi ou non. Dans ce cas, il explique pourquoi. Un résumé est donné en sortie.

Options :
    -h montre ce message
    -q mode silencieux. Seul le résumé est affiché."

parsed_options=$(
  getopt -n "$0" -o qh -- "$@"
) || exit
eval "set -- $parsed_options"

while [ "$#" -gt 0 ]; do
  case $1 in
    (-[q]) QUIET=true;shift;;
    (-[h]) echo "$usage"; exit 0;;
    (--) shift; break;;
    (*) exit 1
  esac
done

# Si on lance tous les tests, alors le test est nécessairement silencieux.
if [[ "$#" == 0 ]]
then
  QUIET=true
fi


function prt {
  # Fonction echo sauf si -q est activé
  if ! $QUIET
  then
    echo "$1"
  fi
}

# Dossier du script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Vérifie si le code est bien dans le même dossier.
if [ ! -f $DIR/exo3.c ]; then
    echo "Le fichier $DIR/exo3.c n'existe pas ou n'est pas dans le même dossier que ce script.!"
    exit 2
fi

# Vérifie si les tests demandés existent
if [[ "$#" -eq 0 ]]
then
  s=$(seq -f "%02g" 1 18) # Si aucun test n'est demandé, alors tous les tests sont lancés
else
  s="$*"
fi

for i in $s 
do
if [ ! -f $DIR/tests/exo3/$i/input ]; then
    echo "Le dossier de test $DIR/tests/exo3/$i précisé en argument n'existe pas ou ne contient pas de fichier nommé input!"
    exit 2
fi
done

# Compile le code
prt "Compilation"
if ! $QUIET
then
  make -C $DIR -f $DIR/Makefile exo3
else
  make -C $DIR -f $DIR/Makefile -s exo3
fi
# Si la compilation ne passe pas, fin
if [ $?  != 0 ]
then
  echo "Problème de compilation"
  make -C $DIR -f $DIR/Makefile -s clean
  exit 1
fi


ok=0 # Nombre de tests réussis
nb=0 # Nombre de tests effectués
notsucceed=() # Noms des tests échoués

# On effectue chaque test
for i in $s 
do
  
  ((nb++)) # Un test de plus

  prt "=== TEST ${i} ==="

  instancefile=$DIR/tests/exo3/$i/input

  # On affiche l'instance
  prt "=== Instance : "
  prt "$(cat $instancefile)"
  prt

  # On affiche la solution
  prt "Solution renvoyée : "
  
  # Permet de conserver le code de retour avant les pipes
  set -o pipefail
  if $QUIET
  then
    # Mode -q : Lance le code exo3, s'arrête au bout de 3s max et tout ce qui est envoyé dans stdout est transmis à la variable $solution.
    solution=$(timeout 3s $DIR/exo3 < $instancefile)
  else
    # Lance le code exo3, s'arrête au bout de 3s max et tout ce qui est envoyé dans stdout est à la fois transmis à la console et dans la variable $solution.
    solution=$(timeout 3s $DIR/exo3 < $instancefile | tee /dev/tty)
  fi
  
  # Code de retour
  rc=$?
  set +o pipefail
  if [ "$rc" = 124 ] # Timeout
  then
    prt "TIMEOUT : votre fonction prend trop de temps."
  elif [ "$rc" = 139 ] # Segfault
  then
    prt "Erreur de segmentation"
  else
    prt

    # Sinon tout va bien, on vérifie la solution.

    solution="$(echo "${solution}" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"


    if $QUIET
    then
      # On vérifie la solution silencieusment
      echo "$solution" | python3 $DIR/check_solution.py -q $instancefile
      res=$? # Code de retour de la vérification
    else
      # On vérifie l solution silencieusmenet si l'utilisateur le demande.
      read -p "Animer votre solution ? (Y/n) " -n 1 -r
      echo 
      if [[ $REPLY == [yY] ]] || [[ $REPLY == '' ]]
      then
        # On vérifie la solution non silencieusement
        echo "$solution" | python3 $DIR/check_solution.py $instancefile
        res=$? # Code de retour de la vérification
      else
      # On vérifie la solution silencieusment
        echo "$solution" | python3 $DIR/check_solution.py -q $instancefile
        res=$? # Code de retour de la vérification
      fi
    fi

    # Si le code de retour est 0, c'est que le test est réussi.
    if [ $res = 0 ]
    then 
      ((ok++))
      
      if [[ "$#" -ne 1 ]] # S'il n'y a qu'un test, on affiche toujours si le test est réussi. Sinon on affiche que si -q n'est pas activé.
      then 
        prt
        prt "TEST REUSSI"
      else
        echo
        echo "TEST REUSSI"
      fi
    else
      notsucceed+=($i)
      if [[ "$#" -ne 1 ]] # S'il n'y a qu'un test, on affiche toujours si le test a échoué. Sinon on affiche que si -q n'est pas activé.
      then 
        prt
        prt "TEST ÉCHOUÉ"
      else
        echo
        echo "TEST ÉCHOUÉ"
      fi
    fi 

  fi

done


# On affiche un résumé de la situation.
if [[ "$#" -ne 1 ]]
then
  echo
  echo "TESTS REUSSIS : ${ok}"
  echo "NB TESTS : ${nb}"
  if [[ $ok != $nb ]]
  then
    echo "TESTS ECHOUES : ${notsucceed[*]}"
  fi
fi
make -C $DIR -f $DIR/Makefile -s clean
exit 0
