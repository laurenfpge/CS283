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

@test "quotation parsing still holds with pipes" {
	run ./dsh <<EOF
ls | grep ".c"
EOF
 	
    [ "$status" -eq 0 ]	
}

@test "multiple spaces still works after refactoring" {
	run ./dsh <<EOF
echo emmys     grammys    oscars    tonys
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="emmysgrammysoscarstonysdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]	

}

@test "piping test 1" {
	run ./dsh <<EOF
echo "hello world" | rev
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dlrowollehdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]	

}

@test "piping test 2" {
	run ./dsh <<EOF
echo "cherry apple banana" | wc -c
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="20dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]	

}

@test "piping test 3" {
	run ./dsh <<EOF
ls | sort
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="batsdshdsh_cli.cdshlib.cdshlib.hmakefilequestions.mdtestdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]	

}

@test "piping test 4 (accessing test file)" {
	run ./dsh <<EOF
cat test | grep "o" | wc -l
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="3dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]	

}

@test "nonexistent command fails" {
	run ./dsh <<EOF
lz
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="execvperror:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]	

}







