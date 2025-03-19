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

@test "exit works" {
	run ./dsh << EOF
exit
EOF
	
	[ "$status" -eq 0 ]

}

@test "stop-server works" {
	run ./dsh << EOF
stop-server
EOF
	
	[ "$status" -eq 0 ]

}

@test "piping test: echo|rev" {
	run ./dsh << EOF
echo "hello world" | rev
EOF
	actual=$(echo "$output" | tr -d '[:space:]')
	expected="dlrowollehlocalmodedsh4>dsh4>cmdloopreturned0"

	echo "exit status: $status"
	echo "${actual} -> ${expected}"

	[ "$actual" = "$expected" ]
	[ "$status" -eq 0 ]

}

@test "piping test: echo with extra spaces|wc" {
	run ./dsh << EOF
echo "cherry     apple    banana" | wc -c 
EOF
	actual=$(echo "$output" | tr -d '[:space:]')
	expected="27localmodedsh4>dsh4>cmdloopreturned0"

	echo "exit status: $status"
	echo "${actual} -> ${expected}"

	[ "$actual" = "$expected" ]
	[ "$status" -eq 0 ]

}

@test "piping test: cat|grep|wc" {
	run ./dsh << EOF
cat test | grep "o" | wc -l
EOF
	actual=$(echo "$output" | tr -d '[:space:]')
	expected="3localmodedsh4>dsh4>cmdloopreturned0"

	echo "exit status: $status"
	echo "${actual} -> ${expected}"

	[ "$actual" = "$expected" ]
	[ "$status" -eq 0 ]

}

@test "lz fails (nonexistent command)" {
	run ./dsh << EOF
lz
EOF
	actual=$(echo "$output" | tr -d '[:space:]')
	expected="execvperror:Nosuchfileordirectorylocalmodedsh4>localmodedsh4>dsh4>cmdloopreturned0"

	echo "exit status: $status"
	echo "${actual} -> ${expected}"

	[ "$actual" = "$expected" ]
	[ "$status" -eq 0 ]

}

@test "multiple repeated commands work" {
	run ./dsh << EOF
echo "my dog looks cute with a ponytail"
pwd
echo "how about your dog"
pwd
EOF
	actual=$(echo "$output" | tr -d '[:space:]')
	expected="mydoglookscutewithaponytail/home/lfg34/cs283/assignments/6-RShellhowaboutyourdog/home/lfg34/cs283/assignments/6-RShelllocalmodedsh4>dsh4>dsh4>dsh4>dsh4>cmdloopreturned0"

	echo "exit status: $status"
	echo "${actual} -> ${expected}"

	[ "$actual" = "$expected" ]
	[ "$status" -eq 0 ]

}

@test "create file with touch" {
	run ./dsh << EOF
touch katseye
EOF
	actual=$(echo "$output" | tr -d '[:space:]')
	expected="localmodedsh4>dsh4>cmdloopreturned0"

	echo "exit status: $status"
	echo "${actual} -> ${expected}"

	[ "$actual" = "$expected" ]
	[ "$status" -eq 0 ]

}
