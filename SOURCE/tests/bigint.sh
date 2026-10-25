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

	python -c "import sys; import random as R; sys.set_int_max_str_digits(1000000); print(R.randint(10**$((${DIGITS} - 1)), 10**${DIGITS} - 1))"
}

# Due to large size of numbers, simple [ NUM1 -ge NUM2 ] will not work
compare() {
	if [ $(bc <<< "$1 > $2") -eq 1 ]; then
		return 1
	fi

	return 0
}

# Usage: single_test <operation> <digits>
# Perform single bigint test with numbers of up to <digits> long
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

	# Get whether to print full info or just if test passed or not
	if [ ! -z "$3" ]; then
		FULLINFO="$3"
	fi

	NUM1=$(rand "$DIGITS")
	NUM2=$(rand "$DIGITS")

	# Swap NUM1 with NUM2 if performing subtraction and NUM2 is greater than NUM1
	if [ "$OPERATION" = "-" ] && compare $NUM1 $NUM2; then
		NUMTMP="$NUM1"
		NUM1="$NUM2"
		NUM2="$NUMTMP"
	fi

	# Save operation and both numbers in file
	INPUT_FILE=$(mktemp)
	echo "$OPERATION" "$NUM1" "$NUM2" > "$INPUT_FILE"

	RESULT=$(./${TEST} "$INPUT_FILE")
#	EXPECTED=$(python -c "import sys; sys.set_int_max_str_digits(0); c = $NUM1 $OPERATION $NUM2; print(c)")
	EXPECTED=$(python -c "import sys; sys.set_int_max_str_digits(0); f = open(\"${INPUT_FILE}\"); op, a, b = f.read().split(); f.close(); print(int(a) ${OPERATION} int(b))")

	# Remove file with operation and numbers
	rm "$INPUT_FILE"

	STATUS="NIE ZDANE"
	if [ "$RESULT" = "$EXPECTED" ]; then
		STATUS="    ZDANE"
	fi

	if [ "$FULLINFO" = "TRUE" ]; then
		echo "Status: [$STATUS] | $NUM1 $OPERATION $NUM2 = $RESULT | Oczekiwany wynik: $EXPECTED"
	else
		echo "Status: [$STATUS] | Długości dodawanych liczb: $DIGITS"
	fi
}

# Loop and test numbers of incremental lengths
for i in $(seq 1 40); do
	single_test + $i "TRUE"
	single_test - $i "TRUE"
done

# Test some ultra-long numbers
lengths=(100 1000 10000 100000 1000000)
for i in "${lengths[@]}"; do
	single_test + $i "FALSE"
done
