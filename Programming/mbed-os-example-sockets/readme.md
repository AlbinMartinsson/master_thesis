# Arrowhead core systems demo

## Prerequisites 

### Arrowhead local cloud in insecure mode.
You need a local arrowhead cloud with all core services running insecure mode in order to run this example.

### ST-Link drivers
You need to install the ST-link drivers from https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stsw-link007.html

### Tera term
To view the output this example you will need to have tera term installed. Tera term can be downloaded from this [link](https://ttssh2.osdn.jp/index.html.en)

#### Tera tern setup
When start tera term you need to change a couple of settings for the terminal so that the output presents itself in a nice way. In the terminal settings change new-line receive to AUTO and transmit to LF. 

### MBED online compiler.

In order to import this project simply follow the link https://os.mbed.com/compiler/#import:https://github.com/AlbinMartinsson/arrowhead_stm32_demo

### Configuration
In mbeb_app.json enter your wifi credentials. In main.cpp modify the address to your local arrowhead cloud and change the address from "localhost" to the addresses of the provider and consumer you want to use.

### Expected output 
This example can be run as is without the last part that connects and posts the temperature to the provider. Comment out this part if you dont have a server. The output of the example is the response body from the orchestration, containing the information of the provider. That address will also be parsed and used to connect to the provider.

### Flask app
If you want to try to post from to the board to the provider found in the orchestration you can to this by running a very simple flask app. You can start the flaskapp.py but you need to edit run configuration to run on 0.0.0.0 and the port specified in your service registry.

