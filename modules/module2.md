# Module 2 - Set up Azure IoT Edge

In this module, we are going to do the following:
- set up a virtual machine in Azure for our IoT Edge device
- create the IoT Edge device in Azure and configure Edge on the VM
- configure that IoT Edge device as a transparent gateway
- download and store the root CA certificate needed for our MXChip

## Setup IoT Edge virtual machine

In this section, we are going to create and setup an Azure VM as the IoT Edge box.  Technically, it a 'real' deployment, you would use something on-premises, such as a local hardware device (PC, Raspberry Pi, VM, etc) as your IoT Edge device.  However, just for ease of setup here, we are going to use a VM in Azure. If you already have, or prefer to use, a local device, feel free. For that option, you can find the IoT Edge setup instructions here (TODO: link)

To create the IoT Edge VM in Azure, we are going to creat the VM via the Azure CLI. Specifically, to make things even easier, we are going to use a "cloud shell" in Azure to keep from having to install the Azure CLI locally.

To create a "cloud shell" instance, login to the Azure portal, and click the button that looks like a ">_" next to the search bar at the top

![cloud shell icon](/images/cloud-shell-icon.png)

You may be required to provision some storage to create the cloud shell. For this lab, choose "bash" as the type of shell.

Once you have the shell launched, we are ready to create a Virtual Machine. For this VM, we are going to use the pre-built "Ubuntu 16.04 with Azure IoT Edge runtime" VM.  This is a version of Ubuntu 16.04 Linux with the IoT Edge runtime pre-installed.

Before we create the VM, let's create a resource group in Azure to contain our project services, if you don't already have one (if you already have one, feel free to skip)

In the cloud shell, run the following command to create your resource group

```bash
az group create --name {your resource group name} --location {location}
```

where {your resource group name} is the name you want to call your resource group and {location} is the Azure region in which you want to work (i.e. 'centralus'). You can find a list of available azure regions (and their abbreviations) by running this command 'az account list-locations -o table' in the azure CLI

To create the VM, run this command

```bash
az vm create \
--resource-group [resource_group] \
--name [vm name] \
--location [location] \
--image microsoft_iot_edge:iot_edge_vm_ubuntu:ubuntu_1604_edgeruntimeonly:1.0.1 \
--admin-username [userid] \
--admin-password [password] \
--authentication-type password \
--size Standard_B1ms \
--public-ip-address-dns-name [vm name]
```

where
- resource_group is the name you used above when created the resource group
- vm name is a globally unique name for your VM
- location is the location you created your RG in above
- userid is the login id you want to use for your VM
- password is a very strong password you want to use for the userid above

This will create a VM with the iot edge runtime already installed and assign it the DNS name [vm name].[location].cloudapp.azure.com   (for example steveiotedgevm.centralus.cloudapp.azure.com).  

NOTE:  note the DNS name of your VM, as you will use that later in the IoT Edge setup and to point your MXChip at it

# Creat IoT Edge device in Azure and configure IoT Edge on the VM

Before we do anything related to IoTHub, we need to add the CLI extension for IoT. To do this, run the following command:

```bash
az extension add --name azure-cli-iot-ext
```
You should only need to do that once.

If you haven't already created an IoT Hub, you can do so with this command in the CLI

```bash
az iot hub create --name {your iot hub name} --resource-group {your resource group name} --sku S1
```

{your iot hub name} needs to be a globally unique name for your hub

Once the IoT Hub is created, you can create an IoT Edge device in your IoT Hub with this command

```bash
az iot hub device-identity create --device-id [device id] --hub-name [hub name] --edge-enabled
```

where [device id] is a name you make up for your edge device.  It does not need to be globally unique, just unique within your hub.  No underscores, no spaces!

Once the device is created, we need to grab it's connection string

```bash
az iot hub device-identity show-connection-string --device-id [device id] --hub-name [hub name]
```

Copy the connection string returned over to notepad or similar, we will need it later.

The final step (for now) in the portal is to go ahead and set a default config for our edge device, which ensures that the edge runtime gets fully deployed as soon as our edge devices gets configured for the cloud.

To set the initial configuration for our device, run this command  (note the string tick marks at the beginning and end of the json payload. Make sure you get them too):

