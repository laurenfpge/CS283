#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "multiple quoted phrases parse correctly" {
	run ./dsh <<EOF
echo "-one dog-" and "-two cats-" and "-three fish-"
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="-onedog-and-twocats-and-threefish-dsh2>dsh2>cmdloopreturned0"
	
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

@test "'cd' with no arguments does not change current directory" {
	current=$(pwd)
	run ./dsh <<EOF
cd
pwd
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="${current}dsh2>dsh2>dsh2>cmdloopreturned0"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

@test "'cd' is successful" {
	current=$(pwd)
	run ./dsh <<EOF
cd dir1
pwd
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="${current}/dir1dsh2>dsh2>dsh2>cmdloopreturned0"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

@test "'cd' with more than one argument does nothing + error code" {
	run ./dsh <<EOF
cd dir1 extra
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="dsh2>toomanyargumentsforcddsh2>cmdloopreturned-4"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]
	
	[ "$status" -eq 0 ]
}

@test "'cd' is case sensitive" {
	run ./dsh <<EOF
CD dir1
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="execvperror:Nosuchfileordirectorydsh2>dsh2>dsh2>cmdloopreturned0"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

@test "'pwd' is case sensitive" {
	run ./dsh <<EOF
PWD
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="execvperror:Nosuchfileordirectorydsh2>dsh2>dsh2>cmdloopreturned0"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

@test "'cd' into nonexistent directory fails" {
	run ./dsh <<EOF
cd dir2
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="cderror:Nosuchfileordirectorydsh2>dsh2>cmdloopreturned-4"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

@test "'echo' with a lot of spaces and quotes" {
	run ./dsh <<EOF
echo        "    one    and three"         "but     also what    "      "hello world"     
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="oneandthreebutalsowhathelloworlddsh2>dsh2>cmdloopreturned0"

	echo "Output: $output"
	echo "Exit Status: $status"
	echo "actual: ${stripped_output}"
	echo "expect: ${expected_output}"

	[ "$stripped_output" = "$expected_output" ]

	[ "$status" -eq 0 ]
}

