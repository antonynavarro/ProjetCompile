#!/bin/bash

# Répertoire principal
TEST_DIR="test"

# Variables de suivi global
total_pass=0
total_tests=0
good_pass=0
good_tests=0
syn_err_pass=0
syn_err_tests=0
sem_err_pass=0
sem_err_tests=0
warn_pass=0
warn_tests=0

# Assoc pour stocker les stats par dossier
declare -A pass_counts
declare -A total_counts

# Fonction pour déterminer le code attendu selon le nom du dossier
get_expected_code() {
    case "$1" in
        good) echo 0 ;;
        syn-err) echo 1 ;;
        sem-err) echo 2 ;;
        warn) echo 0 ;;
        *) echo 99 ;;  # Inconnu
    esac
}

# Fonction pour tester un répertoire
test_directory() {
    local dir_path=$1
    local dir_name
    dir_name=$(basename "$dir_path")
    local expected_status
    expected_status=$(get_expected_code "$dir_name")

    echo ""
    echo "=== TESTS DANS $dir_name ($dir_path) ==="
    local pass=0
    local total=0

    for file in "$dir_path"/*.tpc; do
        [[ -f "$file" ]] || continue
        ((total++))
        echo -n "Test $file : "

        ./bin/tpcc < "$file" > /dev/null 2>&1
        status=$?

        if [[ $status -eq $expected_status ]]; then
            echo "[OK]"
            ((pass++))
        else
            echo "[FAIL] (Code retour : $status, attendu : $expected_status)"
        fi
    done

    pass_counts["$dir_name"]=$pass
    total_counts["$dir_name"]=$total

    ((total_pass+=pass))
    ((total_tests+=total))

    echo "==> Résultat $dir_name : $pass / $total"
}

# Parcourir tous les sous-dossiers de TEST_DIR
for dir in "$TEST_DIR"/*; do
    [[ -d "$dir" ]] || continue
    test_directory "$dir"
done

# Récap global
echo ""
echo "=== RÉCAPITULATIF GLOBAL ==="

for key in good syn-err sem-err warn; do
    pass=${pass_counts[$key]:-0}
    total=${total_counts[$key]:-0}
    printf "Tests %-12s : %d / %d\n" "$key" "$pass" "$total"
done

echo "Score total        : $total_pass / $total_tests"