```bash
az iot edge set-modules -n [hub name] -d [devuce id] -k '
{
    "modulesContent": {
        "$edgeAgent": {
            "properties.desired": {
                "modules": {},
                "runtime": {
                    "settings": {
                        "minDockerVersion": "v1.25"
                    },
                    "type": "docker"
                },
                "schemaVersion": "1.0",
                "systemModules": {
                    "edgeAgent": {
                        "settings": {
                            "image": "mcr.microsoft.com/azureiotedge-agent:1.0",
                            "createOptions": ""
                        },
                        "type": "docker"
                    },
                    "edgeHub": {
                        "settings": {
                            "image": "mcr.microsoft.com/azureiotedge-hub:1.0",
                            "createOptions": "{\"HostConfig\":{\"PortBindings\":{\"443/tcp\":[{\"HostPort\":\"443\"}],\"5671/tcp\":[{\"HostPort\":\"5671\"}],\"8883/tcp\":[{\"HostPort\":\"8883\"}]}}}"
                        },
                        "type": "docker",
                        "status": "running",
                        "restartPolicy": "always"
                    }
                }
            }
        },
        "$edgeHub": {
            "properties.desired": {
                "routes": {
                    "route": "FROM /messages/* INTO $upstream"
                },
                "schemaVersion": "1.0",
                "storeAndForwardConfiguration": {
                    "timeToLiveSecs": 7200
                }
            }
        }
    }
}
'
```

### Allow MQTTs traffic to VM

The last thing we need to in the portal is to allow MQTTS traffic into our VM from external.  In the Azure Portal, navigate to "Virtual Machines", choose your VM, then click on "Networking" on the left-hand nav.  On the Networking blade, select "Add inbound port rule"

- for destination port range, enter 8883
- for Port Name, enter "AllowMQTTs"

click 'Add'

## Configure IoT Edge as a transparent gateway

### Basic IoT Edge setup

Using your favorite SSH client (if you don't have one, I recommend [putty](http://putty.org)), connect to your IoT Edge VM via SSH by it's DNS name ([vm name].[location].cloudapp.azure.com).  Once you are logged in and have your bash prompt, run

```bash
sudo nano /etc/iotedge/config.yaml
```

This will open up the iot edge configuration file in the nano editor.  Arrow down to the 'provisioning' section, where you should see a line for 'device connection string'.  Replace \<add device connection string here> with your connection string

TIP:  erase everything inside the quotes, position your cursor inside the quotes, copy your connection string, and then right-click in the putty terminal window, which will 'paste' the string in

Scroll down to the "Edge device hostname" section and find the 'hostname' parameter. To the end of the short hostname, append ".[location].cloudapp.azure.com" such that the hostname matches the public DNS name of your VM

Save the file by hitting CTRL-O, \<enter>, then CTRL-X to exit

now we need to restart iotedge

```bash
sudo systemctl restart iotedge
```

Once it returns, let's check the status of Edge

```bash
sudo systemctl status iotedge
```

You should see a status that looks similar to this..

![success-restart](/images/iot-edge-status.png)

Note the green 'active' status.  If yours doesn't have the success, recheck your entries and restart edge until it does.

### Create and install Edge certificates

In order to have downstream or 'leaf' devices (like our MXChip), you must configure IoT Edge with certificates. Specifically, the end result will be a certificate that IoT Edge uses under the covers as the 'server' certificate it returns whenever a client tries to connect to prove to the client that it is the legitimate IoT Edge box it is trying to connect to.

In order to create and install these certificates, follow the process outlined [here](https://docs.microsoft.com/en-us/azure/iot-edge/how-to-create-transparent-gateway), except stop before the "Deploy Edgehub to the gateway" section (we've already done that).

The final step with certificates is to install the certificates on the local IoT Edge OS.  To do so, follow [this section](https://docs.microsoft.com/en-us/azure/iot-edge/how-to-connect-downstream-device#ubuntu).  Only execute the two commands in that section and return back here.

Once we are done, restart IoT Edge again with these commands and make sure it's up and going again..

```bash
sudo systemctl restart iotedge
sudo systemctl status iotedge
```

Finally, let's ensure that the certificates are valid by connecting to our IoT Edge box over a TLS connection.

To do so, run

```bash
openssl s_connect -connect [vm name].[location].cloudapp.azure.com:8883 --showcerts
```

This will connect to our IoT Edge device similarly to an IoT leaf device would an validates the certificate.

At the top of the ouput, you can see the certificate chain, as explain in this article (TODO:  link to cert article).  However, the most important item is the verification at the bottom.  I should look like this (with the "Verify (ok)" output)

![verified cert](/images/iot-edge-verify-cert.png)

## Grab the root CA certificate and save it for later use

Now that we have a functional IoT Edge box, the last step is to grab the root CA certificate and save it for later use.

To grab the certificate, run

```bash
openssl x509 -in <WRKDIR>/certs/azure-iot-test-only.root.ca.cert.pem -text
```

where WRKDIR is the working directory you used when you ran the certificate generation scripts.

Your output will look something like this

![root cert](/images/iot-edge-root-cert.png)

Copy everything, including the 'BEGIN CERTIFICATE' and 'END CERTIFICATE' lines and store for later use.

You are now ready to move on to [Module 3](modules/module3.md) and get your MXChip up and talking to IoT Edge


