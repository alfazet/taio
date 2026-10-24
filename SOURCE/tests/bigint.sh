#!/bin/bash

TEST=../build/test_bigint/taio

# Usage: rand <digits>
# Prints random number with at most <digits> digits
rand() {
	if [ -z "$1" ]; then
		DIGITS=10
	else
		DIGITS="$1"
	fi

	python -c "import random as R; print(R.randint(0, 10**${DIGITS}))"
}

# Due to large sizes of numbers simple [ NUM1 -ge NUM2 ] will not work
compare() {
	if [ $(bc <<< "$1 > $2") -eq 1 ]; then
		return 1
	fi

	return 0
}

single_test() {
	# Get operation
	if [ ! -z "$1" ]; then
		OPERATION="$1"
	else
		OPERATION="+"
	fi

	# Get digits count
	if [ ! -z "$2" ]; then
		DIGITS="$2"
	fi

	NUM1=$(rand "$DIGITS")
	NUM2=$(rand "$DIGITS")

	# Swap NUM1 with NUM2 if performing subtraction and NUM2 is greater than NUM1
	if [ "$OPERATION" = "-" ] && compare $NUM1 $NUM2; then
		NUMTMP="$NUM1"
		NUM1="$NUM2"
		NUM2="$NUMTMP"
	fi

	RESULT=$(./${TEST} "$OPERATION" "$NUM1" "$NUM2")
	EXPECTED=$(python -c "print($NUM1 $OPERATION $NUM2)")

	STATUS="NIE ZDANE"
	if [ "$RESULT" = "$EXPECTED" ]; then
		STATUS="    ZDANE"
	fi

	echo "Status: [$STATUS] | $NUM1 $OPERATION $NUM2 = $RESULT | Oczekiwany wynik: $EXPECTED"
}

for i in $(seq 1 40); do
	single_test + $i
	single_test - $i
done
