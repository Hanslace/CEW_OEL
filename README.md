# Weather Monitor

## Description
An integrated environmental monitoring system in C, covering a range of fundamental concepts and practical applications. The project involves interacting with a free API that provides real-time environmental data. The system's core functionalities include data retrieval, processing and reporting.

## Setup
First of all, clone the repository in to your linux or wsl in windows machine:  
git clone https://github.com/Hanslace/CEW_OEL.git  
 
How to set up WSL?  
https://learn.microsoft.com/en-us/windows/wsl/setup/environment#set-up-your-linux-username-and-password  
Next use remote explorer extension of VS code to connect to it.  

Use the CLI to move into the cloned directory.  

Run the following command when in the cloned directory and replace the placeholder values:  
cat <<EOL > .env  
SENDER_EMAIL=your_sender_email@example.com  
SENDER_PASSWORD=your_sender_password  
RECIPIENT_EMAIL=your_recipient_email@example.com  
OPENWEATHER_API_KEY=your_openweather_api_key  
CITY_NAME=your_city_name  
EOL  
  
For execution of precomiled executable:  
./monitoring_system  

If you make changes to your codes file, recompile your code using the following commands:  

### Only Once:  
sudo apt update  
sudo apt install -y zenity curl libcjson-dev  

### Everytime  
make clean  
make  

## Automation  
To automate the program, use crontab:  
sudo apt update  
sudo apt install -y cron  
crontab -e (This will open the editor, on first open select the 1st editor and it will be set to default)  

Add this line to the opened file in the editor to set the time for automation:  
 ┌───────────── Minute (0 - 59, * for every)  
 │ ┌───────────── Hour (0 - 23, * for every)  
 │ │ ┌───────────── Day of Month (1 - 31, * for every)  
 │ │ │ ┌───────────── Month (1 - 12, * for every)  
 │ │ │ │ ┌───────────── Day of Week (0 - 7) (Sunday is both 0 and 7) (* for every)  
 │ │ │ │ │  
\* * * * * /path/to/your/directory/scripts/run_monitoring.sh  (This is the command, above is labelling, put in the path to your run_monitoring.sh from the root directory)  

To start cron jobs:  
sudo systemctl start cron

To check jobs status:  
sudo systemctl status cron

## That`s it, THANK YOU!!