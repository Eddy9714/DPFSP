# CMakeList.txt: progetto CMake per DPFSP. Includere l'origine e definire
# qui la logica specifica del progetto.
#
cmake_minimum_required (VERSION 3.8)

# Aggiungere l'origine all'eseguibile di questo progetto.
add_executable (DPFSP "DPFSP.cpp" "DPFSP.h" "Istanza.h" "Istanza.cpp"     "Random.h"  "ADE.h" "Permutazione.h" "Permutazione.cpp"   "ADE_DEP_DPFSP.cpp" "ADE_DEP_DPFSP.h" "IndiciRandom.cpp" "IndiciRandom.h"   "PermutazioneST.cpp" "PermutazioneST.h" "PermutazioneT.cpp" "PermutazioneT.h" "PermutazioneI.h" "PermutazioneI.cpp" "Globali.cpp" "Globali.h")

# TODO: Aggiungere i test e, se necessario, installare le destinazioni.
