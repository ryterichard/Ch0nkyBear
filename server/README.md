# C2 Server

Ch0nkyBear server is hosted on https://ch0nkybear.herokuapp.com/

- Using Python Flask as primary listener with Gunicorn as WSGI
- MySQL as backend database

## Features

- Stores client, target and job details
- Checks if request header for an exe contains User-Agent: ch0nky (emulates ch0nky, ensures only implant downloads the exe, unintended entities download calculator exe)
- User authentication
- Incoming request hex encoded
- Send command to specific implants

![Image](../resources/images/control_panel.PNG)

![Image](../resources/images/implant_details.PNG)

![Image](../images/implant_details2.PNG)
