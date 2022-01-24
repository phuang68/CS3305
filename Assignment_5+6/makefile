CC = gcc
H = read_input.h types.h
C = main.c read_input.c
EXEC = a6

build: ${C} ${H}
	${CC} ${C} -o ${EXEC} -pthread

test:
	@echo "Running a6 on assignment_6_input_file.txt"
	./${EXEC} assignment_6_input_file.txt
	diff assignment_6_output_file.txt expected_output.txt
	@echo "Passed!"

clean:
	@echo "Cleaning up..."
	rm -f ${EXEC} assignment_6_output_file.txt