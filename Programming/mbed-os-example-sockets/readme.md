# Arrowhead core systems demo

# Prerequisites 

## Arrowhead local cloud in insecure mode.
You need a local arrowhead cloud with all core services running insecure mode in order to run this example. For this example I used the compile code option to install arrowhead. To disable secure make sure to edit the ssl.enabled = true to false in all the core systems, i.e. service registry, authorization and orchestrator, application.properties file. That file is located in the target folder of each individual core system. 

Go to eclipse arrowheads [github](https://github.com/eclipse-arrowhead/core-java-spring) for information on that.

## ST-Link drivers
You need to install the ST-link drivers from [here](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-utilities/stsw-link009.html). In order to download you have to have an ST account. Download the file, unzip and install the version approriate to your system. To check that it works plug in your st-device and make sure it appears as a unit on your computer.

## Tera term
To view the output this example you need to have tera term installed. Tera term can be downloaded from this [link](https://osdn.net/projects/ttssh2/releases/). Once there download and install the latest version. 

## Tera tern setup
When you start tera term you need to change a couple of settings for the terminal so that the output presents itself in a nice way. In the terminal settings change new-line receive to AUTO and transmit to LF. 

## MBED online compiler.

Create an account on [ARMmbed](https://ide.mbed.com/compiler/) and import the project from github.

Make sure that when selecting device to compile to you select B-L475E-IOT01A, even if you have B-L4S5I-IOT01A otherwise the example will not compile.

In order to import this project into your MBED online compiler simply follow this [link](https://os.mbed.com/compiler/#import:https://github.com/AlbinMartinsson/arrowhead_stm32_demo). 


# Configuration

## mbed_app.json
In this file you have to change the wifi credentials, add your SSID and password. 

## main.cpp
Here you have to change the address on all calls from localhost to the IP-address of your local arrowhead cloud. You also have to change the address of your provider and consumer body to match this. The provider address should be the same as your arrowhead cloud and the consumer should be the address of your stm32 board. Take note of which port you define to the provider as you will use it in your flask app.

## <span>flaskapp.p</span>y
Make sure that the port matches the port of the provier system in main.cpp and that the address is set to 0.0.0.0 to allow external communication. 

# Complilation and running

## flask app
Start the flask app with the run configurations specified in the previous section.

## mbed compiler
Do the changes in the previous section and hit ctrl+b to compile the project. This will download a .bin file. To run that .bin file on your board simply drag it to the board in the explorer. As if you were putting a file on a flash drive. 

## Expected output and result
What this demo does is:
* Register a provider system to the service registry.
* Register a consumer system to the service registry.
* Register a service to the service registry.
* Creates an intra cloud authorization rule.
* Creates an orchestrator store entry.
* Uses the id of the consumer to find an appropriate consumer.
* Based on the response from the orchestration gets the url to the provider and sends the temperature as a json to it. 

# Future work and modifications.

## Secure mode
This example needs to implement HTTPS some time in the future. There is support in this library for HTTPS. I have had issues with the certificates and the format of those.

## Continuously provide temperature data.
Right now the temperature is only posted once, to make this somewhat useful it should post continuously.

## Data logger
A nice way to present the temperature data would be very useful.


