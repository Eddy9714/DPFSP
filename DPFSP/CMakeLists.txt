# CMakeList.txt: progetto CMake per DPFSP. Includere l'origine e definire
# qui la logica specifica del progetto.
#
cmake_minimum_required (VERSION 3.8)

# Aggiungere l'origine all'eseguibile di questo progetto.
add_executable (DPFSP "DPFSP.cpp" "DPFSP.h" "Istanza.h" "Istanza.cpp" "Individuo.h" "Individuo.cpp" "ADE.cpp" "ADE.h" "Random.h" "ADE_DEP.cpp" "ADE_DEP.h" "Permutazione.h" "Permutazione.cpp"  )

# TODO: Aggiungere i test e, se necessario, installare le destinazioni.
