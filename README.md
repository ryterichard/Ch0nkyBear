# Ch0nkyBear

## Architecture  

![Image](/resources/images/architecture_diagram.png)

### C2 Server (Check server folder for detailed features and running instructions)
-   Handles connections from multiple operators and implants
-   Using Flask as primary listener with Gunicorn as the WSGI
-   Using flask-SqlAlchemy to facilitate CRUD operations on agents and operators
-   UI Interface to control the server and interact with the implant

### Implant (Check malware folder for detailed features and running instructions)
- checks if target has ch0nky.txt on file
-   Situational awareness: 
    - Machine GUID (primary key)
	- Computer Name
	- Username
	- Can execute remote commands so can escalate priveleges and gain any system info if required.
-   Queries RUN key to check if implant has already infected the system.
-   Execution
	-   Remote Command Execution
	-   DLL Injection 
-   Loot: chromium stealer - usernames, passwords, cookies from all user profiles
-   Persistence - RUN registry key
-   Defense Evasion - UPX packer to obfuscate payload.

### Database - PostgreSQL
-   Clients: operators connected to the C2
    - Client can remotely execute commands on specific implants registered with the C2 server
-   Jobs: Jobs sent to implants along with status of the job and the output response
-   Implants:
	-   Machine GUID (primary key)
	-   Computer Name
	-   Username
	-   Commands executed and responses
	-   First Seen: First check in/ register time
	-   Last Seen: Last comm with agent

### To Get Started:
> Caution: This will infect your system with malware. Ensure you are following it in a sandboxed environment. As a cautionary measure the malware will check if C:\malware\ch0nky.txt exists on the local filesystem. Well if the file exists then you know what you are doing. You have been warned.

Download the [BU-IT-Support.zip](BU-IT-Support.zip), extract and open the Word Document (Macros will have to be enabled).

Steps on how to run individual components seperately are noted in the respective Readme files.