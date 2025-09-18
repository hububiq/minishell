Object of this project was to create a simple shell, imitating Bash, with command-line prompt and history.

The job was to handle:
- **environment variable expansion** - including single and double quotes edge cases,
- **pipes**,
- **redirections and heredoc**
- **last exit code**

Program should also be operational in terms of keyboard shortctus with the likes of Ctrl+C, Ctrl+D - **signals** implementation.
**Built-ins** like echo, pwd, cd, export, unset, env, exit also had to be implemented. 
