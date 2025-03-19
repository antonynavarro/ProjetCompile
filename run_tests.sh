#!/bin/bash

# Répertoires des tests
TEST_DIR="test"
GOOD_TESTS="$TEST_DIR/good"
ERR_TESTS="$TEST_DIR/syn-err"

# Variables de suivi
good_pass=0
good_total=0
err_pass=0
err_total=0

# Test des programmes corrects
echo "=== TESTS DES PROGRAMMES CORRECTS ==="
for file in "$GOOD_TESTS"/*.tpc; do
    if [[ -f "$file" ]]; then
        ((good_total++))
        echo -n "Test $file : "
        
        ./bin/tpcas < "$file" > /dev/null 2>&1
        status=$?

        # Vérifier le code de retour attendu
        if [[ $status -eq 0 ]]; then
            echo "[OK]"
            ((good_pass++))
        else
            echo "[FAIL] (Code retour : $status)"
        fi
    fi
done

# Test des programmes incorrects
echo "=== TESTS DES PROGRAMMES INCORRECTS ==="
for file in "$ERR_TESTS"/*.tpc; do
    if [[ -f "$file" ]]; then
        ((err_total++))
        echo -n "Test $file : "
        
        ./bin/tpcas < "$file" > /dev/null 2>&1
        status=$?

        # Vérifier le code de retour attendu
        if [[ $status -eq 1 ]]; then
            echo "[OK]"
            ((err_pass++))
        else
            echo "[FAIL] (Code retour : $status)"
        fi
    fi
done

# Résumé
echo ""
echo "=== RÉCAPITULATIF ==="
echo "Tests corrects réussis : $good_pass / $good_total"
echo "Tests incorrects réussis : $err_pass / $err_total"
total_score=$((good_pass + err_pass))
total_tests=$((good_total + err_total))
echo "Score total : $total_score / $total_tests"
