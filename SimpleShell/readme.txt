Group Number: 105
Members:
- Chawan Srujeeth (2022141)
- Rathod Uttam Kumar (2021277)

GitHub Link: [GitHub Repository](https://github.com/chawansrujeeth/os-assignment-2)

Project Description:
Our project, named "SimpleShell," is designed to read user commands from standard input and execute them. It operates in a continuous loop, accepting user inputs until the "exit" command is given or the user presses Ctrl+C to exit.

Main Functionality:
1. cd:--  We have implemented the "cd" command to change the current working directory.
2. history:-- Our shell maintains a history of executed commands.
3. exit:--The "exit" command allows the user to terminate the shell.
4. pipe:-- We support pipeline commands for executing multiple commands in sequence.

Implementation Details:
- In the main function, we have set up the shell loop that continually reads user commands.
- For most commands, we use an `execute_command` function that handles forking and executing child processes.
- The `execute_command` function also stores command history, including the command, process ID (PID), and execution details.
- We have opted for fixed-size arrays for storing inputs and history rather than using dynamic memory allocation (malloc/realloc) due to simplicity.

Contributions:
- Uttam Kumar Rathod: Focused on creating and implementing the history feature and handling the execution of commands using the `exec` function.
- Srujeeth Chawan: Implemented features such as "cd," "exit," "history," and pipeline commands. Collaborated with Uttam to discuss and implement various functionalities.

Our collaborative effort and discussions during the project have resulted in the successful implementation of SimpleShell. Thank you for considering our project.