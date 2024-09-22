#!/bin/bash

amount_total=0
amount_passed=0
amount_failed=0

run() {
	$cmd
	exit_observed="$?"
}

# [code] [command] [message]
# Assert that [command] exits with [code]
assert() {
	((amount_total++))
	exit_expected="$1"
	cmd="$2"
	msg="$3"
	echo -n "Running test $amount_total: $msg... "
	run > /dev/null 2>&1
	if [[ $exit_observed -ne $exit_expected ]]
	then
		((amount_failed++))
		echo "FAILED."
		echo "    Expected exit code $exit_expected, got $exit_observed."
	else
		((amount_passed++))
		echo "OK"
	fi
}

cd build
WDA="./wda --save-location=test.txt"
echo "" > "test.txt"
assert 0 "./wda" "doesn't crash with no parameters"
assert 0 "./wda --nonsense-parameter" "doesn't crash with nonsense parameters"
assert 0 "$WDA --help" "help works"
assert 0 "$WDA aa aaaa --help" "help overrides"
assert 1 "$WDA locations" "no location subcommand specified"
assert 0 "$WDA locations add 1 51.4778 0 Greenwich Royal Observatory" "location add works"
assert 1 "$WDA locations add 1 0 0 point null" "location add fails with duplicate id"
assert 1 "$WDA locations add 100 0 X Y" "location add fails with non-numedic coord"
assert 0 "$WDA locations list" "list works"
assert 1 "$WDA locations search" "search fails on invalid params"
assert 0 "$WDA locations search 1" "search runs on id"
assert 0 "$WDA locations search blah blah" "search runs on location"
assert 0 "$WDA query 1" "query with defaults works"
assert 1 "$WDA query 1 --temperature-unit=1" "query with bad temp unit fails"
assert 1 "$WDA query 1 --time-format=windowstime" "query with bad time format fails"
L="$WDA query 1 --relative-time=0,1 --absolute-time=2000-01-01,2000-01-02"
assert 1 "$L" "query mixing time filters fails"
L="$WDA query 1 --relative-time=0,1 --model=bom_access_global --type=daily --temperature-unit=fahrenheit"
L="$L --wind-speed-unit=kn --timezone=Europe/Berlin"
assert 0 "$L" "query with lots of parameters works"
assert 1 "$WDA query 1 --pressure-level=30" "pressure level requires type hourly"
assert 0 "$WDA query 1 --type=hourly --pressure-level=30" "pressure level query works"

echo ""
echo "Tests passed: $amount_passed/$amount_total"
if [[ $amount_failed -ne 0 ]]
then
	echo "Tests failed: $amount_failed"
	exit 1
fi

exit 0
